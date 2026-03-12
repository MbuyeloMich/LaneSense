@echo off
setlocal EnableDelayedExpansion

set "GPP=C:\msys64\mingw64\bin\g++.exe"
set "SRC=C:\Users\TonyS\LaneSense\highway-kalman-filter\src"
set "BUILD=C:\Users\TonyS\LaneSense\highway-kalman-filter\build"

echo Starting compilation...
echo GPP: %GPP%
echo SRC: %SRC%
echo BUILD: %BUILD%
echo.

"%GPP%" -std=c++17 -Wall -I"%SRC%" -I"%SRC%\Eigen" -o"%BUILD%\ukf_highway.exe" "%SRC%\main.cpp" "%SRC%\ukf.cpp" "%SRC%\tools.cpp" > "%BUILD%\output.log" 2>&1

echo Exit code: %ERRORLEVEL% >> "%BUILD%\output.log"
echo Compilation complete.

endlocal

