@echo off
setlocal EnableDelayedExpansion

set "GPP=C:\msys64\mingw64\bin\g++.exe"
set "SRC=C:\Users\TonyS\LaneSense\highway-kalman-filter\src"
set "BUILD=C:\Users\TonyS\LaneSense\highway-kalman-filter\build"

call "%GPP%" -std=c++17 -Wall -I"%SRC%" -I"%SRC%\Eigen" -o"%BUILD%\ukf_highway.exe" "%SRC%\main.cpp" "%SRC%\ukf.cpp" "%SRC%\tools.cpp"

echo Exit code: %ERRORLEVEL%

