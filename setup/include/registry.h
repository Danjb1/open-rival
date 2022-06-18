#pragma once

#include <IntSafe.h>
#include <windows.h>

#include <stdexcept>

/*
 * Based on:
 * https://docs.microsoft.com/en-us/archive/msdn-magazine/2017/may/c-use-modern-c-to-access-the-windows-registry
 */

namespace Registry {

class RegistryError : public std::runtime_error
{
public:
    RegistryError(const char* message, LONG errorCode);

    LONG errorCode() const noexcept;

private:
    LONG m_errorCode;
};

/**
 * Gets a 32-bit unsigned integer from the registry.
 *
 * Example:
 *
 *   try {
 *       DWORD data = RegGetDword(HKEY_CURRENT_USER, subkey, L"MyDwordValue");
 *   } catch (const RegistryError& e) {
 *       std::cout << e.errorCode() << "\n";
 *   }
 */
DWORD RegGetDword(HKEY hKey, const std::wstring& subKey, const std::wstring& value);

/**
 * Gets a string from the registry.
 *
 * Example:
 *
 *   try {
 *       wstring s = RegGetString(HKEY_CURRENT_USER, subkey, L"MyStringValue");
 *   } catch (const RegistryError& e) {
 *       std::cout << e.errorCode() << "\n";
 *   }
 */
std::wstring RegGetString(HKEY hKey, const std::wstring& subKey, const std::wstring& value);

}  // namespace Registry
