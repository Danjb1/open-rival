#include "pch.h"
#include "setup-utils.h"

#include <stdio.h>
#include <windows.h>

/**
 * Attempts to create the given directory.
 */
bool createDirectory(const char* filename) {
    return CreateDirectoryA(filename, NULL)
            || ERROR_ALREADY_EXISTS == GetLastError();
}

void* read_file(std::wstring filename, uint32_t* size) {

    FILE* fp = _wfopen(filename.c_str(), L"rb");
    if (!fp) {
        return NULL;
    }

    fseek(fp, 0L, SEEK_END);
    *size = (uint32_t) ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    void* data = malloc(*size);
    if (!data) {
        fclose(fp);
        return NULL;
    }

    fread(data, *size, 1, fp);

    if (ferror(fp)) {
        free(data);
        data = NULL;
    }
    fclose(fp);

    return data;
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
