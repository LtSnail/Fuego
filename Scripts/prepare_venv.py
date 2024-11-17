from __future__ import annotations

import os
import platform
import subprocess
import sys
from pathlib import Path


def create_venv(venv_path):
    result = subprocess.run(['python', '-m', 'venv', venv_path])

    if result.returncode == 0:
        print(f'Successfully created venv at {venv_path}')
    else:
        print(
            f'''Creating venv at {venv_path} failed with return code
            {result.returncode}''',
        )
        sys.exit(1)


def install_pip_module(venv_path, module_name):
    if platform.system() == 'Windows':
        pip_path = os.path.join(venv_path, 'Scripts', 'pip.exe')
    else:
        pip_path = os.path.join(venv_path, 'bin', 'pip')

    result = subprocess.run([pip_path, 'install', module_name])

    if result.returncode == 0:
        print(f'Successfully installed {module_name} to venv')
    else:
        print(
            f'''Installing {module_name} to venv failed with return code
            {result.returncode}''',
        )
        sys.exit(1)


def main(venv_name):
    root_path = Path(os.path.abspath(__file__)).parent.parent
    venv_path = os.path.join(root_path, venv_name)
    create_venv(venv_path)
    install_pip_module(venv_path, 'pre-commit')


if __name__ == '__main__':
    venv_name = ''
    if len(sys.argv) <= 1:
        venv_name = 'venv'
        print('''No arguments were provided for the script.
              Using default venv name.''')
    else:
        venv_name = sys.argv[1]
        print(f"Using venv name: {venv_name}.")
    main(venv_name)
