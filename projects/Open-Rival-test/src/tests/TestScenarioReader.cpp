#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "scenario/ScenarioReader.h"

using namespace Rival;

namespace TestScenarioReader {

using Catch::Matchers::ContainsSubstring;

/** Error reported when trying to read beyond the end of the scenario data. */
const std::string endOfDataError = "Unexpected end of scenario data";

/** Writes the given bytes to a temporary file, which is deleted when this goes out of scope. */
class TempFile
{
public:
    TempFile(const std::string& name, const std::vector<std::uint8_t>& bytes)
        : path(std::filesystem::temp_directory_path() / name)
    {
        std::ofstream out(path, std::ios::binary);
        out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    }

    ~TempFile()
    {
        std::error_code ignored;
        std::filesystem::remove(path, ignored);
    }

    std::string getPath() const
    {
        return path.string();
    }

private:
    std::filesystem::path path;
};

SCENARIO("ScenarioReader rejects invalid scenario data", "[scenario-reader]")
{
    GIVEN("An empty scenario file")
    {
        TempFile file("open-rival-test-empty.sco", {});
        ScenarioReader reader(file.getPath());

        THEN("reading the scenario fails with an error")
        {
            REQUIRE_THROWS_WITH(reader.readScenario(), ContainsSubstring(endOfDataError));
        }
    }

    GIVEN("A truncated scenario file")
    {
        TempFile file("open-rival-test-truncated.sco", std::vector<std::uint8_t>(16, 0));
        ScenarioReader reader(file.getPath());

        THEN("reading the scenario fails with an error")
        {
            REQUIRE_THROWS_WITH(reader.readScenario(), ContainsSubstring(endOfDataError));
        }
    }

    GIVEN("A campaign file whose scenario offset is beyond the end of the file")
    {
        // 1 level, at offset 0xFFFFFFF0, with size 0
        TempFile file("open-rival-test-campaign.cmp", { 0x01, 0xF0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 });
        ScenarioReader reader(file.getPath());

        THEN("reading the scenario fails with an error")
        {
            REQUIRE_THROWS_WITH(reader.readCampaignScenario(0), ContainsSubstring(endOfDataError));
        }
    }
}

}  // namespace TestScenarioReader
