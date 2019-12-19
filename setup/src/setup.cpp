#include "pch.h"

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>
#include <windows.h>

#include "registry.h"

int main() {

    /*
     * TODO:
     *  1) Read original game directory from registry
     *  2) Extract sounds
     *  3) Extract images
     *  4) Build textures
     *  5) Copy videos
     */

    // Read original game directory from registry
    try {
        std::wstring gameDir = Registry::RegGetString(
                HKEY_CURRENT_USER,
                L"Software\\Titus Games\\Rival Realms",
                L"Game Directory");
        std::wcout << gameDir << "\n";
    } catch (const Registry::RegistryError& e) {
        std::cout << e.errorCode() << "\n";
    }

}
