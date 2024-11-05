$rootFolder = "$PSScriptRoot"
$buildDir = Join-Path -Path $rootFolder -ChildPath "build\win"

git -C $rootFolder submodule init
git -C $rootFolder submodule update

if (-Not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir
}

Set-Location $buildDir

cmake -S $rootFolder -B . -G "Visual Studio 17 2022" 

# -S .. Source code is located in root folder (relatively build)
# -B . output files will be created in current folder
cmake --build .

Read-Host -Prompt "Press Enter, to close the window"
