/**
 * Simple one-off script to convert hardcoded unit data to json.
 */
int main() {
    using namespace Rival::Animations;

    json j;

    for (uint8_t unitType = 0; unitType < Unit::lastUnitType; ++unitType) {

        json thisUnit;
        thisUnit["name"] = "unit_" + std::to_string(unitType);
        thisUnit["portrait"] = 0;
        thisUnit["sounds"]["train"] = json::array({ 0 });
        thisUnit["sounds"]["select"] = json::array({ 0 });
        thisUnit["sounds"]["move"] = json::array({ 0 });

        for (uint8_t animType = 0; animType < 6; ++animType) {
            std::pair<Unit::Type, UnitAnimationType> key = { static_cast<Unit::Type>(unitType),
                static_cast<UnitAnimationType>(animType) };
            if (Animations::unitAnimationLookup.find(key)
                    != Animations::unitAnimationLookup.end()) {
                const Animation animData = Animations::unitAnimationLookup.at(key);
                thisUnit["animations"]["animType_" + std::to_string(animType)]["startIndex"] = animData.startIndex;
                thisUnit["animations"]["animType_" + std::to_string(animType)]["endIndex"] = animData.endIndex;
                thisUnit["animations"]["animType_" + std::to_string(animType)]["msPerFrame"] = animData.msPerFrame;
            }
        }

        j["units"].push_back(thisUnit);
    }

    std::cout << j.dump(2) << std::endl;
}
