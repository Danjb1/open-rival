#include "UnitAnimation.h"

#include <stdexcept>
#include <string>

namespace Rival {

	UnitAnimation::UnitAnimation(UnitType unitType) : 
        unitAnimationPair(std::make_pair(unitType, UnitAnimationType::Standing)) {
        updateSpriteSheetEntry();
    }

    void UnitAnimation::setFacing(Facing newFacing) {
        facing = newFacing;
    }

    void UnitAnimation::rotateLeft() {
        int newFacing = ( static_cast<int>( facing ) - 1 ) % 8;
        setFacing( static_cast<Facing>( newFacing ) );
    }

    void UnitAnimation::rotateRight() {
        int newFacing = ( static_cast<int>( facing ) + 1 ) % 8;
        setFacing( static_cast<Facing>( newFacing ) );
    }

    void UnitAnimation::setAnimation(UnitAnimationType unitAnimationType ) {
        unitAnimationPair.second = unitAnimationType;
        updateSpriteSheetEntry();
    }

    int UnitAnimation::getCurrentSpriteIndex() const {
        auto spritesheetSpan = spritesheetEntry.second - spritesheetEntry.first + 1;
        auto directionOffset = static_cast<int>(facing) - static_cast<int>(Facing::South);
        return spritesheetEntry.first + spritesheetSpan * directionOffset + animationStep;
    }

    void UnitAnimation::updateSpriteSheetEntry() {
        auto lookupIt = unitAnimationLookup.find( unitAnimationPair );
        if ( unitAnimationLookup.end() == lookupIt ) {
            throw std::logic_error( "Can't find SpritesheetEntry" );
        }
        spritesheetEntry = lookupIt->second;
    }

    void UnitAnimation::tick() {
        auto spritesheetSpan = spritesheetEntry.second - spritesheetEntry.first + 1;
        animationStep = ( animationStep + 1 ) % ( spritesheetSpan );
    }
}
