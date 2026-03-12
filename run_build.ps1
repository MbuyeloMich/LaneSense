# Build script for UKF Highway project
Set-Location "C:\Users\TonyS\LaneSense\highway-kalman-filter"

# Remove old build and create new one
if (Test-Path build) {
    Remove-Item -Recurse -Force build
}
New-Item -ItemType Directory -Path build | Out-Null
Set-Location build

# Run cmake
& "C:\Program Files\CMake\bin\cmake.exe" -G "MinGW Makefiles" ..

# Run make
& mingw32-make
