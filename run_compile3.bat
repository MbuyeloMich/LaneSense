@echo off
set PATH=C:\msys64\mingw64\bin;%PATH%
cd c:\Users\TonyS\LaneSense\highway-kalman-filter
g++ --version > ver.txt 2>&1
echo Version error: %ERRORLEVEL%
type ver.txt

