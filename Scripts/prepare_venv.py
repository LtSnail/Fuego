#!/usr/bin/env python3
from __future__ import annotations

import os
import platform
import subprocess
import sys
from pathlib import Path


def get_python_command():
    """Get the correct Python command based on platform."""
    if platform.system() == 'Windows':
        return 'python'
    return 'python3'


def create_venv(venv_path):
    """Create a virtual environment."""
    python_cmd = get_python_command()
    result = subprocess.run([python_cmd, '-m', 'venv', venv_path])

    if result.returncode == 0:
        print(f'Successfully created venv at {venv_path}')
    else:
        print(
            f'Creating venv at {venv_path} failed with return code '
            f'{result.returncode}',
        )
        sys.exit(1)


def install_pip_module(venv_path, module_name):
    """Install a Python module using pip."""
    if platform.system() == 'Windows':
        pip_path = os.path.join(venv_path, 'Scripts', 'pip.exe')
    else:
        pip_path = os.path.join(venv_path, 'bin', 'pip')

    if not os.path.exists(pip_path):
        raise RuntimeError(
            'pip executable not found in virtual environment. '
            'Virtual environment creation may have failed.',
        )

    result = subprocess.run([pip_path, 'install', module_name])

    if result.returncode == 0:
        print(f'Successfully installed {module_name} to venv')
    else:
        print(
            f'Installing {module_name} to venv failed with return code '
            f'{result.returncode}',
        )
        sys.exit(1)


def main(venv_name):
    """Main function to set up the virtual environment."""
    root_path = Path(os.path.abspath(__file__)).parent.parent
    venv_path = os.path.join(root_path, venv_name)

    # Remove existing venv if it exists
    if os.path.exists(venv_path):
        print(f'Removing existing venv at {venv_path}')
        if platform.system() == 'Windows':
            subprocess.run(['rmdir', '/s', '/q', venv_path], shell=True)
        else:
            subprocess.run(['rm', '-rf', venv_path])

    create_venv(venv_path)
    install_pip_module(venv_path, 'pre-commit')


if __name__ == '__main__':
    venv_name = ''
    if len(sys.argv) <= 1:
        venv_name = 'venv'
        print(
            'No arguments were provided for the script. '
            'Using default venv name.',
        )
    else:
        venv_name = sys.argv[1]
        print(f'Using venv name: {venv_name}.')
    main(venv_name)
