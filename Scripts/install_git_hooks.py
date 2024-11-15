import os
import shutil

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

            print(f"\nFile copied: {relative_path}\nFrom {source_dir} to {dest_dir}")

source_dir = "GitHooks"
dest_dir = "../.git/hooks"

copy_git_hooks(source_dir, dest_dir)

input("\nCopying of GitHooks has completed. Press Enter to exit.")
