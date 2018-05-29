# Graphics Extraction Process

Individual sprites can be extracted manually, but the process is time-consuming and imperfect, and the resulting images may still need touching up by hand.

## Setup

1. Install the latest DxWnd

1. Enable expert mode in the Options menu

1. Set a recording hotkey in Options > Global settings > Function Keys > Custom t.

## Screenshot Capturing

1. Launch game through DxWnd

1. Load test map

1. Enable "see all" cheat

1. Select a unit and issue a move command

1. Deselect the unit while it is still moving

1. Move the mouse out of the way

1. Press recording hotkey (Alt + F8) to start recording

1. Wait for the unit to take a few steps

1. Press recording hotkey (Alt + F8) to stop recording

## Screenshot Processing

1. Run [Sprite Extractor](https://github.com/Danjb1/sprite-extractor) to extract the sprites from the screenshots
 
1. Fix any ambiguous pixels by hand

1. Remove any extraneous images so we have one per frame of the animation

1. Run [Colour Remover](https://github.com/Danjb1/colour-remover) to remove the shadows

1. (Optional) Run [Stray Pixel Cleaner](https://github.com/Danjb1/stray-pixel-cleaner) if needed

## Shadow Generation

This process preserves the shadows but separates them from the main images. It is possible the shadows could be generated programmatically instead. The following Skew transform is Photoshop gets pretty close:

    Centre reference point x: +7
    Centre reference point y: +14
    Horizontal scale: 80%
    Vertical scale: 55%
    Angle: 0
    Horizontal skew: -40
    Vertical skew: 0

