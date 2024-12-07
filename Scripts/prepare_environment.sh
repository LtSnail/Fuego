#!/bin/bash

script_root=$(dirname "$0")
root_path=$(realpath "$script_root/..")
prepare_venv_path="$script_root/prepare_venv.py"
venv_name="venv"

echo "Preparing the virtual environment..."
python3 "$prepare_venv_path" "$venv_name"
if [ $? -ne 0 ]; then
    exit $?
fi

echo "Activating the virtual environment..."
activate_script="$root_path/$venv_name/bin/activate"

if [ -f "$activate_script" ]; then
    source "$activate_script"
    echo "Virtual environment activated."
else
    echo "Could not find the virtual environment activation script."
    exit 1
fi

echo "Running pre-commit..."
pre-commit install -f
if [ $? -ne 0 ]; then
    echo "Pre-commit installation failed."
    exit $?
else
    echo "Pre-commit installation completed successfully."
fi

pre-commit autoupdate
if [ $? -eq 0 ]; then
    echo "Pre-commit autoupdate completed successfully."
else
    echo "Pre-commit autoupdate failed."
    exit $?
fi
