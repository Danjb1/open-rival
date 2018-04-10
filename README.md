# Open Rival

**Rival Realms** was a fantastic but little-known real-time strategy game from 1998, developed by Activ Pub and published by Titus Software. Almost all traces of this game have now disappeared from the internet. This project is an attempt to revive the game, and maybe even make a remake possible.

![Title](http://danjb.com/images/rival_realms/title.jpg)

The original game can still be run on Windows 10, however there is a game-breaking bug where units no longer recover health when resting. Multiplayer games are also very difficult, if not impossible, to get working. A remake could fix these issues, add support for higher resolutions, and much more.

## Assets

### Graphics

The game's images are presumably stored in the file called *IMAGES.DAT*, but apparently this file actually contains dynamically-loaded code, and it is certainly not a format I recognise.

I have attempted to extract the game's graphics through a somewhat more manual process with the help of DxWnd author **ghotik** (full thread [here](https://sourceforge.net/p/dxwnd/discussion/general/thread/19bc4f4b/)). This involves running the game through the wrapper program DxWnd, with some special settings enabled to allow screenshots to be taken in rapid succession.

These screenshots can then be run through my [Sprite Extractor](https://github.com/Danjb1/sprite-extractor), [Colour Remover](https://github.com/Danjb1/colour-remover) and [Stray Pixel Cleaner](https://github.com/Danjb1/stray-pixel-cleaner) to extract individual sprites, but the process is imperfect and the resulting images still need touching up by hand.

### Sound

The sound files can be extracted from *SOUNDS.DAT* using Watto's [Game Extractor](http://www.watto.org/game_extractor.html). These are mostly WAV files, but there are also a handful of [RIFF files](http://www.vgmpf.com/Wiki/index.php/MDS) that I have not yet figured out how to play.

For some reason, these sounds seem to be of a worse - or at least, different - quality than those heard in-game.

### Music

The music was ripped directly from the game's CD using Windows Media Player.

### Videos

These are taken from the game's *MOVIES* directory.

## Legal Stuff

I have tried to track down the copyright holders, even the original developers, but I have had very little response. If you are reading this and you have any connection to this game, please contact me! But until that time, I consider it to be abandonware.

## Links

 - [Intro and Music Playlist](https://www.youtube.com/watch?v=N7JFz1GapOE&list=PL6389EE9ABE2B104B)

 - [Let's Play](https://www.youtube.com/watch?v=bSefVvZnxKs)
