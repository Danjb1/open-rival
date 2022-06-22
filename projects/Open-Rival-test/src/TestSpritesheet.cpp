#include "pch.h"
#include "catch2/catch.h"

#include "Spritesheet.h"
#include "Texture.h"

using namespace Rival;

SCENARIO("Spritesheet calculates the correct texture co-ordinates", "[spritesheet]")
{

    GIVEN("A Spritesheet that divides up a Texture")
    {
        Texture texture(0, 64, 64);
        Spritesheet spritesheet(texture, 32, 32);

        WHEN("getting the texture co-ordinates for a frame of the sprite")
        {
            const std::vector<GLfloat> texCoords = spritesheet.getTexCoords(0);
            float tx1 = texCoords[0];
            float ty1 = texCoords[1];
            float tx2 = texCoords[2];
            float ty2 = texCoords[5];

            THEN("the correct texture co-ordinates are calculated")
            {
                REQUIRE(tx1 == 0.0f);
                REQUIRE(ty1 == 1.0f);
                REQUIRE(tx2 == 0.5f);
                REQUIRE(ty2 == 0.5f);
            }
        }
    }
}
