# Texture Builder

A utility program for building textures from individual indexed TGA sprites.

## Build

Build using Visual Studio.

## Setup

This application requires a list of definition files, specifying which sprites to combine. These can be found within the `setup` project.

The source sprites should be placed in an `images` folder.

## Run

There are 2 possible modes of operation: spritesheet (the default) and atlas.

Both expect a directory containing definition files, and output textures to a `textures` directory.

### Spritesheet

In spritesheet mode, all source images referenced by a definition file are padded to match the size of the first source image in the file. The resulting textures always have power-of-2 dimensions (the program chooses dimensions that waste as little space as possible).

#### Usage

    texture-builder.exe DEFINITIONS_DIR

### Atlas

In atlas mode, source images of different sizes are arranged and packed together into the output texture, with the aim of wasting as little space as possible. A border is added around each image to help prevent texture bleeding.

As well as the texture, an atlas file is generated describing the position of each source image in the output texture.

#### Usage

    texture-builder.exe --atlas DEFINITIONS_DIR
