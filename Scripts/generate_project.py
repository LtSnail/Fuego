#!/usr/bin/env python3
from __future__ import annotations

import subprocess
import sys
from pathlib import Path
import os

build_log           = "\n[-- BUILD LOG --]"
build_log_error     = "\n[-- BUILD LOG ERROR --] --> "
protoc_compiler_log = "\n[-- PROTOC COMPILER LOG --]"
cxx_language_version = "20"
platfrorm_var = "none"
platform_macro_definition = "none"
protoc_compiler_name = "none"
protoc_language = "none"

def main():
    if len(sys.argv) < 1:
        print(f"{build_log_error} Needs to provide at least 1 argument: <platform>, usage: generate_project.py <platform> <enable_tests> <build_dll>")
        sys.exit(1)

    platform = sys.argv[1].lower()
    enable_test = "false"
    build_dll = "true"
    if len(sys.argv) >= 2:
        enable_test = sys.argv[2].lower()
    if len(sys.argv) >= 3:
        build_dll = sys.argv[3].lower()   
    generate_project(platform, enable_test, build_dll)


def generate_project(platform, enable_test, build_dll):
    """Generate a project based on the specified platform."""
    root_folder = Path(__file__).parent.parent.resolve()
    build_dir = os.path.join(root_folder, "build")


    if platform == 'macos':
        build_dir = os.path.join(build_dir, "macos")
        platfrorm_var = "macos"
        generator = 'Xcode'
        platform_macro_definition = "-DFUEGO_PLATFORM_MACOS=1"
    elif platform == 'windows':
        build_dir = os.path.join(build_dir, "win")
        platfrorm_var = "win"
        generator = 'Visual Studio 17 2022'
        platform_macro_definition = "-DFUEGO_PLATFORM_WIN=1"
    else:
        print(f"{build_log_error} Unsupported platform: {platform}")
        sys.exit(1)
    if enable_test == "true":
        enable_test = "-DENABLE_FUEGO_TEST=ON"
        print("Tests are On")
    elif enable_test == "false":
        print("Tests are Off")
        enable_test = "-DENABLE_FUEGO_TEST=OFF"
    else: 
        print(f"{build_log_error} Unsupported argument for tests: {enable_test}")
        sys.exit(1)

    if build_dll == "true":
        print("Fuego engine is DLL")
        build_dll = "-DFUEGO_LIB_TYPE=SHARED"
    elif build_dll == "false":
        print("Fuego engine is static")
        build_dll = "-DFUEGO_LIB_TYPE=STATIC"
    else: 
        print(f"{build_log_error} Unsupported argument for build_dll: {build_dll}")
        sys.exit(1)

    if os.path.exists(build_dir):
        os.makedirs(build_dir, exist_ok=True)

    print('Initializing and updating git submodules...')
    print(f"Root folder: {root_folder}")

    run_command(f"git -C {root_folder} submodule init")
    run_command(f"git -C {root_folder} submodule update")

#Engine    
    print(f"{build_log} Engine Project generation...")

    engine_arguments = (f' {enable_test}'
                        f' -DFUEGO_PLATFORM={platfrorm_var}'
                        f' {platform_macro_definition}'
                        f' {build_dll}')
    
    run_command(f'cmake -S "{root_folder}" -B "{build_dir}" -G "{generator}" {engine_arguments}')
    print(f"{build_log} Project generation for {platform} completed successfully.")
# End Engine


    
def run_command(command):
    """Run a shell command and handle errors."""
    try:
        subprocess.run(command, shell=True, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Command failed with exit code {e.returncode}: {e.cmd}")
        sys.exit(e.returncode)

def get_cmake_available_generators():
    raw_available_list = subprocess.run(["cmake", "--help"], capture_output=True, text=True).stdout
    lines = raw_available_list.splitlines()

    generators = []
    parsing = False

    for line in lines:

        if not parsing:
            if line.startswith("Generators"):
                parsing = True
            continue

        if not line or line.startswith("=") or line.startswith("   "):
            continue
        if line.startswith("* "):
            line = line.lstrip("* ")

        generator_name = line.split("=", 1)[0].strip()
        generators.append(generator_name)

    return generators

def find_matching_generator(preferred_generators, available_cmake_generators):
    generator = None
    for pref_gen in preferred_generators:
        found = False
        for available_gen in available_cmake_generators:
            if pref_gen.lower() == available_gen.lower():
                generator = available_gen
                found = True
                break
        if found:
            break
    return generator

if __name__ == '__main__':
    main()
