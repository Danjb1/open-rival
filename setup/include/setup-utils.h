#ifndef SETUP_UTILS_H
#define SETUP_UTILS_H

#include <stdint.h>
#include <string>

bool createDirectory(const char* filename);

void* read_file(std::wstring filename, uint32_t* size);

int nextPowerOf2(int v);

#endif  // SETUP_UTILS_H
