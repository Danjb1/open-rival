# Open-Rival Setup

Combined setup utility that prepares all required resources using an existing Rival Realms installation.

By default, this program will attempt to:

- Locate the original game directory
- Extract the game's audio
- Extract the game's images
- Combine these images into textures
- Copy over the game's videos

## :runner: Run

### Game Directory

By default, this looks for the Rival Realms install directory in the Windows registry.

Specifically, it looks for the string value:

```
HKEY_CURRENT_USER\Software\Titus Games\Rival Realms\Game Directory
```

> A suitable registry file is included with [this download](https://www.old-games.com/download/5514/rival-realms) of the original game, or can be easily created.

Alternatively, a directory can be specified using the `-d` argument, for example:

```
setup.exe -d "C:\Rival Realms"
```

### Requirements

This requires the `setup/definitions` directory to be present alongside the executable, containing definition files for all textures to be generated. This should be automatically copied to the output directory by CMake.

### Output

If successful, the following directories will be created and populated:

```
setup/images
res/sound
res/textures
res/video
```

`setup/images` is the result of an intermediate setup step and can be safely deleted.
