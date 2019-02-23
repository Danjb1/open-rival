# Image Extractor

**IMAGES.DAT** contains assembly code to render the majority of the game's images.

Thanks to **0xa0000** of [Reddit](https://www.reddit.com/r/asm/comments/as8ivk/how_can_i_call_a_stored_routine_with_unknown/egtl489/), we can call this code directly via `image_reader.exe`, to allow us to extract the images to individual files.

Other images (for example, the interface) can be extracted from screenshots. These can be taken by running the game through [DxWnd](https://sourceforge.net/projects/dxwnd/).

## Build

Compile using any C compiler, for example in the Developer Command Prompt for Visual Studio:

    cl image_extractor.c -D WIN32

## Run

Run the executable, and the images will be extracted to an "images" directory.

## Palettes

The game's palettes were extracted from a 24-bit screenshot taken using DxWnd. The palette was exported using Photoshop to an ACT file, and converted to text by running `convert_palette.py`.
