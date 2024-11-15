import os
import sys
import subprocess
import platform
import shutil
from pathlib import Path


def create_venv(venv_path):
    result = subprocess.run(['python', '-m', 'venv', venv_path])

    if result.returncode == 0:
        print(f'Successfully created venv at {venv_path}')
    else:
        print(f'Creating venv at {venv_path} failed with return code {result.returncode}')
        sys.exit(1)

def install_clang_format(venv_path):
    if platform.system() == 'Windows':
        pip_path = os.path.join(venv_path, 'Scripts', 'pip.exe')
    else:
        pip_path = os.path.join(venv_path, 'bin', 'pip')

    result = subprocess.run([pip_path, 'install', 'clang-format'])

    if result.returncode == 0:
        print(f'Successfully installed clang-format to venv')
    else:
        print(f'Installing clang-format to venv failed with return code {result.returncode}')
        sys.exit(1)

def copy_git_hooks(source_dir, dest_dir):
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)

    for root, dirs, files in os.walk(source_dir):
        for file in files:
            source_file = os.path.join(root, file)
            relative_path = os.path.relpath(source_file, source_dir)
            dest_file = os.path.join(dest_dir, relative_path)
            
            os.makedirs(os.path.dirname(dest_file), exist_ok=True)
            
            shutil.copy2(source_file, dest_file)
            print(f'Successfully copied githooks')

def main():
    root_path = Path(os.path.abspath(__file__)).parent.parent
    venv_path = os.path.join(root_path, 'venv')
    create_venv(venv_path)
    install_clang_format(venv_path)

    dest_dir = os.path.join(root_path, '.git', 'hooks')
    src_dir = os.path.join(root_path, 'Scripts', 'GitHooks')
    copy_git_hooks(src_dir, dest_dir)


if __name__ == '__main__':
    main()
