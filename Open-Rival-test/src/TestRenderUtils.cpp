#include "pch.h"
#include "catch2/catch.h"

#include "RenderUtils.h"

TEST_CASE("tileToPx_X calculates the correct tile position", "[render-utils]") {
    REQUIRE(Rival::RenderUtils::tileToPx_X(0) == 0);
    REQUIRE(Rival::RenderUtils::tileToPx_X(1) == 32);
    REQUIRE(Rival::RenderUtils::tileToPx_X(2) == 64);
}

TEST_CASE("tileToPx_Y calculates the correct tile position", "[render-utils]") {
    // First row
    REQUIRE(Rival::RenderUtils::tileToPx_Y(0, 0) == 0);
    REQUIRE(Rival::RenderUtils::tileToPx_Y(1, 0) == 16);
    REQUIRE(Rival::RenderUtils::tileToPx_Y(2, 0) == 0);
    REQUIRE(Rival::RenderUtils::tileToPx_Y(3, 0) == 16);

    // Second row
    REQUIRE(Rival::RenderUtils::tileToPx_Y(0, 1) == 32);
    REQUIRE(Rival::RenderUtils::tileToPx_Y(1, 1) == 48);
    REQUIRE(Rival::RenderUtils::tileToPx_Y(2, 1) == 32);
    REQUIRE(Rival::RenderUtils::tileToPx_Y(3, 1) == 48);
}

TEST_CASE("tileToScaledPx_X calculates the correct tile position based on the zoom level", "[render-utils]") {
    REQUIRE(Rival::RenderUtils::tileToScaledPx_X(0, 2.0f) == 0);
    REQUIRE(Rival::RenderUtils::tileToScaledPx_X(1, 2.0f) == 64);
    REQUIRE(Rival::RenderUtils::tileToScaledPx_X(2, 2.0f) == 128);
}

TEST_CASE("tileToScaledPx_Y calculates the correct tile position based on the zoom level", "[render-utils]") {
    // First row
    REQUIRE(Rival::RenderUtils::tileToScaledPx_Y(0, 0, 2.0f) == 0);
    REQUIRE(Rival::RenderUtils::tileToScaledPx_Y(1, 0, 2.0f) == 32);
    REQUIRE(Rival::RenderUtils::tileToScaledPx_Y(2, 0, 2.0f) == 0);
    REQUIRE(Rival::RenderUtils::tileToScaledPx_Y(3, 0, 2.0f) == 32);

    // Second row
    REQUIRE(Rival::RenderUtils::tileToScaledPx_Y(0, 1, 2.0f) == 64);
    REQUIRE(Rival::RenderUtils::tileToScaledPx_Y(1, 1, 2.0f) == 96);
    REQUIRE(Rival::RenderUtils::tileToScaledPx_Y(2, 1, 2.0f) == 64);
    REQUIRE(Rival::RenderUtils::tileToScaledPx_Y(3, 1, 2.0f) == 96);
}

TEST_CASE("worldToPx_X calculates the correct distance", "[render-utils]") {
    REQUIRE(Rival::RenderUtils::worldToPx_X(0.5f) == 16);
    REQUIRE(Rival::RenderUtils::worldToPx_X(1.0f) == 32);
    REQUIRE(Rival::RenderUtils::worldToPx_X(1.5f) == 48);
}

TEST_CASE("worldToPx_Y calculates the correct distance", "[render-utils]") {
    REQUIRE(Rival::RenderUtils::worldToPx_Y(0.5f) == 16);
    REQUIRE(Rival::RenderUtils::worldToPx_Y(1.0f) == 32);
    REQUIRE(Rival::RenderUtils::worldToPx_Y(1.5f) == 48);
}

TEST_CASE("pxToWorld_X calculates the correct distance", "[render-utils]") {
    REQUIRE(Rival::RenderUtils::pxToWorld_X(16.0f) == 0.5f);
    REQUIRE(Rival::RenderUtils::pxToWorld_X(32.0f) == 1.0f);
    REQUIRE(Rival::RenderUtils::pxToWorld_X(48.0f) == 1.5f);
}

TEST_CASE("pxToWorld_Y calculates the correct distance", "[render-utils]") {
    REQUIRE(Rival::RenderUtils::pxToWorld_Y(16.0f) == 0.5f);
    REQUIRE(Rival::RenderUtils::pxToWorld_Y(32.0f) == 1.0f);
    REQUIRE(Rival::RenderUtils::pxToWorld_Y(48.0f) == 1.5f);
}

TEST_CASE("getEntityZ calculates the correct z-positions", "[render-utils]") {
    float zStart = Rival::RenderUtils::zEntitiesStart;

    // First row
    REQUIRE(Rival::RenderUtils::getEntityZ(0, 0) == zStart + 0.0f);
    REQUIRE(Rival::RenderUtils::getEntityZ(1, 0) == zStart + 0.5f);
    REQUIRE(Rival::RenderUtils::getEntityZ(2, 0) == zStart + 0.0f);
    REQUIRE(Rival::RenderUtils::getEntityZ(3, 0) == zStart + 0.5f);

    // Second row
    REQUIRE(Rival::RenderUtils::getEntityZ(0, 1) == zStart + 1.0f);
    REQUIRE(Rival::RenderUtils::getEntityZ(1, 1) == zStart + 1.5f);
    REQUIRE(Rival::RenderUtils::getEntityZ(2, 1) == zStart + 1.0f);
    REQUIRE(Rival::RenderUtils::getEntityZ(3, 1) == zStart + 1.5f);
}

TEST_CASE("getCanvasWidth always returns an even number", "[render-utils]") {
    REQUIRE(Rival::RenderUtils::getCanvasWidth(10.03124f) == 320);
    REQUIRE(Rival::RenderUtils::getCanvasWidth(10.03125f) == 322);
}

TEST_CASE("getCanvasHeight always returns an even number", "[render-utils]") {
    REQUIRE(Rival::RenderUtils::getCanvasHeight(10.03124f) == 320);
    REQUIRE(Rival::RenderUtils::getCanvasHeight(10.03125f) == 322);
}
