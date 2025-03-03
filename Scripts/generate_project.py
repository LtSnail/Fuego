#!/usr/bin/env python3
from __future__ import annotations

import subprocess
import sys
from pathlib import Path
import os


def run_command(command):
    """Run a shell command and handle errors."""
    try:
        subprocess.run(command, shell=True, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Command failed with exit code {e.returncode}: {e.cmd}")
        sys.exit(e.returncode)


def generate_project(platform):
    """Generate a project based on the specified platform."""
    root_folder = Path(__file__).parent.parent.resolve()
    build_dir = root_folder / 'build'
    cxx_language_version = "20"

    if platform == 'macos':
        build_dir = build_dir / 'macos'
        generator = 'Xcode'
    elif platform == 'windows':
        build_dir = build_dir / 'win'
        generator = 'Visual Studio 17 2022'
        preferred_generators = ["Visual Studio 17 2022",
                            "Visual Studio 16 2019"]
        available_cmake_generator = get_cmake_available_generators()
        generator = find_matching_generator(preferred_generators, available_cmake_generator)
        if generator:
            print(f"{build_log} Windows generator is: {generator}")
        else:
            print(f"{build_log_error} There is no available Windows generator, please install one of {preferred_generators}, {generator}")
            sys.exit(1)
    else:
        print(f"{build_log_error} Unsupported platform: {platform}")
        sys.exit(1)

    print('Initializing and updating git submodules...')
    run_command(f"git -C {root_folder} submodule init")
    run_command(f"git -C {root_folder} submodule update")

    if not build_dir.exists():
        print(f"Creating build directory: {build_dir}")
        build_dir.mkdir(parents=True)
    
    print(f"Generating Abseil for {platform}...")

#Abseil
    #abseil_cxx_flags_debug = "/bigobj /MTd /EHsc"
    abseil_root = os.path.join(root_folder, "Engine", "External", "abseil")
    abseil_build = os.path.join(abseil_root, "abseil_build")
    abseil_installed = os.path.join(abseil_root, "abseil_installed")

    abseil_build_arguments = (  f'-DCMAKE_INSTALL_PREFIX="{abseil_installed}" '
                                ' -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug '
                                ' -DBUILD_SHARED_LIBS=ON '
                                ' -D_ITERATOR_DEBUG_LEVEL=0 '
                                ' -DABSL_RUN_TESTS=OFF '
                                ' -DBUILD_TESTING=OFF '
                                f' -DCMAKE_CXX_STANDARD={cxx_language_version}')
    
    print(f"{build_log} Abseil build arguments: {abseil_build_arguments}")
    if os.path.exists(abseil_root):
        print(f"Creating build folder for Abseil: {abseil_build}")
        if not os.path.exists(abseil_build):
            os.makedirs(abseil_build)
            os.makedirs(abseil_installed)

        os.chdir(abseil_root) 

        print(f"{build_log} Creating solution for Abseil in: {abseil_build}")
        run_command(f'cmake -B abseil_build -G "{generator}" {abseil_build_arguments}')
        print(f"Creating solution for Abseil in: {abseil_build}")

        print(f"{build_log} Building Abseil from: {abseil_build} to: {abseil_installed}")
        run_command(f'cmake --build abseil_build --target install --parallel 4')

        print(f"abseil_installed to: {abseil_installed}")
#end abseil

#Protobuf
    print(f"Generating Protobuf project")
    #protobuf_cxx_flags = "/bigobj /MTd /EHsc"
    protobuf_root = os.path.join(root_folder, "Engine", "External", "protobuf")
    protobuf_build = os.path.join(protobuf_root, "protobuf_build")
    protobuf_installed = os.path.join(protobuf_root, "protobuf_installed")
    if not os.path.exists(protobuf_build):
        os.makedirs(protobuf_build)
        os.makedirs(protobuf_installed)
    protobuf_build_arguments = (  f'-Dprotobuf_BUILD_TESTS=OFF'
                                    ' -DCMAKE_SKIP_INSTALL_RPATH=ON'
                                    ' -DENABLE_WPO=OFF'
                                    ' -Dprotobuf_WITH_ZLIB=OFF'
                                    ' -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug'
                                    ' -DBUILD_SHARED_LIBS=ON'
                                    ' -D_ITERATOR_DEBUG_LEVEL=0'
                                    ' -DABSL_RUN_TESTS=OFF'
                                    ' -DBUILD_TESTING=OFF'
                                    ' -Dprotobuf_ABSL_PROVIDER=package'
                                    f' -DCMAKE_CXX_STANDARD={cxx_language_version}'
                                    f' -DCMAKE_INSTALL_PREFIX="{protobuf_installed}"'
                                    f' -DCMAKE_PREFIX_PATH="{abseil_installed}"')
    print(f"{build_log} Protobuf build arguments: {protobuf_build_arguments}")
            print(f"{build_log} Creating build folder for Protobuf: {protobuf_build}")
            print(f"{build_log} Creating installed folder for Protobuf: {protobuf_installed}")

    os.chdir(protobuf_root)
    print(f"{build_log} Creating solution for Protobuf in: {protobuf_build}")
    os.system(f'cmake -B "{protobuf_build}" -G"{generator}" {protobuf_build_arguments}')
    print(f"{build_log} Building Protobuf from: {protobuf_build} to: {protobuf_installed}")
    os.system(f'cmake --build protobuf_build --target install --parallel 4 --verbose')
# end protobuf

#Engine    
    print(f"Engine Project generation...")
    run_command(f'cmake -S "{root_folder}" -B "{build_dir}" -G "{generator}"')
    run_command(f'cmake --build {build_dir} --target install --parallel 4 --verbose')
    print(f"Project generation for {platform} completed successfully.")
# end engine

def main():
    if len(sys.argv) != 2:
        print('Usage: generate_project.py <platform>')
        sys.exit(1)

    platform = sys.argv[1].lower()
    generate_project(platform)

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
