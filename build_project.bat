@echo off
set PATH=C:\msys64\mingw64\bin;%PATH%
cd c:\Users\TonyS\LaneSense\highway-kalman-filter

echo Compiling UKF Highway Project...
echo Using system Eigen from MSYS2
echo.

REM Use system Eigen from MSYS2 (no need to include local src/Eigen)
g++ -std=c++17 -Wall -Isrc -obuild/ukf_highway.exe src/main.cpp src/ukf.cpp src/tools.cpp 2>&1

echo.
echo Compile exit code: %ERRORLEVEL%

if %ERRORLEVEL% EQU 0 (
    echo.
    echo SUCCESS: build/ukf_highway.exe created!
    dir build\ukf_highway.exe
) else (
    echo.
    echo FAILED: Compilation failed
)

