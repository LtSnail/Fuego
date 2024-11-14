$rootFolder = "$PSScriptRoot"
$buildDir = Join-Path -Path $rootFolder -ChildPath "build\win"

git -C $rootFolder submodule init
git -C $rootFolder submodule update

if (-Not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir
}

$currentDir = Get-Location
Set-Location $buildDir

cmake -S $rootFolder -B . -G "Visual Studio 17 2022" 

Set-Location $currentDir