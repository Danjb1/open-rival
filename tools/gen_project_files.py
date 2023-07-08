"""
Script to automatically add all .cpp and .h files under the Open-Rival folder to
our CMake build.

Rebuild the ZERO_CHECK project after running this script to regenerate the
project files.

    USAGE:
      python gen_project_files.py
"""

import os
import re

CMAKE_DIR = "../cmake"
OPEN_RIVAL_DIR = "../projects/Open-Rival"
INCLUDE_DIR = f"{OPEN_RIVAL_DIR}/include"
SRC_DIR = f"{OPEN_RIVAL_DIR}/src"

output_src = []
output_include_headers = []

# Headers
for dirpath, dirs, files in os.walk(INCLUDE_DIR):
    for file in files:
        relative_filename = (
            os.path.join(dirpath, file)
            .replace(OPEN_RIVAL_DIR, "")
            .replace("\\", "/")
        )
        entry = f"${{OPEN_RIVAL_DIR}}{relative_filename}"
        output_include_headers.append(entry)

# Source files
for dirpath, dirs, files in os.walk(SRC_DIR):
    for file in files:
        relative_filename = (
            os.path.join(dirpath, file)
            .replace(OPEN_RIVAL_DIR, "")
            .replace("\\", "/")
        )
        entry = f"${{OPEN_RIVAL_DIR}}{relative_filename}"
        output_src.append(entry)

# Sort lists alphabetically    
output_src.sort()
output_include_headers.sort()

# Write to CMake file
cmake_file = f"{CMAKE_DIR}/OpenRivalFiles.cmake"
with open(cmake_file, 'w') as myfile:
    myfile.write("set(OPEN_RIVAL_SOURCES\n")
    for file in output_src:
        myfile.write(f"    {file}\n")
    myfile.write(")\n\n")

    myfile.write("set(OPEN_RIVAL_HEADERS\n")
    for file in output_include_headers:
        myfile.write(f"    {file}\n")
    myfile.write(")\n")
