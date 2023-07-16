'''
Script to create a blank .cpp and .h file for Open-Rival.

    USAGE:
      python new_file.py [dir] filename

    EXAMPLE:
      python new_file.py Rect
      python new_file.py net/packets NewPacket
'''

import re
import sys

SRC_DIR = '../projects/Open-Rival/src/'
HDR_DIR = '../projects/Open-Rival/include/'

CPP_TEMPLATE = '''\
#include "{filename}.h"

namespace Rival {{

    // TODO

}}  // namespace Rival
'''

HDR_TEMPLATE = '''\
#pragma once

namespace Rival {

    // TODO

}  // namespace Rival
'''

def camelCaseToSnakeCase(name):
    return re.sub(r'(?<!^)(?=[A-Z])', '_', name).upper()

def main():
    # Check arguments
    if len(sys.argv) < 2:
        print('No filename provided!')
        sys.exit(1)
    if len(sys.argv) > 3:
        print('Too many arguments!')
        sys.exit(1)

    path = ''
    filename = ''

    if len(sys.argv) == 2:
        filename = sys.argv[1]
    if len(sys.argv) == 3:
        path = sys.argv[1]
        filename = sys.argv[2]

    # Write .cpp file
    context = {
        'filename': path + '/' + filename
    }
    cpp_filename = SRC_DIR + path + '/' + filename + '.cpp'
    with open(cpp_filename, 'w') as myfile:
        myfile.write(CPP_TEMPLATE.format(**context))
        print(f'Created {cpp_filename}')

    # Write .h file
    hdr_filename = HDR_DIR + path + '/' + filename + '.h'
    with open(hdr_filename, 'w') as myfile:
        myfile.write(HDR_TEMPLATE)
        print(f'Created {hdr_filename}')

if __name__ == '__main__':
    main()
