@echo off
cd /d c:\Users\TonyS\LaneSense\highway-kalman-filter
"C:\Users\TonyS\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\c++.exe" -std=c++17 -Wall -I"src" -I"src/Eigen" -c src/main.cpp -o build/main.o

