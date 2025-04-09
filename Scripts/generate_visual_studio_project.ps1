$currentDir = Get-Location
Set-Location -Path $PSScriptRoot
$platform = "windows"
$enable_tests = "true"
$build_dll = "true"
python .\generate_project.py $platform $enable_tests $build_dll
Set-Location -Path $currentDir
Write-Host "Original directory: $currentDir"
