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

def camelCaseToSnakeCase(name):
    return re.sub(r'(?<!^)(?=[A-Z])', '_', name).upper()

def main():
    # Check arguments
    if len(sys.argv) < 2:
        print('No filename provided!')
        return
    if len(sys.argv) > 3:
        print('Too many arguments!')
        return

    path = ''
    filename = ''

    if len(sys.argv) == 2:
        filename = sys.argv[1]
    if len(sys.argv) == 3:
        path = sys.argv[1]
        filename = sys.argv[2]

    # Write .cpp file
    template = '''\
#include "pch.h"
#include "{filename}.h"

namespace Rival {{

    // TODO

}}  // namespace Rival
'''
    context = {
        'filename': path + '/' + filename
    }
    with open(SRC_DIR + path + '/' + filename + '.cpp', 'w') as myfile:
        myfile.write(template.format(**context))

    # Write .h file
    template = '''\
#pragma once

namespace Rival {

    // TODO

}  // namespace Rival
'''
    with open(HDR_DIR + path + '/' + filename + '.h', 'w') as myfile:
        myfile.write(template)

if __name__ == '__main__':
    main()
