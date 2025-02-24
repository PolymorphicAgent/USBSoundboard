/****************************************************************************
** Meta object code from reading C++ file 'soundboard.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../soundboard.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'soundboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10SoundboardE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10SoundboardE = QtMocHelpers::stringData(
    "Soundboard",
    "sendSerial",
    "",
    "refreshSerialPorts",
    "connectToSerialPort",
    "disconnectSerialPort",
    "selectSound",
    "index",
    "fileDropped",
    "receiveSerialData",
    "sendSerialData",
    "playSound",
    "soundEnd",
    "saveConfig",
    "loadConfig",
    "saveInitData",
    "loadInitData",
    "showFromTray",
    "exitApp",
    "startupConfigSelected",
    "populateAudioDevices",
    "updateOutput1VolumeLabel",
    "updateOutput2VolumeLabel",
    "resetOutput1Volume",
    "resetOutput2Volume",
    "combo1Changed",
    "combo2Changed",
    "openStartupHelp"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10SoundboardE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  164,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,  167,    2, 0x08,    3 /* Private */,
       4,    1,  168,    2, 0x08,    4 /* Private */,
       5,    2,  171,    2, 0x08,    6 /* Private */,
       6,    1,  176,    2, 0x08,    9 /* Private */,
       8,    2,  179,    2, 0x08,   11 /* Private */,
       9,    0,  184,    2, 0x08,   14 /* Private */,
      10,    1,  185,    2, 0x08,   15 /* Private */,
      11,    1,  188,    2, 0x08,   17 /* Private */,
      12,    1,  191,    2, 0x08,   19 /* Private */,
      13,    1,  194,    2, 0x08,   21 /* Private */,
      14,    1,  197,    2, 0x08,   23 /* Private */,
      15,    0,  200,    2, 0x08,   25 /* Private */,
      16,    0,  201,    2, 0x08,   26 /* Private */,
      17,    0,  202,    2, 0x08,   27 /* Private */,
      18,    0,  203,    2, 0x08,   28 /* Private */,
      19,    1,  204,    2, 0x08,   29 /* Private */,
      20,    0,  207,    2, 0x08,   31 /* Private */,
      21,    1,  208,    2, 0x08,   32 /* Private */,
      22,    1,  211,    2, 0x08,   34 /* Private */,
      23,    0,  214,    2, 0x08,   36 /* Private */,
      24,    0,  215,    2, 0x08,   37 /* Private */,
      25,    1,  216,    2, 0x08,   38 /* Private */,
      26,    1,  219,    2, 0x08,   40 /* Private */,
      27,    0,  222,    2, 0x08,   42 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,    2,    2,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Soundboard::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN10SoundboardE.offsetsAndSizes,
    qt_meta_data_ZN10SoundboardE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10SoundboardE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Soundboard, std::true_type>,
        // method 'sendSerial'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'refreshSerialPorts'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connectToSerialPort'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'disconnectSerialPort'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'selectSound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'fileDropped'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'receiveSerialData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendSerialData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'playSound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'soundEnd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'saveConfig'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'loadConfig'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'saveInitData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadInitData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showFromTray'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exitApp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startupConfigSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'populateAudioDevices'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateOutput1VolumeLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateOutput2VolumeLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'resetOutput1Volume'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resetOutput2Volume'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'combo1Changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'combo2Changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'openStartupHelp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Soundboard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Soundboard *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sendSerial((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->refreshSerialPorts(); break;
        case 2: _t->connectToSerialPort((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->disconnectSerialPort((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 4: _t->selectSound((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->fileDropped((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 6: _t->receiveSerialData(); break;
        case 7: _t->sendSerialData((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->playSound((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->soundEnd((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->saveConfig((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 11: _t->loadConfig((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 12: _t->saveInitData(); break;
        case 13: _t->loadInitData(); break;
        case 14: _t->showFromTray(); break;
        case 15: _t->exitApp(); break;
        case 16: _t->startupConfigSelected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: _t->populateAudioDevices(); break;
        case 18: _t->updateOutput1VolumeLabel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->updateOutput2VolumeLabel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->resetOutput1Volume(); break;
        case 21: _t->resetOutput2Volume(); break;
        case 22: _t->combo1Changed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 23: _t->combo2Changed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->openStartupHelp(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (Soundboard::*)(QString );
            if (_q_method_type _q_method = &Soundboard::sendSerial; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *Soundboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Soundboard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10SoundboardE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Soundboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void Soundboard::sendSerial(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
