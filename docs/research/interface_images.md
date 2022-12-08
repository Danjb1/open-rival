# Interface Images

## Game

Images listed between the arrows below are repeated three times with slight variations for different races.

In actual fact, most of the images are the same for all races. Only Greenskin has a few different backgrounds.

    skills                  34x34
    items                   42x45
    resources               14x14
    colour blocks           20x20
    map scroll arrows       64x32
    level icons             14x14
    level icon (small)      6x6
    status bar              800x22
    selected unit HUD       425x144         <---
    horizontal border?      210x5
    vertical border?        5x144
    block?                  16x14
    square background?      68x69
    light background        211x51
    dark background         211x47
    bag messages            153x20
    solid black             30x14
    solid black             48x14
    spybook                 216x247
    background              160x144
    minimap buttons         26x21
    group buttons           13x14
    minimap buttons         27x29
    menu scroll buttons     10x58
    green cross icon        29x31
    sell icon               27x28
    go get upgraded icon    28x28
    transfer button         150x32
    exit button             30x15
    up / down arrows        16x13
    hire button             150x32         <---

We should combine these into the following textures:

1) Items (spritesheet)
2) Skills (spritesheet)
3) UI, including race variants (texture atlas)

We will need a program to generate texture atlases in the appropriate format. We will probably have to write this ourselves given the unusual file format of our textures. Given a list of images, this will output a texture and an accompanying text file describing the pixel co-ordinates of each image within the texture. For now, we needn't worry about optimal packing; just stacking them vertically is fine.

> Each image should have a 1px border to prevent texture bleeding!

We can then load this texture and parse this text file in Open-Rival to create a TextureAtlas. Similar to a Spritesheet, this will include methods to generate the tex co-ords of a given image.
