#!/bin/bash

# Apply clang-format to directories on macOS
DIR=$1  # Accept directory as an argument
echo "Applying clang-format to directory: $DIR"

# Find all C++ files and apply clang-format
find $DIR -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.mm" \) \
  ! -path "*/External/*" ! -path "*/build/*" ! -name "clang-format*" \
  -exec clang-format -i --style=file {} \;

echo "clang-format applied to all matching files."
