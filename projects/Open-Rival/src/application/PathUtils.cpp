#include "application/PathUtils.h"

#include <shlobj_core.h>

#include <sstream>

#include "utils/LogUtils.h"
#include "utils/StringUtils.h"

namespace Rival { namespace PathUtils {

static std::vector<std::string> defaultFontDirs;

std::string getLocalAppData()
{
    wchar_t* localAppDataRaw = 0;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localAppDataRaw);

    if (result != S_OK)
    {
        LOG_WARN("Get local app data failed: {}", result);
        return "";
    }

    std::wstring localAppData = std::wstring(localAppDataRaw);
    CoTaskMemFree(static_cast<void*>(localAppDataRaw));

    return StringUtils::toUtf8(localAppData) + "\\";
}

void initDefaultFontDirs()
{
    std::string localAppData = getLocalAppData();
    defaultFontDirs = {
        "res\\fonts\\",                               //
        "C:\\Windows\\Fonts\\",                       //
        localAppData + "Microsoft\\Windows\\Fonts\\"  //
    };
}

std::vector<std::string> getDefaultFontDirs()
{
    if (defaultFontDirs.size() == 0)
    {
        initDefaultFontDirs();
    }
    return defaultFontDirs;
}

}}  // namespace Rival::PathUtils
