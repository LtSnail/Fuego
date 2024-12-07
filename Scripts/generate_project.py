#!/usr/bin/env python3
from __future__ import annotations

import subprocess
import sys
from pathlib import Path


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

    print(f"Generating project for {platform}...")
    run_command(f'cmake -S "{root_folder}" -B "{build_dir}" -G "{generator}"')
    print(f"Project generation for {platform} completed successfully.")


def main():
    if len(sys.argv) != 2:
        print('Usage: generate_project.py <platform>')
        sys.exit(1)

    platform = sys.argv[1].lower()
    generate_project(platform)


if __name__ == '__main__':
    main()
