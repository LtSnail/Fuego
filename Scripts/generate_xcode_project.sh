#!/bin/bash

current_dir=$(pwd)
cd "$(dirname "$0")" || exit
platform="macos"
python3 ./generate_project.py $platform
cd "$current_dir"