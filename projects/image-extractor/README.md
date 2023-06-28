# Image Extractor

This extracts the in-game images from **IMAGES.DAT**.

This file contains assembly code to render the majority of the game's images.

Thanks to **0xa0000** of [Reddit](https://www.reddit.com/r/asm/comments/as8ivk/how_can_i_call_a_stored_routine_with_unknown/egtl489/), we can call this code directly, to allow us to extract the images to individual files.

Interface images are extracted by [interface-extractor](/projects/interface-extractor).

## :runner: Run

`IMAGES.DAT` should be placed in the directory from which the program is run.

Run the executable, and the images will be extracted to an `images` directory.

A full list of images can be found [here](/docs/research/images.md).

## :art: Palettes

The original game uses various color palettes, which can be obtained from screenshots taken using [DxWnd](https://sourceforge.net/projects/dxwnd/) and exported to ACT files using Photoshop. These can be converted to text using [this script](convert_palette.py).

For the in-game palette, the team colours start at index 160:

    160 - Red
    166 - Cyan
    172 - Green
    178 - Yellow
    184 - Orange
    190 - Violet
    196 - Blue
    202 - Black
