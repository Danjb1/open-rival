# Open Rival

**Rival Realms** was a fantastic but little-known real-time strategy game from 1998, developed by Activ Pub and published by Titus Software. Almost all traces of this game have now disappeared from the internet. This project is an attempt to revive the game, and maybe even make a remake possible.

![Title](http://danjb.com/images/rival_realms/title.jpg)

The original game can still be run on Windows 10, however there is a game-breaking bug where units no longer recover health when resting. Multiplayer games are also very difficult, if not impossible, to get working. A remake could fix these issues, add support for higher resolutions, and much more.

## Assets

### Graphics

#### Data Files

The game's images are stored in the file called *IMAGES.DAT*, but this is very difficult to decipher. You can read more about this [here](/image-reader).

#### Manual Extraction

I have attempted to extract the game's graphics through a somewhat more manual process with the help of DxWnd author **ghotik** (full thread [here](https://sourceforge.net/p/dxwnd/discussion/general/thread/19bc4f4b/)). This involves running the game through the wrapper program DxWnd, with some special settings enabled to allow screenshots to be taken in rapid succession.

These screenshots can then be run through my [Sprite Extractor](https://github.com/Danjb1/sprite-extractor), [Colour Remover](https://github.com/Danjb1/colour-remover) and [Stray Pixel Cleaner](https://github.com/Danjb1/stray-pixel-cleaner) to extract individual sprites, but the process is imperfect and the resulting images still need touching up by hand.

I have preserved the shadows but separated them from the main images. It is possible the shadows could be generated programmatically instead. The closest I have been able to achieve is using the following Skew transform in Photoshop:

    Centre reference point x: +7
    Centre reference point y: +14
    Horizontal scale: 80%
    Vertical scale: 55%
    Angle: 0
    Horizontal skew: -40
    Vertical skew: 0

### Sound

The sound files can be extracted from *SOUNDS.DAT* using Watto's [Game Extractor](http://www.watto.org/game_extractor.html).

For some reason, these sounds seem to be of a worse - or at least, different - quality than those heard in-game.

### Music

#### WMA

The music was ripped directly from the game's CD using Windows Media Player.

Track 11 seems to be the same as Track 14, but with a few seconds of silence at the end.

#### MID

The MID files were contained within *SOUNDS.DAT* alongside the sound effects and can be played in [foobar2000](https://www.foobar2000.org/) using the [MIDI Player](https://www.foobar2000.org/components/view/foo_midi) component by **kode54**.

Track 6 was duplicated.

### Videos

These are taken from the game's *MOVIES* directory.

## Legal Stuff

I have tried to track down the copyright holders, even the original developers, but I have had very little response. If you are reading this and you have any connection to this game, please contact me! But until that time, I consider it to be abandonware.

## Links

 - [Intro and Music Playlist](https://www.youtube.com/watch?v=N7JFz1GapOE&list=PL6389EE9ABE2B104B)

 - [Let's Play](https://www.youtube.com/watch?v=bSefVvZnxKs)
