# Open-Rival Setup

Combined setup utility that prepares all required resources using an existing Rival Realms installation.

By default, this program will attempt to:

- Locate the original game directory
- Extract the game's audio
- Extract the game's images
- Combine these images into textures
- Copy over the game's videos

## :warning: x64 Warning

This project uses inline assembly which is [not supported](https://learn.microsoft.com/en-us/cpp/assembler/inline/inline-assembler?view=msvc-170) on x64 processors. Therefore this project can *only* be compiled for x86 architectures.

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

This requires the following directories to be present:

```
res
setup/definitions
```

`setup/definitions` should contain definition files for all textures to be generated. These can be found in the [`dist`](/dist/setup/definitions) folder.

### Output

If successful, the following directories will be created and populated:

```
setup/images
res/sound
res/textures
res/video
```

`setup/images` is the result of an intermediate setup step and can be safely deleted.
