#!/bin/bash

rootFolder="$(pwd)"
buildDir="$rootFolder/build/macos"

git -C $rootFolder submodule init
git -C $rootFolder submodule update

if [ ! -d "$buildDir" ]; then
    mkdir -p "$buildDir"
fi

cd "$buildDir" || exit

cmake -S "$rootFolder" -B . -G "Xcode"
