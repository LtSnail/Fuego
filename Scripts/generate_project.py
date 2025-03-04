#!/usr/bin/env python3
from __future__ import annotations

import subprocess
import sys
from pathlib import Path
import os

build_log           = "[-- BUILD LOG --]"
build_log_error     = "[-- BUILD LOG ERROR --] --> "

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
    build_dir = os.path.join(root_folder, "build")

    if platform == 'macos':
        build_dir = os.path.join(build_dir, "macos")
        generator = 'Xcode'
    elif platform == 'windows':
        build_dir = os.path.join(build_dir, "win")
        generator = 'Visual Studio 17 2022'
    else:
        print(f"{build_log_error} Unsupported platform: {platform}")
        sys.exit(1)

    print('Initializing and updating git submodules...')
    print(f"Root folder: {root_folder}")

    run_command(f"git -C {root_folder} submodule init")
    run_command(f"git -C {root_folder} submodule update")

#Abseil
    build_log = "Log "
    #print(f"{build_log} Generating Abseil for {platform}...")
    #abseil_cxx_flags_debug = "/bigobj /MTd /EHsc"
    abseil_root = os.path.join(root_folder, "Engine", "External", "abseil")
    abseil_build = os.path.join(abseil_root, "abseil_build")
    abseil_installed = os.path.join(abseil_root, "abseil_installed")

    abseil_build_arguments = (  f'-DCMAKE_INSTALL_PREFIX="{abseil_installed}" '
                                ' -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug '
                                ' -DBUILD_SHARED_LIBS=OFF '
                                ' -D_ITERATOR_DEBUG_LEVEL=0 '
                                ' -DABSL_RUN_TESTS=OFF '
                                ' -DBUILD_TESTING=OFF '
                                f' -DCMAKE_CXX_STANDARD=20'
                                ' -DABSL_MSVC_STATIC_RUNTIME=ON'
                                f' -DABSL_BUILD_TESTING=OFF'
                                f' -DABSL_BUILD_TEST_HELPERS=OFF'
                                f' -DABSL_USE_EXTERNAL_GOOGLETEST=OFF'
                                f' -DABSL_USE_GOOGLETEST_HEAD=OFF'
                                f' -DABSL_BUILD_MONOLITHIC_SHARED_LIBS=ON')
    
    print(f"{build_log} Abseil build arguments: {abseil_build_arguments}")

    if os.path.exists(abseil_root):
        if not os.path.exists(abseil_build):
            print(f"{build_log} Creating build folder for Abseil: {abseil_build}")
            os.makedirs(abseil_build)
        if not os.path.exists(abseil_installed):
            print(f"{build_log} Creating installed folder for Abseil: {abseil_installed}")
            os.makedirs(abseil_installed)

        os.chdir(abseil_root) 

        print(f"{build_log} Creating solution for Abseil in: {abseil_build}")
        run_command(f'cmake -B abseil_build -G "{generator}" {abseil_build_arguments}')
        print(f"{build_log} Building Abseil from: {abseil_build} to: {abseil_installed}")
        run_command(f'cmake --build abseil_build --target install --parallel 16')
        print(f"{build_log} Abseil installed successfully to: {abseil_installed}")
#end abseil

#Protobuf
    print(f"{build_log} Generating Protobuf project")
    #protobuf_cxx_flags = "/bigobj /MTd /EHsc"
    protobuf_root = os.path.join(root_folder, "Engine", "External", "protobuf")
    protobuf_build = os.path.join(protobuf_root, "protobuf_build")
    protobuf_installed = os.path.join(protobuf_root, "protobuf_installed")

    protobuf_build_arguments = (  f'-Dprotobuf_BUILD_TESTS=OFF'
                                    ' -DCMAKE_SKIP_INSTALL_RPATH=ON'
                                    ' -DENABLE_WPO=OFF'
                                    ' -Dprotobuf_WITH_ZLIB=OFF'
                                    ' -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug'
                                    ' -DBUILD_SHARED_LIBS=OFF'
                                    ' -D_ITERATOR_DEBUG_LEVEL=0'
                                    ' -DABSL_RUN_TESTS=OFF'
                                    ' -DBUILD_TESTING=OFF'
                                    ' -Dprotobuf_ABSL_PROVIDER=package'
                                    f' -DCMAKE_CXX_STANDARD=20'
                                    f' -DCMAKE_INSTALL_PREFIX="{protobuf_installed}"'
                                    f' -DCMAKE_PREFIX_PATH="{abseil_installed}"'
                                    f' -Dprotobuf_BUILD_LIBUPB=ON'
                                    f' -Dprotobuf_BUILD_LIBPROTOC=ON'
                                    f' -Dprotobuf_BUILD_PROTOC_BINARIES=ON'
                                    f' -Dprotobuf_BUILD_PROTOBUF_BINARIES=ON'
                                    f' -Dprotobuf_BUILD_EXAMPLES=ON'
                                    f' -Dprotobuf_BUILD_CONFORMANCE=OFF'
                                    f' -Dprotobuf_INSTALL=ON'
                                    f' -Dprotobuf_MSVC_STATIC_RUNTIME=ON')
    
    print(f"{build_log} Protobuf build arguments: {protobuf_build_arguments}")
    if os.path.exists(protobuf_root):
        if not os.path.exists(protobuf_build):
            print(f"{build_log} Creating build folder for Protobuf: {protobuf_build}")
            os.makedirs(protobuf_build)
        if not os.path.exists(protobuf_installed):
            print(f"{build_log} Creating installed folder for Protobuf: {protobuf_installed}")
            os.makedirs(protobuf_installed)

    os.chdir(protobuf_root)
    print(f"{build_log} Creating solution for Protobuf in: {protobuf_build}")
    os.system(f'cmake -B "{protobuf_build}" -G"{generator}" {protobuf_build_arguments}')
    print(f"{build_log} Building Protobuf from: {protobuf_build} to: {protobuf_installed}")
    os.system(f'cmake --build protobuf_build --target install --parallel 16 --verbose')
    print(f"{build_log} Protobuf installed successfully to: {protobuf_installed}")
# end protobuf
    if not os.path.exists(build_dir):
        print(f"Creating build directory: {build_dir}")
        os.makedirs(build_dir)

    engine_build_arguments = (  f'-Dprotobuf_MSVC_STATIC_RUNTIME=ON'
                                f' -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug')
    
    print(f"Generating project for {platform}...")
    run_command(f'cmake -S "{root_folder}" -B "{build_dir}" -G "{generator}" {engine_build_arguments}')
    run_command(f'cmake --build {build_dir} --target install --parallel 16 --verbose')
    print(f"Project generation for {platform} completed successfully.")

    if os.path.exists(abseil_root):
        if not os.path.exists(abseil_build):
            print(f"{build_log} Creating build folder for Abseil: {abseil_build}")
            os.makedirs(abseil_build)
        if not os.path.exists(abseil_installed):
            print(f"{build_log} Creating installed folder for Abseil: {abseil_installed}")
            os.makedirs(abseil_installed)

        os.chdir(abseil_root) 

        print(f"{build_log} Creating solution for Abseil in: {abseil_build}")
        run_command(f'cmake -B abseil_build -G "{generator}" {abseil_build_arguments}')
        print(f"{build_log} Building Abseil from: {abseil_build} to: {abseil_installed}")
        run_command(f'cmake --build abseil_build --target install --parallel 4')
        print(f"{build_log} Abseil installed successfully to: {abseil_installed}")
#end abseil

#Protobuf
    print(f"{build_log} Generating Protobuf project")
    #protobuf_cxx_flags = "/bigobj /MTd /EHsc"
    protobuf_root = os.path.join(root_folder, "Engine", "External", "protobuf")
    protobuf_build = os.path.join(protobuf_root, "protobuf_build")
    protobuf_installed = os.path.join(protobuf_root, "protobuf_installed")

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
    if os.path.exists(protobuf_root):
        if not os.path.exists(protobuf_build):
            print(f"{build_log} Creating build folder for Protobuf: {protobuf_build}")
            os.makedirs(protobuf_build)
        if not os.path.exists(protobuf_installed):
            print(f"{build_log} Creating installed folder for Protobuf: {protobuf_installed}")
            os.makedirs(protobuf_installed)

    os.chdir(protobuf_root)
    print(f"{build_log} Creating solution for Protobuf in: {protobuf_build}")
    os.system(f'cmake -B "{protobuf_build}" -G"{generator}" {protobuf_build_arguments}')
    print(f"{build_log} Building Protobuf from: {protobuf_build} to: {protobuf_installed}")
    os.system(f'cmake --build protobuf_build --target install --parallel 4 --verbose')
    print(f"{build_log} Protobuf installed successfully to: {protobuf_installed}")
# end protobuf

#Engine    
    print(f"{build_log} Engine Project generation...")
    protobuf_libraries_path = os.path.join(protobuf_installed, "lib")
    protobuf_includedirs_path = os.path.join(protobuf_installed, "include")
    protobuf_cmake_path = os.path.join(protobuf_installed, "lib", "cmake", "protobuf")

    engine_build_arguments = () #f'-DProtobuf_LIBRARIES="{protobuf_libraries_path}"'
                               #f' -DProtobuf_INCLUDE_DIR="{protobuf_includedirs_path}"'
                               #f' -DProtobuf_DIR="{protobuf_cmake_path}"')
    
    print(f"{build_log} Engine build arguments: {engine_build_arguments}")

    run_command(f'cmake -S "{root_folder}" -B "{build_dir}" -G "{generator}" {engine_build_arguments}')
    run_command(f'cmake --build {build_dir} --target install --parallel 4 --verbose')
    print(f"{build_log} Project generation for {platform} completed successfully.")
# end engine

def main():
    if len(sys.argv) != 2:
        print(f"{build_log_error} Arguments more\\less than 2, usage: generate_project.py <platform>")
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
