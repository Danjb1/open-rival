# Graphics Extraction Process

Individual sprites can be extracted manually, but the process is time-consuming and imperfect, and the resulting images may still need touching up by hand.

## Setup

1. Install the latest DxWnd

1. Enable expert mode in the Options menu

1. Set a recording hotkey:

    *Options > Global settings > Function Keys > Custom t.*

1. *(Optional)* Set a frame limit to avoid duplicated screenshots:

    *Modify "Rival Realms" Target > Timing > Limit (e.g. 15 or 30)*

## Screenshot Capturing

1. Launch game through DxWnd

1. Load test map

1. Enable "see all" cheat

1. Very quickly:

    1. Select a unit and issue a move command

    1. Deselect the unit while it is still moving

    1. Move the mouse out of the way

    1. Press recording hotkey (Alt + F8) to start recording

    1. After a few steps, press recording hotkey (Alt + F8) to stop recording

## Screenshot Processing

1. Run [Sprite Extractor](https://github.com/Danjb1/sprite-extractor) to extract the sprites from the screenshots

1. Run [Sprite Fixer](https://github.com/Danjb1/sprite-fixer) to consolidate any duplicates and fix any highlighted pixels

1. Check over the images in case there are any accidental holes

1. Find the first frame of the animation, and rename the images accordingly
