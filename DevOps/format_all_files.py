import os
import subprocess
import platform

base_dir = "fuego" 
clang_format_cmd = "clang-format"
exclusions=[
    "External", 
    "build", 
    ]

def extract_relative_path(full_path):
    base_dir_pos = full_path.lower().find(base_dir.lower())
    if base_dir_pos != -1:
        return full_path[base_dir_pos + len(base_dir):]
    return full_path

def apply_clang_format_to_dir(directory):
    print(f"Applying clang-format to directory: {directory}")

    code_files = []
    for root, _, files in os.walk(directory):
        if any(exclusion in root for exclusion in exclusions):
            continue

        for file in files:
            if file.endswith(('.cpp', '.h', '.mm', '.m')):
                full_path = os.path.join(root, file)
                if not any(exclusion in full_path for exclusion in exclusions):
                    code_files.append(full_path)

    print(f"\nFound Code files: {len(code_files)}")

    for file_path in code_files:
        print(f"Applying clang-format to: {extract_relative_path(file_path)}")
        subprocess.run([clang_format_cmd, "-i", "--style=file", file_path], check=True)

current_dir = os.getcwd()
root_dir = os.path.abspath(os.path.join(current_dir, ".."))

engine_dir = os.path.join(root_dir, "Engine")
sandbox_dir = os.path.join(root_dir, "Sandbox")

print(f"\nStep [1\\2]")
print(f"Engine directory: {engine_dir}:")

apply_clang_format_to_dir(engine_dir)

print(f"\nStep [2\\2]")
print(f"Sandbox directory: {sandbox_dir}")

apply_clang_format_to_dir(sandbox_dir)

print("\nclang-format applied to all matching files, excluding External, build directories, and clang-format files.")
