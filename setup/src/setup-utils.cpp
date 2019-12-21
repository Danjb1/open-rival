#include "pch.h"
#include "setup-utils.h"

#include <windows.h>

/**
 * Attempts to create the given directory.
 */
bool create_directory(const char* filename) {
    return CreateDirectoryA(filename, NULL)
        || ERROR_ALREADY_EXISTS == GetLastError();
}

/**
 * Rounds a number up to the nearest power of 2.
 *
 * See:
 * http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
 */
int nextPowerOf2(int v) {

    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}
