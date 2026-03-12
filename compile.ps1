$GPP = "C:\msys64\mingw64\bin\g++.exe"
$SRC_DIR = "c:\Users\TonyS\LaneSense\highway-kalman-filter"
$BUILD_DIR = "$SRC_DIR\build"

# Create build directory if it doesn't exist
if (-not (Test-Path $BUILD_DIR)) {
    New-Item -ItemType Directory -Path $BUILD_DIR | Out-Null
}

$OutputFile = "$BUILD_DIR\ukf_highway.exe"
$SourceFiles = @(
    "$SRC_DIR\src\main.cpp",
    "$SRC_DIR\src\ukf.cpp",
    "$SRC_DIR\src\tools.cpp"
)

$IncludePaths = @(
    "$SRC_DIR\src",
    "$SRC_DIR\src\Eigen"
)

$IncludeArgs = $IncludePaths | ForEach-Object { "-I""$_""" }
$CompilerArgs = @(
    "-std=c++17",
    "-Wall"
) + $IncludeArgs + @(
    "-o""$OutputFile""",
    $SourceFiles
)

Write-Host "Compiling UKF Highway..." -ForegroundColor Cyan
Write-Host "Compiler: $GPP"
Write-Host "Output: $OutputFile"
Write-Host ""

$ErrorLog = "$BUILD_DIR\compile_errors.txt"

& $GPP $CompilerArgs 2>&1 | Tee-Object -Variable compileOutput

$compileOutput | Out-File -FilePath $ErrorLog -Encoding utf8

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "Compilation successful!" -ForegroundColor Green
    Write-Host "Output: $OutputFile"
} else {
    Write-Host ""
    Write-Host "Compilation failed with error code: $LASTEXITCODE" -ForegroundColor Red
    Write-Host ""
    Write-Host "Error log:" -ForegroundColor Yellow
    Get-Content $ErrorLog | Select-Object -First 50
}

