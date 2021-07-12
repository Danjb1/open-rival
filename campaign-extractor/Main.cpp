#include "pch.h"

#include "campaign-extractor.h"

using namespace Rival::Setup;

int main() {
    extractCampaign("CELF.CMP");
    extractCampaign("CGRE.CMP");
    extractCampaign("CHUMAN.CMP");
    extractCampaign("INTRO.CMP");
    extractCampaign("SORR.BLD");
}
