$ErrorActionPreference = "Continue"
$exePath = "c:\Users\TonyS\LaneSense\highway-kalman-filter\build\ukf_highway.exe"
$outputFile = "c:\Users\TonyS\LaneSense\highway-kalman-filter\build\run_output.txt"

$psi = New-Object System.Diagnostics.ProcessStartInfo
$psi.FileName = $exePath
$psi.UseShellExecute = $false
$psi.RedirectStandardOutput = $true
$psi.RedirectStandardError = $true
$psi.CreateNoWindow = $true

$process = New-Object System.Diagnostics.Process
$process.StartInfo = $psi
$process.Start() | Out-Null

$stdout = $process.StandardOutput.ReadToEnd()
$stderr = $process.StandardError.ReadToEnd()
$process.WaitForExit()

$output = "STDOUT:`n$stdout`n`nSTDERR:`n$stderr`n`nExit Code: $($process.ExitCode)"
$output | Out-File -FilePath $outputFile -Encoding utf8

Write-Host $output


