$currentDir = Get-Location
Set-Location -Path $PSScriptRoot
$platform = "windows"
python .\generate_project.py $platform
Set-Location -Path $currentDir
Write-Host "Original directory: $currentDir"
