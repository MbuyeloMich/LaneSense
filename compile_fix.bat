@echo off
setlocal

set SRC_DIR=c:\Users\TonyS\LaneSense\highway-kalman-filter
set BUILD_DIR=%SRC_DIR%\build
set GPP="C:\msys64\mingw64\bin\g++.exe"

echo Compiling UKF Highway...
echo.

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

%GPP% -std=c++17 -Wall -I"%SRC_DIR%\src" -I"%SRC_DIR%\src\Eigen" -o"%BUILD_DIR%\ukf_highway.exe" "%SRC_DIR%\src\main.cpp" "%SRC_DIR%\src\ukf.cpp" "%SRC_DIR%\src\tools.cpp"

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilation successful!
    echo Output: %BUILD_DIR%\ukf_highway.exe
) else (
    echo.
    echo Compilation failed with error code: %ERRORLEVEL%
)

endlocal

