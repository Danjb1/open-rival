# Assets

One of the tricky aspects of remaking the game is extracting the original assets. The steps taken to achieve this are described below.

## Graphics

### Data Files

The game's images are stored in the file called *IMAGES.DAT*, but this is very difficult to decipher. You can read more about this [here](/image-reader).

### Manual Extraction

I have attempted to extract the game's graphics through a somewhat more manual process with the help of DxWnd author **ghotik** (full thread [here](https://sourceforge.net/p/dxwnd/discussion/general/thread/19bc4f4b/)). This involves running the game through the wrapper program DxWnd, with some special settings enabled to allow screenshots to be taken in rapid succession. More about this [here](/docs/GFX.md).

## Sound

The sound files can be extracted from *SOUNDS.DAT* using Watto's [Game Extractor](http://www.watto.org/game_extractor.html).

For some reason, these sounds seem to be of a worse - or at least, different - quality than those heard in-game.

## Music

### WMA

The music was ripped directly from the game's CD using Windows Media Player.

Track 11 seems to be the same as Track 14, but with a few seconds of silence at the end.

### MID

The MID files were contained within *SOUNDS.DAT* alongside the sound effects and can be played in [foobar2000](https://www.foobar2000.org/) using the [MIDI Player](https://www.foobar2000.org/components/view/foo_midi) component by **kode54**.

Track 6 was duplicated.

## Videos

These are taken from the game's *MOVIES* directory.

## Maps

The scenario (*.sco*) format is under analysis and will be documented [here](/docs/MAPS.md).
