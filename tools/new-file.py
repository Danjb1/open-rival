'''
Script to create a blank .cpp and .h file for Open-Rival.

    USAGE:
      python new-file.py [filename]

    EXAMPLE:
      python new-file.py Rect
'''

import re
import sys

SRC_DIR = '../Open-Rival/src/'
HDR_DIR = '../Open-Rival/include/'

def camelCaseToSnakeCase(name):
    return re.sub(r'(?<!^)(?=[A-Z])', '_', name).upper()

def main():

    # Check arguments
    if len(sys.argv) < 2:
        print('No filename provided!')
        return

    filename = sys.argv[1]

    # Write .cpp file
    template = '''\
#include "pch.h"
#include "{filename}.h"

namespace Rival {{

    // TODO

}}  // namespace Rival
'''
    context = {
        'filename': filename
    }
    with open(SRC_DIR + filename + '.cpp', 'w') as myfile:
        myfile.write(template.format(**context))

    # Write .h file
    template = '''\
#ifndef {filename}_H
#define {filename}_H

namespace Rival {{

    // TODO

}}  // namespace Rival

#endif  // {filename}_H
'''
    context = {
        'filename': camelCaseToSnakeCase(filename)
    }
    with open(HDR_DIR + filename + '.h', 'w') as myfile:
        myfile.write(template.format(**context))

if __name__ == '__main__':
    main()
