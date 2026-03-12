# Build script for highway-kalman-filter
$ErrorActionPreference = "Stop"

$projectDir = "C:\Users\TonyS\LaneSense\highway-kalman-filter"
$buildDir = "$projectDir\build"

Write-Host "Starting build process..."

# Navigate to project directory
Set-Location $projectDir

# Remove and create build directory
if (Test-Path $buildDir) {
    Write-Host "Removing old build directory..."
    Remove-Item -Recurse -Force $buildDir
}

Write-Host "Creating new build directory..."
New-Item -ItemType Directory -Force -Path $buildDir | Out-Null
Set-Location $buildDir

# Find cmake
$cmakePaths = @(
    "C:\Program Files\CMake\bin\cmake.exe",
    "C:\Program Files (x86)\CMake\bin\cmake.exe",
    "cmake"
)

$cmakeCmd = $null
foreach ($path in $cmakePaths) {
    if (Test-Path $path) {
        $cmakeCmd = $path
        break
    }
}

if (-not $cmakeCmd) {
    $cmakeCmd = Get-Command cmake -ErrorAction SilentlyContinue
    if ($cmakeCmd) {
        $cmakeCmd = $cmakeCmd.Source
    }
}

if (-not $cmakeCmd) {
    Write-Host "ERROR: CMake not found. Please install CMake." -ForegroundColor Red
    exit 1
}

Write-Host "Using CMake: $cmakeCmd"

# Run cmake
Write-Host "Running CMake..."
& $cmakeCmd -G "MinGW Makefiles" ..

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake failed with exit code $LASTEXITCODE" -ForegroundColor Red
    exit 1
}

# Find make
$makePaths = @(
    "C:\msys64\mingw64\bin\mingw32-make.exe",
    "C:\msys64\usr\bin\make.exe",
    "mingw32-make",
    "make"
)

$makeCmd = $null
foreach ($path in $makePaths) {
    if (Test-Path $path) {
        $makeCmd = $path
        break
    }
}

if (-not $makeCmd) {
    $makeCmd = Get-Command mingw32-make -ErrorAction SilentlyContinue
    if ($makeCmd) {
        $makeCmd = $makeCmd.Source
    }
}

if (-not $makeCmd) {
    Write-Host "ERROR: make not found. Please install MinGW-w64." -ForegroundColor Red
    exit 1
}

Write-Host "Using Make: $makeCmd"

# Run make
Write-Host "Running make..."
& $makeCmd

if ($LASTEXITCODE -ne 0) {
    Write-Host "make failed with exit code $LASTEXITCODE" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "Build successful!" -ForegroundColor Green
Write-Host ""
Write-Host "To run the program, run: ./ukf_highway" -ForegroundColor Cyan
