# Function to traverse directories with exclusions
function Apply-ClangFormatToDir($dir) {
    Write-Host "Applying clang-format to directory: $dir"

    # Output the path to verify
    Write-Host "The clang-format style file path is: $clangFormatStyleFile"

    # Apply clang-format to C++ files (.cpp, .h, .mm), excluding External, build folders, and clang-format files
    $cppFiles = Get-ChildItem -Path $dir -Recurse -File -Include *.cpp, *.h, *.mm | Where-Object { 
        $filePath = $_.FullName
        $filePath -notmatch '\\External\\' -and $filePath -notmatch '\\build\\' -and $filePath -notmatch '\\clang-format(\.cmake|$)' -and $filePath -notmatch '\\.clang-format\.cmake' # Exclude External, build, and clang-format files, including .clang-format.cmake
    }

    Write-Host "Found C++ files: $($cppFiles.Count)"
    foreach ($file in $cppFiles) {
        Write-Host "Applying clang-format to: $($file.FullName)"
        # Apply clang-format using the standard .clang-format for C++ files
        & clang-format -i --style=file $file.FullName
    }
}

# Get current directory
$currentDir = Get-Location

# Use relative path to move one level up
$rootDir = Resolve-Path "$currentDir\.."

# Apply clang-format to CMake and C++ files in Engine directory
$engineDir = Join-Path -Path $rootDir -ChildPath "Engine"
Write-Host "Engine directory: $engineDir"
Write-Host "Applying clang-format to files in Engine..."
Apply-ClangFormatToDir $engineDir

# Apply clang-format to CMake and C++ files in Sandbox directory
$sandboxDir = Join-Path -Path $rootDir -ChildPath "Sandbox"
Write-Host "Sandbox directory: $sandboxDir"
Write-Host "Applying clang-format to files in Sandbox..."
Apply-ClangFormatToDir $sandboxDir

Write-Host "clang-format applied to all matching files, excluding External, build directories and clang-format files."

# Pause to see the result
Read-Host "Press Enter to exit"
