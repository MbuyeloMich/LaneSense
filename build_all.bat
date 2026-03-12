@echo off
cd /d c:\Users\TonyS\LaneSense\highway-kalman-filter

REM Compile main.cpp
"C:\Users\TonyS\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\c++.exe" -std=c++17 -Wall -I"src" -I"src/Eigen" -c src/main.cpp -o build/main.o

REM Compile ukf.cpp
"C:\Users\TonyS\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\c++.exe" -std=c++17 -Wall -I"src" -I"src/Eigen" -c src/ukf.cpp -o build/ukf.o

REM Compile tools.cpp
"C:\Users\TonyS\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\c++.exe" -std=c++17 -Wall -I"src" -I"src/Eigen" -c src/tools.cpp -o build/tools.o

REM Link the object files
"C:\Users\TonyS\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\c++.exe" -o build/ukf_highway.exe build/main.o build/ukf.o build/tools.o

