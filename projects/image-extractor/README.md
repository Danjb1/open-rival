# Image Extractor

**IMAGES.DAT** contains assembly code to render the majority of the game's images.

Thanks to **0xa0000** of [Reddit](https://www.reddit.com/r/asm/comments/as8ivk/how_can_i_call_a_stored_routine_with_unknown/egtl489/), we can call this code directly, to allow us to extract the images to individual files.

Other images (for example, the interface) can be extracted from screenshots. These can be taken by running the game through [DxWnd](https://sourceforge.net/projects/dxwnd/).

## Build

Build using Visual Studio.

## Run

`IMAGES.DAT` should be placed in the directory from which the program is run.

Run the executable, and the images will be extracted to an `images` directory.

A full list of images can be found [here](/docs/images.md).

## Palettes

Some images have parts that can change colour, for example units that change colour based on their team. This program specifies which colours from the game's palette should be used for these parts.

The palette was captured in a 24-bit screenshot taken using DxWnd. This was exported to an ACT file using Photoshop, and converted to text by running [this script](convert_palette.py).

The team colours start at index 160:

    160 - Red
    166 - Cyan
    172 - Green
    178 - Yellow
    184 - Orange
    190 - Violet
    196 - Blue
    202 - Black
