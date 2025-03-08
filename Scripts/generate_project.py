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

def main():
    if len(sys.argv) != 3:
        print(f"{build_log_error} Arguments more\\less than 3, usage: generate_project.py <platform> <BEnableTests>")
        sys.exit(1)

    platform = sys.argv[1].lower()
    enable_test = sys.argv[2].lower()
    generate_project(platform, enable_test)


def generate_project(platform, enable_test):
    """Generate a project based on the specified platform."""
    root_folder = Path(__file__).parent.parent.resolve()
    build_dir = os.path.join(root_folder, "build")

    if platform == 'macos':
        build_dir = os.path.join(build_dir, "macos")
        platfrorm_var = "macos"
        generator = 'Xcode'
    elif platform == 'windows':
        build_dir = os.path.join(build_dir, "win")
        platfrorm_var = "win"
        generator = 'Visual Studio 17 2022'
    else:
        print(f"{build_log_error} Unsupported platform: {platform}")
        sys.exit(1)
    if enable_test == "true":
        enable_test = "-DENABLE_FUEGO_TEST=ON"
    elif enable_test == "false":
        enable_test = "-DENABLE_FUEGO_TEST=OFF"
    else: 
        print(f"{build_log_error} Unsupported argument for tests: {enable_test}")
        sys.exit(1)

    if os.path.exists(build_dir):
        os.makedirs(build_dir, exist_ok=True)

    print('Initializing and updating git submodules...')
    print(f"Root folder: {root_folder}")

    run_command(f"git -C {root_folder} submodule init")
    run_command(f"git -C {root_folder} submodule update")

#Abseil
    print(f"{build_log} Preparing for Abseil")
    #abseil_cxx_flags_debug = "/bigobj /MTd /EHsc"
    abseil_root                     = os.path.join(root_folder, "Engine", "External", "abseil")
    abseil_build_debug              = os.path.join(abseil_root, "abseil_build_debug")
    abseil_installed_debug          = os.path.join(abseil_root, "abseil_installed_debug")
    abseil_installed_debug_cmake    = os.path.join(abseil_installed_debug, "lib", "cmake", "absl")

    abseil_build_release            = os.path.join(abseil_root, "abseil_build_release")
    abseil_installed_release        = os.path.join(abseil_root, "abseil_installed_release")
    abseil_installed_release_cmake  = os.path.join(abseil_installed_release, "lib", "cmake", "absl")

    abseil_debug_build_arguments = ( ' -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebugDLL'
                                    f' -DCMAKE_INSTALL_PREFIX="{abseil_installed_debug}"')

    abseil_release_build_arguments  =   (' -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL'
                                        f' -DCMAKE_INSTALL_PREFIX="{abseil_installed_release}"')

    abseil_common_build_arguments   = ( ' -DBUILD_SHARED_LIBS=OFF'
                                        ' -D_ITERATOR_DEBUG_LEVEL=0'
                                        ' -DABSL_RUN_TESTS=OFF'
                                        ' -DBUILD_TESTING=OFF'
                                        f' -DCMAKE_CXX_STANDARD={cxx_language_version}'
                                        ' -DABSL_MSVC_STATIC_RUNTIME=OFF'
                                        f' -DABSL_BUILD_TESTING=OFF'
                                        f' -DABSL_BUILD_TEST_HELPERS=OFF'
                                        f' -DABSL_USE_EXTERNAL_GOOGLETEST=OFF'
                                        f' -DABSL_USE_GOOGLETEST_HEAD=OFF'
                                        f' -DABSL_BUILD_MONOLITHIC_SHARED_LIBS=ON')

    print(f"{build_log} Abseil build arguments:")
    print(f"{build_log} Common arguments: {abseil_common_build_arguments}")
    print(f"{build_log} Debug arguments: {abseil_debug_build_arguments}")
    print(f"{build_log} Release arguments: {abseil_release_build_arguments}")

    if os.path.exists(abseil_root):
        if not os.path.exists(abseil_build_debug):
            print(f"{build_log} Creating debug build folder for Abseil: {abseil_build_debug}")
            os.makedirs(abseil_build_debug)
        if not os.path.exists(abseil_build_release):
            print(f"{build_log} Creating release build folder for Abseil: {abseil_build_release}")
            os.makedirs(abseil_build_release)
        if not os.path.exists(abseil_installed_debug):
            print(f"{build_log} Creating debug installed folder for Abseil: {abseil_installed_debug}")
            os.makedirs(abseil_installed_debug)
        if not os.path.exists(abseil_installed_release):
            print(f"{build_log} Creating release installed folder for Abseil: {abseil_installed_release}")
            os.makedirs(abseil_installed_release)

        os.chdir(abseil_root) 
        # Debug
        print(f"{build_log} Creating debug solution for Abseil in: {abseil_build_debug}")
        run_command(f'cmake -B {abseil_build_debug} -G "{generator}" {abseil_common_build_arguments} {abseil_debug_build_arguments}')

        print(f"{build_log} Building debug Abseil from: {abseil_build_debug} to: {abseil_installed_debug}")
        run_command(f'cmake --build {abseil_build_debug} --config Debug --target install --parallel 16')

        print(f"{build_log} Debug Abseil installed successfully to: {abseil_installed_debug}")

        os.chdir(abseil_root) 
        # Release
        print(f"{build_log} Creating release solution for Abseil in: {abseil_build_release}")
        run_command(f'cmake -B {abseil_build_release} -G "{generator}" {abseil_common_build_arguments} {abseil_release_build_arguments}')

        print(f"{build_log} Building release Abseil from: {abseil_build_release} to: {abseil_installed_release}")
        run_command(f'cmake --build {abseil_build_release} --config Release --target install --parallel 16')

        print(f"{build_log} Release Abseil installed successfully to: {abseil_installed_release}")
#end Abseil

#Protobuf
    print(f"{build_log} Preparing for Protobuf")
    #protobuf_cxx_flags = "/bigobj /MTd /EHsc"
    protobuf_root               = os.path.join(root_folder, "Engine", "External", "protobuf")

    protobuf_build_debug        = os.path.join(protobuf_root, "protobuf_build_debug")
    protobuf_installed_debug    = os.path.join(protobuf_root, "protobuf_installed_debug")
    protobuf_installed_debug_cmake  = os.path.join(protobuf_installed_debug, "lib", "cmake", "protobuf")

    protobuf_build_release      = os.path.join(protobuf_root, "protobuf_build_release")
    protobuf_installed_release      = os.path.join(protobuf_root, "protobuf_installed_release")
    protobuf_installed_release_cmake  = os.path.join(protobuf_installed_release, "lib", "cmake", "protobuf")

    utf8_installed_debug_cmake = os.path.join(protobuf_installed_debug, "lib", "cmake", "utf8_range")
    utf8_installed_release_cmake = os.path.join(protobuf_installed_release, "lib", "cmake", "utf8_range")

    protobuf_debug_build_arguments =   ( ' -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebugDLL'
                                        f' -Dabsl_DIR="{abseil_installed_debug_cmake}"'
                                        f' -DCMAKE_INSTALL_PREFIX="{protobuf_installed_debug}"'
                                        f' -Dprotobuf_BUILD_LIBPROTOC=OFF'
                                        f' -Dprotobuf_BUILD_PROTOC_BINARIES=OFF'
                                        f' -Dprotobuf_BUILD_PROTOBUF_BINARIES=ON'
                                        f' -Dprotobuf_BUILD_EXAMPLES=OFF')

    protobuf_release_build_arguments = ( ' -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL'
                                        f' -Dabsl_DIR="{abseil_installed_release_cmake}"'
                                        f' -DCMAKE_INSTALL_PREFIX="{protobuf_installed_release}"'
                                        f' -Dprotobuf_BUILD_LIBPROTOC=ON'
                                        f' -Dprotobuf_BUILD_PROTOC_BINARIES=ON'
                                        f' -Dprotobuf_BUILD_PROTOBUF_BINARIES=ON'
                                        f' -Dprotobuf_BUILD_EXAMPLES=OFF')

    protobuf_common_build_arguments =  (f'-Dprotobuf_BUILD_TESTS=OFF'
                                        ' -DCMAKE_SKIP_INSTALL_RPATH=ON'
                                        ' -DENABLE_WPO=OFF'
                                        ' -Dprotobuf_WITH_ZLIB=OFF'
                                        ' -DBUILD_SHARED_LIBS=OFF'
                                        ' -D_ITERATOR_DEBUG_LEVEL=0'
                                        ' -DABSL_RUN_TESTS=OFF'
                                        ' -DBUILD_TESTING=OFF'
                                        ' -Dprotobuf_ABSL_PROVIDER=package'
                                        f' -DCMAKE_CXX_STANDARD={cxx_language_version}'
                                        f' -Dprotobuf_BUILD_LIBUPB=ON'
                                        f' -Dprotobuf_BUILD_CONFORMANCE=OFF'
                                        f' -Dprotobuf_INSTALL=ON'
                                        f' -Dprotobuf_MSVC_STATIC_RUNTIME=OFF')
    
    print(f"{build_log} Protobuf build arguments:")
    print(f"{build_log} Common arguments: {protobuf_common_build_arguments}")
    print(f"{build_log} Debug arguments: {protobuf_debug_build_arguments}")
    print(f"{build_log} Release arguments: {protobuf_release_build_arguments}")

    if os.path.exists(protobuf_root):
        if not os.path.exists(protobuf_build_debug):
            print(f"{build_log} Creating debug build folder for Protobuf: {protobuf_build_debug}")
            os.makedirs(protobuf_build_debug)
        if not os.path.exists(protobuf_build_release):
            print(f"{build_log} Creating release build folder for Protobuf: {protobuf_build_release}")
            os.makedirs(protobuf_build_release)
        if not os.path.exists(protobuf_installed_debug):
            print(f"{build_log} Creating debug installed folder for Protobuf: {protobuf_installed_debug}")
            os.makedirs(protobuf_installed_debug)
        if not os.path.exists(protobuf_installed_release):
            print(f"{build_log} Creating release installed folder for Protobuf: {protobuf_installed_release}")
            os.makedirs(protobuf_installed_release)

    os.chdir(protobuf_root)
    # Debug
    print(f"{build_log} Creating debug solution for Protobuf in: {protobuf_build_debug}")
    run_command(f'cmake -B {protobuf_build_debug} -G "{generator}" {protobuf_common_build_arguments} {protobuf_debug_build_arguments}')

    print(f"{build_log} Building debug Protobuf from: {protobuf_build_debug} to: {protobuf_installed_debug}")
    run_command(f'cmake --build {protobuf_build_debug} --config Debug --target install --parallel 16')

    print(f"{build_log} Debug Protobuf installed successfully to: {protobuf_installed_debug}")

    os.chdir(protobuf_root) 
    # Release
    print(f"{build_log} Creating release solution for Protobuf in: {protobuf_build_release}")
    run_command(f'cmake -B {protobuf_build_release} -G "{generator}" {protobuf_common_build_arguments} {protobuf_release_build_arguments}')

    print(f"{build_log} Building release Protobuf from: {protobuf_build_release} to: {protobuf_installed_release}")
    run_command(f'cmake --build {protobuf_build_release} --config Release --target install --parallel 16')

    print(f"{build_log} Release Protobuf installed successfully to: {protobuf_installed_release}")
# end protobuf

# Compiling of proto files using protoc compiler
    protoc_path = os.path.join(protobuf_installed_release, "bin")
    proto_input = os.path.join(root_folder, "Engine", "Fuego", "ProtoIn")
    proto_output = os.path.join(root_folder, "Engine", "Fuego", "ProtoOut")

    if not os.path.exists(proto_input):
        os.makedirs(proto_input)
    if not os.path.exists(proto_output):
        os.makedirs(proto_output)

    os.chdir(protoc_path)

    for dirpath, dirnames, filenames in os.walk(proto_input):
        for filename in filenames:
            if filename.endswith('.proto'):
                if not dirnames:
                    dirnames = "Root Folder"
                print(f"{protoc_compiler_log} Proto file was found: {filename} in: {dirnames}")
                run_command(f'.\\protoc.exe --proto_path="{proto_input}" --cpp_out="{proto_output}" {filename}')
    

# End protoc

#Engine    
    print(f"{build_log} Engine Project generation...")
    
    engine_arguments = (f' -DABSEIL_INSTALLED_DEBUG="{abseil_installed_debug_cmake}"'
                        f' -DABSEIL_INSTALLED_RELEASE="{abseil_installed_release_cmake}"'
                        f' -DPROTOBUF_INSTALLED_DEBUG="{protobuf_installed_debug_cmake}"'
                        f' -DPROTOBUF_INSTALLED_RELEASE="{protobuf_installed_release_cmake}"'
                        f' -DUTF8_INSTALLED_DEBUG="{utf8_installed_debug_cmake}"'
                        f' -DUTF8_INSTALLED_RELEASE="{utf8_installed_release_cmake}"'
                        f' -DPROTO_PATH="{proto_output}"'
                        f' {enable_test}'
                        f' -DPLATFORM={platfrorm_var}')
    
    run_command(f'cmake -S "{root_folder}" -B "{build_dir}" -G "{generator}" {engine_arguments}')
    run_command(f'cmake --build {build_dir} --target install --parallel 16 --verbose')

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
