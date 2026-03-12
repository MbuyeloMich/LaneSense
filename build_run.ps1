# Build script for UKF Highway project
$projectDir = "C:\Users\TonyS\LaneSense\highway-kalman-filter"
$buildDir = "$projectDir\build"

Set-Location $projectDir

# Remove old build and create new one
if (Test-Path $buildDir) {
    Remove-Item -Recurse -Force $buildDir
}
New-Item -ItemType Directory -Path $buildDir | Out-Null
Set-Location $buildDir

# Set compiler paths
$env:CXX = "C:\msys64\mingw64\bin\g++.exe"
$env:CC = "C:\msys64\mingw64\bin\gcc.exe"

# Run cmake with make program specified
& "C:\Program Files\CMake\bin\cmake.exe" -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="C:\msys64\mingw64\bin\g++.exe" -DCMAKE_C_COMPILER="C:\msys64\mingw64\bin\gcc.exe" -DCMAKE_MAKE_PROGRAM="C:\msys64\mingw64\bin\mingw32-make.exe" ..

# Run make
& "C:\msys64\mingw64\bin\mingw32-make.exe"

Write-Host ""
Write-Host "Build completed!"

