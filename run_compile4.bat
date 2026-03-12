@echo off
set PATH=C:\msys64\mingw64\bin;%PATH%
cd c:\Users\TonyS\LaneSense\highway-kalman-filter
g++ test.cpp -o test.exe 2>&1
echo Compile error: %ERRORLEVEL%
dir test.exe

