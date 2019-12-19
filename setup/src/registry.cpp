#include "pch.h"
#include "registry.h"

namespace Registry {

    RegistryError::RegistryError(const char* message, LONG errorCode) :
        std::runtime_error{ message },
        m_errorCode{ errorCode } {}

    LONG RegistryError::errorCode() const noexcept {
        return m_errorCode;
    }

    DWORD RegGetDword(
        HKEY hKey,
        const std::wstring& subKey,
        const std::wstring& value) {

        DWORD data{};
        DWORD dataSize = sizeof(data);
        LONG retCode = ::RegGetValue(
            hKey,
            subKey.c_str(),
            value.c_str(),
            RRF_RT_REG_DWORD,
            nullptr,
            &data,
            &dataSize
        );

        if (retCode != ERROR_SUCCESS) {
            throw RegistryError{ "Cannot read DWORD from registry.", retCode };
        }

        return data;
    }


    std::wstring RegGetString(
        HKEY hKey,
        const std::wstring& subKey,
        const std::wstring& value) {

        // Determine the size of the string
        DWORD dataSize{};
        LONG retCode = ::RegGetValue(
            hKey,
            subKey.c_str(),
            value.c_str(),
            RRF_RT_REG_SZ,
            nullptr,
            nullptr,
            &dataSize
        );

        if (retCode != ERROR_SUCCESS) {
            throw RegistryError{ "Cannot read string from registry", retCode };
        }

        // Create a buffer to hold the string
        std::wstring data;
        data.resize(dataSize / sizeof(wchar_t));

        // Retrieve the string
        retCode = ::RegGetValue(
            hKey,
            subKey.c_str(),
            value.c_str(),
            RRF_RT_REG_SZ,
            nullptr,
            &data[0],
            &dataSize
        );

        if (retCode != ERROR_SUCCESS) {
            throw RegistryError{ "Cannot read string from registry", retCode };
        }

        // Convert size from bytes to wide (Unicode) character count
        DWORD stringLengthInWchars = dataSize / sizeof(wchar_t);

        // Fix double NUL-terminator
        stringLengthInWchars--; // Exclude the NUL written by the Win32 API
        data.resize(stringLengthInWchars);

        return data;
    }

}
