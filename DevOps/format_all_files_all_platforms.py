import os
import platform
import subprocess

def run_windows_script():
    # Path to the PowerShell script
    ps_script_path = "format_all_files.ps1"
    
    # Run the PowerShell script
    subprocess.run(["powershell", "-ExecutionPolicy", "Bypass", "-File", ps_script_path], check=True)

def run_mac_script():
    # Path to the shell script on macOS (example)
    sh_script_path = "format_all_files.sh"
    
    # Run the shell script
    subprocess.run(["bash", sh_script_path], check=True)

def main():
    # Check the operating system
    current_os = platform.system()

    if current_os == "Windows":
        print("Running PowerShell script on Windows...")
        run_windows_script()
    elif current_os == "Darwin":  # macOS
        print("Running shell script on macOS...")
        run_mac_script()
    else:
        print(f"Unsupported OS: {current_os}")

if __name__ == "__main__":
    main()
