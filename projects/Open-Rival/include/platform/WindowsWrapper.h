#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

// These are useless and intefere with our identifiers
#undef near
#undef far

#endif
