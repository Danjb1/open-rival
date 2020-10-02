#include "pch.h"
#include "catch2/catch.h"

#include "RenderUtils.h"

using namespace Rival;

TEST_CASE("tileToPx_X calculates the correct tile position", "[render-utils]") {
    REQUIRE(RenderUtils::tileToPx_X(0) == 0);
    REQUIRE(RenderUtils::tileToPx_X(1) == 32);
    REQUIRE(RenderUtils::tileToPx_X(2) == 64);
}

TEST_CASE("tileToPx_Y calculates the correct tile position", "[render-utils]") {
    // First row
    REQUIRE(RenderUtils::tileToPx_Y(0, 0) == 0);
    REQUIRE(RenderUtils::tileToPx_Y(1, 0) == 16);
    REQUIRE(RenderUtils::tileToPx_Y(2, 0) == 0);
    REQUIRE(RenderUtils::tileToPx_Y(3, 0) == 16);

    // Second row
    REQUIRE(RenderUtils::tileToPx_Y(0, 1) == 32);
    REQUIRE(RenderUtils::tileToPx_Y(1, 1) == 48);
    REQUIRE(RenderUtils::tileToPx_Y(2, 1) == 32);
    REQUIRE(RenderUtils::tileToPx_Y(3, 1) == 48);
}

TEST_CASE("tileToScaledPx_X calculates the correct tile position based on the zoom level", "[render-utils]") {
    REQUIRE(RenderUtils::tileToScaledPx_X(0, 2.0f) == 0);
    REQUIRE(RenderUtils::tileToScaledPx_X(1, 2.0f) == 64);
    REQUIRE(RenderUtils::tileToScaledPx_X(2, 2.0f) == 128);
}

TEST_CASE("tileToScaledPx_Y calculates the correct tile position based on the zoom level", "[render-utils]") {
    // First row
    REQUIRE(RenderUtils::tileToScaledPx_Y(0, 0, 2.0f) == 0);
    REQUIRE(RenderUtils::tileToScaledPx_Y(1, 0, 2.0f) == 32);
    REQUIRE(RenderUtils::tileToScaledPx_Y(2, 0, 2.0f) == 0);
    REQUIRE(RenderUtils::tileToScaledPx_Y(3, 0, 2.0f) == 32);

    // Second row
    REQUIRE(RenderUtils::tileToScaledPx_Y(0, 1, 2.0f) == 64);
    REQUIRE(RenderUtils::tileToScaledPx_Y(1, 1, 2.0f) == 96);
    REQUIRE(RenderUtils::tileToScaledPx_Y(2, 1, 2.0f) == 64);
    REQUIRE(RenderUtils::tileToScaledPx_Y(3, 1, 2.0f) == 96);
}

TEST_CASE("cameraToPx_X calculates the correct distance", "[render-utils]") {
    REQUIRE(RenderUtils::cameraToPx_X(0.5f) == 16);
    REQUIRE(RenderUtils::cameraToPx_X(1.0f) == 32);
    REQUIRE(RenderUtils::cameraToPx_X(1.5f) == 48);
}

TEST_CASE("cameraToPx_Y calculates the correct distance", "[render-utils]") {
    REQUIRE(RenderUtils::cameraToPx_Y(0.5f) == 16);
    REQUIRE(RenderUtils::cameraToPx_Y(1.0f) == 32);
    REQUIRE(RenderUtils::cameraToPx_Y(1.5f) == 48);
}

TEST_CASE("pxToCamera_X calculates the correct distance", "[render-utils]") {
    REQUIRE(RenderUtils::pxToCamera_X(16.0f) == 0.5f);
    REQUIRE(RenderUtils::pxToCamera_X(32.0f) == 1.0f);
    REQUIRE(RenderUtils::pxToCamera_X(48.0f) == 1.5f);
}

TEST_CASE("pxToCamera_Y calculates the correct distance", "[render-utils]") {
    REQUIRE(RenderUtils::pxToCamera_Y(16.0f) == 0.5f);
    REQUIRE(RenderUtils::pxToCamera_Y(32.0f) == 1.0f);
    REQUIRE(RenderUtils::pxToCamera_Y(48.0f) == 1.5f);
}

TEST_CASE("getEntityZ calculates the correct z-positions", "[render-utils]") {
    float zStart = RenderUtils::zEntitiesStart;

    // First row
    REQUIRE(RenderUtils::getEntityZ(0, 0) == zStart + 0.0f);
    REQUIRE(RenderUtils::getEntityZ(1, 0) == zStart + 0.5f);
    REQUIRE(RenderUtils::getEntityZ(2, 0) == zStart + 0.0f);
    REQUIRE(RenderUtils::getEntityZ(3, 0) == zStart + 0.5f);

    // Second row
    REQUIRE(RenderUtils::getEntityZ(0, 1) == zStart + 1.0f);
    REQUIRE(RenderUtils::getEntityZ(1, 1) == zStart + 1.5f);
    REQUIRE(RenderUtils::getEntityZ(2, 1) == zStart + 1.0f);
    REQUIRE(RenderUtils::getEntityZ(3, 1) == zStart + 1.5f);
}

TEST_CASE("getCanvasWidth always returns an even number", "[render-utils]") {
    REQUIRE(RenderUtils::getCanvasWidth(10.03124f) == 320);
    REQUIRE(RenderUtils::getCanvasWidth(10.03125f) == 322);
}

TEST_CASE("getCanvasHeight always returns an even number", "[render-utils]") {
    REQUIRE(RenderUtils::getCanvasHeight(10.03124f) == 320);
    REQUIRE(RenderUtils::getCanvasHeight(10.03125f) == 322);
}
