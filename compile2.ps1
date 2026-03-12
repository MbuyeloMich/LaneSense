$gpp = "C:\msys64\mingw64\bin\g++.exe"
$srcDir = "C:\Users\TonyS\LaneSense\highway-kalman-filter\src"
$buildDir = "C:\Users\TonyS\LaneSense\highway-kalman-filter\build"

$inc1 = "-I" + $srcDir
$inc2 = "-I" + $srcDir + "\Eigen"
$out = "-o" + $buildDir + "\ukf_highway.exe"

$args = @("-std=c++17", "-Wall", "-v", $inc1, $inc2, $out, "$srcDir\main.cpp", "$srcDir\ukf.cpp", "$srcDir\tools.cpp")

& $gpp @args
Write-Host "Exit code: $LASTEXITCODE"

