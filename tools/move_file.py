'''
Script to move a .cpp and .h file within Open-Rival and update all includes.

    USAGE:
      python move_file.py <old_file> <new_file>

    EXAMPLE:
      python move_file.py Color utils/Color
'''

import os
import sys

SRC_DIR = '../projects/Open-Rival/src/'
HDR_DIR = '../projects/Open-Rival/include/'

def move_file(src, dst):
    print(f'Attempting to move "{src}"')
    try:
        os.renames(src, dst)
    except FileNotFoundError:
        print(f'Source file "{src}" does not exist.')
    except PermissionError:
        print(f'Insufficient permissions to move file "{src}".')
    except OSError as e:
        print(f'An error occurred while moving file "{src}": {e}')

def update_includes_in_file(filepath, old_include, new_include):
    with open(filepath, 'r') as file:
        content = file.read()
    updated_content = content.replace(
        '#include "' + old_include + '.h',
        '#include "' + new_include + '.h')
    if content != updated_content:
        with open(filepath, 'w') as file:
            file.write(updated_content)
        print(f'Updated file: {filepath}')

def update_includes_in_dir(directory, old_include, new_include):
    for root, dirs, files in os.walk(directory):
        for filename in files:
            if not (filename.endswith('.cpp') or filename.endswith('.h')):
                continue
            filepath = os.path.join(root, filename)
            update_includes_in_file(filepath, old_include, new_include)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('Usage: python move_file.py <old_file> <new_file>')
        sys.exit(1)

    old_file = sys.argv[1]
    new_file = sys.argv[2]
    
    # Move .cpp file
    cpp_source = SRC_DIR + old_file + '.cpp'
    cpp_destination = SRC_DIR + new_file + '.cpp'
    move_file(cpp_source, cpp_destination)

    # Move .h file
    hdr_source = HDR_DIR + old_file + '.h'
    hdr_destination = HDR_DIR + new_file + '.h'
    move_file(hdr_source, hdr_destination)

    # Update includes
    update_includes_in_dir(SRC_DIR, old_file, new_file)
    update_includes_in_dir(HDR_DIR, old_file, new_file)
