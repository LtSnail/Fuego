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

    if platform == 'macos':
        build_dir = build_dir / 'macos'
        generator = 'Xcode'
    elif platform == 'windows':
        build_dir = build_dir / 'win'
        generator = 'Visual Studio 17 2022'
    else:
        print(f"Unsupported platform: {platform}")
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

    if os.path.exists(abseil_root):
        print(f"Creating build folder for Abseil: {abseil_build}")
        if not os.path.exists(abseil_build):
            os.makedirs(abseil_build)
            os.makedirs(abseil_installed)

        os.chdir(abseil_root) 

        print(f"Creating solution for Abseil in: {abseil_build}")

        run_command(f'cmake -B abseil_build -G "{generator}" -DCMAKE_INSTALL_PREFIX="{abseil_installed}" -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug -DBUILD_SHARED_LIBS=ON -D_ITERATOR_DEBUG_LEVEL=0 -DABSL_RUN_TESTS=OFF -DBUILD_TESTING=OFF -DCMAKE_CXX_STANDARD=20')
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

    os.chdir(protobuf_root)
    os.system(f'cmake -B "{protobuf_build}" -G"{generator}" -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_SKIP_INSTALL_RPATH=ON -DENABLE_WPO=off -Dprotobuf_WITH_ZLIB=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug -DBUILD_SHARED_LIBS=ON -D_ITERATOR_DEBUG_LEVEL=0 -DABSL_RUN_TESTS=OFF -DBUILD_TESTING=OFF -Dprotobuf_ABSL_PROVIDER=package -DCMAKE_CXX_STANDARD=20 -DCMAKE_INSTALL_PREFIX="{protobuf_installed}" -DCMAKE_PREFIX_PATH="{abseil_installed}"')
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


if __name__ == '__main__':
    main()
