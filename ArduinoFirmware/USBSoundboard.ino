//################################## BEGIN INCLUDES ##################################
#include <Adafruit_NeoPixel.h>
#include <TaskScheduler.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
 #include <avr/sleep.h> // for noise reduction
#endif
//################################### END INCLUDES ###################################

//################################### BEGIN DEFINES ##################################
#define LED_PIN     6  //set the data pin of the led strip
#define LED_COUNT   10 //set the number of leds
#define NUM_BUTTONS 10 //set the number of buttons
#define BRIGHTNESS  25 //set the led brightness (max = 255, good = 125)

//the power switch pin
const int powerSwitch = 15;

//the power switch led pin
const int pswLED = A0;

//the status of the power switch;
//true = on, false = off
static bool power;
//#################################### END DEFINES ###################################

//############################## BEGIN INITIALIZATIONS ###############################
//define the led strip object 
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//the pins for all of the buttons
const int buttonInputs[NUM_BUTTONS] = {2, 3, 4, 5, 7, 8, 9, 10, 16, 14};

//the states of all of the buttons
int buttonStates[NUM_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//whether or not this led should be ignored for the idle wave effect (i.e. it is being set by the flash function)
bool ignore[LED_COUNT] = {false, false, false, false, false, false, false, false, false, false};

//initialize the TaskScheduler object
Scheduler runner;
//############################### END INITIALIZATIONS ################################

//################################# BEGIN FUNCTIONS ##################################
//called by taskscheduler
void readPswCallback(){
  //read the status of the power switch digitally
  int powerStatus = digitalRead(powerSwitch);
  if(powerStatus == HIGH){
    power = true;
    digitalWrite(pswLED, LOW);  //when device on, psw led off
  }
  else{
    power = false;
    digitalWrite(pswLED, HIGH); //when device off, psw led on
    //turn off leds and update the led strip
    strip.clear();
    strip.show();
    //adc low power mode
    sleep_mode();
  } 
}

//reads serial communications from the computer which contains data on which led/button to flash/ignore
void readSerial() {
  //buffer to hold incoming data
  static String buffer = "";
  while (Serial.available() > 0) {
    //read one character at a time
    char incoming = Serial.read();
    if (incoming == '\n') { //reached end of message
      //process the full message
      parseSerial(buffer);
      //clear the buffer for the next message
      buffer = "";
    } else {
      //append character to buffer
      buffer += incoming;
    }
  }
}

//helper function for readSerial();
//updates the arrays 'buttonStates' and 'ignore'
void parseSerial(const String& data) {
  int idx = 0;
  for (int i = 0; i < data.length() && idx < NUM_BUTTONS; i++) {
    if (data[i] == '0') {
      buttonStates[idx] = 0;
      ignore[idx] = false;
      idx++;
    } else if (data[i] == '1') {
      buttonStates[idx] = 1;
      ignore[idx] = true;
      idx++;
    }
  }
}

//called by taskscheduler
void flashTaskCallback(){
  if(!power) return;
  static bool cycle[NUM_BUTTONS] = {false, false, false, false, false, false, false, false, false, false};
  for(int i = 0; i<NUM_BUTTONS;i++){
    if(ignore[i]){
      if(cycle[i]){
        strip.setPixelColor(i, strip.ColorHSV(0));
        cycle[i] = false;
      }else{
        strip.setPixelColor(i, strip.ColorHSV(1023*32));
        cycle[i] = true;
      }
    }
  }
}

//called by taskscheduler
void rgbWaveTaskCallback(){
  if(!power) return;
  rainbow();
  strip.show();
}

//a nice rainbow wave rgb effect (does NOT call strip.show());
//called by rgbWaveTaskCallback()
void rainbow() {
  static uint16_t j = 0;
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    if(ignore[i] == false)
      strip.setPixelColor(i, Wheel((i * 256 / strip.numPixels() + j) & 255));
  }
  j++;
}

//a helper function for the rainbow() function
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

//reads all button states to array buttonStates
void readDigital(){
  if(!power) return;
  //check for any button presses
  for(int i=0;i<NUM_BUTTONS;i++){
    if(digitalRead(buttonInputs[i])==LOW)
      buttonStates[i] = 1;
    else 
      buttonStates[i] = 0;
  }
}

//sends serial communications to the computer;
//contains data about each button;
void sendSerial(){
  if(!power) return;
  String result = String("");

  for (int i = 0; i < NUM_BUTTONS; i++) {
    result += String((int)buttonStates[i]);

    if (i < NUM_BUTTONS - 1) {
      result += String("|");
    }
  }
  
  Serial.println(result);
}
//################################### END FUNCTIONS ##################################

//############################## BEGIN TASK DEFINITIONS ##############################
Task readPowerStatusTask(1, TASK_FOREVER, &readPswCallback);
Task readSerialTask(1, TASK_FOREVER, &readSerial);
Task flashTask(500, TASK_FOREVER, &flashTaskCallback);
Task rgbWaveTask(25, TASK_FOREVER, &rgbWaveTaskCallback);
Task readDigitalTask(1, TASK_FOREVER, &readDigital);
Task sendSerialTask(1, TASK_FOREVER, &sendSerial);
//############################### END TASK DEFINITIONS ###############################

//############################### BEGIN RUNTIME SETUP ################################
void setup() {
  //set pinmodes
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonInputs[i], INPUT_PULLUP);
  }
  pinMode(powerSwitch, INPUT_PULLUP);
  pinMode(pswLED, OUTPUT);
  //initialize, update, and configure the NeoPixel strip object
  strip.begin();           
  strip.show();
  strip.setBrightness(BRIGHTNESS);
  //start serial communication with computer
  Serial.begin(9600);
  //add tasks to the scheduler
  runner.addTask(readPowerStatusTask);
  runner.addTask(readSerialTask);
  runner.addTask(flashTask);
  runner.addTask(rgbWaveTask);
  runner.addTask(readDigitalTask);
  runner.addTask(sendSerialTask);
  //enable the tasks
  readPowerStatusTask.enable();
  readSerialTask.enable();
  flashTask.enable();
  rgbWaveTask.enable();
  readDigitalTask.enable();
  sendSerialTask.enable();
}
//################################ END RUNTIME SETUP #################################

//#################################### BEGIN LOOP ####################################
void loop() {
  //let the taskscheduler handle tasks
  runner.execute();
}
//##################################### END LOOP #####################################