@echo off
REM This script converts all MP3 files in the current directory to WAV format.
REM Ensure ffmpeg is installed and available in the PATH.

for %%f in (*.mp3) do (
    echo Converting "%%f" to "%%~nf.wav"...
    ffmpeg -y -i "%%f" "%%~nf.wav"
)

echo.
echo All conversions complete.
pause
