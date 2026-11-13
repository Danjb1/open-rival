"""
Script to automatically add all .cpp and .h files under the Open-Rival folder to
our CMake build.

This rewrites the block between the BEGIN / END markers in the Open-Rival
CMakeLists.txt file.

Rebuild the ZERO_CHECK project after running this script to regenerate the
project files.

    USAGE:
      python gen_project_files.py
"""

import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
OPEN_RIVAL_DIR = os.path.join(SCRIPT_DIR, "..", "projects", "Open-Rival")
CMAKE_FILE = os.path.join(OPEN_RIVAL_DIR, "CMakeLists.txt")

BEGIN_MARKER = "# BEGIN GENERATED FILE LIST"
END_MARKER = "# END GENERATED FILE LIST"


def find_files(subdir):
    """Finds all files under the given subdirectory, relative to the Open-Rival folder."""
    results = []
    for dirpath, dirs, files in os.walk(os.path.join(OPEN_RIVAL_DIR, subdir)):
        for file in files:
            path = os.path.relpath(os.path.join(dirpath, file), OPEN_RIVAL_DIR)
            results.append(path.replace("\\", "/"))
    return sorted(results)


def make_file_list(var_name, files):
    lines = [f"set({var_name}"]
    lines += [f"    {file}" for file in files]
    lines.append(")")
    return lines


# Build the generated block
generated = [BEGIN_MARKER + " (see tools/gen_project_files.py)"]
generated += make_file_list("OPEN_RIVAL_SOURCES", find_files("src"))
generated.append("")
generated += make_file_list("OPEN_RIVAL_HEADERS", find_files("include"))
generated.append(END_MARKER)

# Replace the existing block in the CMake file
with open(CMAKE_FILE, "r", newline="") as f:
    content = f.read()

newline = "\r\n" if "\r\n" in content else "\n"
lines = content.split(newline)

begin = next(i for i, line in enumerate(lines) if line.startswith(BEGIN_MARKER))
end = next(i for i, line in enumerate(lines) if line.startswith(END_MARKER))
lines[begin : end + 1] = generated

with open(CMAKE_FILE, "w", newline="") as f:
    f.write(newline.join(lines))
