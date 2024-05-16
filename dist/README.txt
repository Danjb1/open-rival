           _____                       ______ _            _ 
          |  _  |                      | ___ (_)          | |
          | | | |_ __   ___ _ __ ______| |_/ /___   ____ _| |
          | | | | '_ \ / _ \ '_ \______|    /| \ \ / / _` | |
          \ \_/ / |_) |  __/ | | |     | |\ \| |\ V / (_| | |
           \___/| .__/ \___|_| |_|     \_| \_|_| \_/ \__,_|_|
                | |                                          
                |_|                                    v0.4.0

            by Danjb (admin@danjb.com)
            https://github.com/Danjb1/open-rival

>>>> Thank you for downloading Open-Rival and supporting this classic game! <<<<

================================================================================
Contents
================================================================================

1. Getting Started
2. Multiplayer
3. Known Issues
4. Settings

================================================================================
1. Getting Started
================================================================================

--------------------------------------------------------------------------------
Setup
--------------------------------------------------------------------------------

The assets are not provided, and must be extracted from the original game.

Run `setup.exe` to perform this extraction.

This will attempt to locate the original game on your system. If this fails, you
can also specify the game directory manually by running from the command line,
e.g.

    setup.exe -d "C:\rivalrealms"

--------------------------------------------------------------------------------
Playing the Game
--------------------------------------------------------------------------------

Once setup is complete, launch `Open-Rival.exe` to run the game!

By default this will load a test map, but any map from the original game can be
loaded (see below).

================================================================================
2. Multiplayer
================================================================================

Multiplayer games are possible by launching the game through the command line.

--------------------------------------------------------------------------------
Host
--------------------------------------------------------------------------------

To host a game:

    Open-Rival.exe -host [-port PORT]

For example:

    Open-Rival.exe -host -port 12345

Omitting the port parameter uses the default port of 25565.

Once all players are connected in the lobby, press Enter to start the game.

--------------------------------------------------------------------------------
Join
--------------------------------------------------------------------------------

To join a game:

    Open-Rival.exe -connect ADDRESS [-port PORT]

Omitting the port parameter uses the default port of 25565.

================================================================================
3. Known Issues
================================================================================

Thank you for your patience during these early stages of development!

- (Not implemented) No visual effect is displayed when issuing a move command.
- (Not implemented) Units can walk through buildings and other objects.
- (Not implemented) Buildings cannot be selected.
- (Not implemented) Selected units do not flash orange.
- (Not implemented) No rectangle is displayed when drag-select is held.
- (Not implemented) Some objects (e.g. chests) are not yet loaded.
- (Not implemented) Unit hitboxes are not accurate.
- (Not implemented) Vehicles should have their inventories hidden.
- (Bug) Flying units sometimes appear behind units they should be in front of.
- (Bug) Units can walk too close to the edge of the map.
- (Bug) Cursor position should have a greater impact on zooming.
- (Bug) Some animations look a little off (e.g. Snake).
- (Bug) Drag-select rectangle disappears when shrunk to a small size.
- (Bug) Pathfinding lags the game when moving very large groups of units.

================================================================================
4. Settings
================================================================================

Some settings can be configured by editing `config.json`.

--------------------------------------------------------------------------------
Audio
--------------------------------------------------------------------------------

=> midiEnabled

    Whether or not the MIDI soundtrack should be played (true/false).

=> soundEnabled

    Whether or not sound effects should be played (true/false).

--------------------------------------------------------------------------------
Fonts
--------------------------------------------------------------------------------

=> fontDirs

    An array of directories that should be searched when looking for font files.
    They will be searched in the order listed.

    By default, the example directories shown here are used (adapted to your
    user).

    Example:

        "fontDirs": [
            "res\\fonts\\",
            "C:\\Windows\\Fonts\\",
            "C:\\Users\\USERNAME\\AppData\\Local\\Microsoft\\Windows\\Fonts\\"
        ]

=> fontSmall

    The name of the font file to use for small text.

    Example:

        "fontSmall": "serife.fon"

=> fontRegular

    The name of the font file to use for regular text.

    Example:

        "fontRegular": "Procopius Regular.ttf"

=> fontSmallSize

    The font size used for small text.

    Note that different fonts may require very different values. For example,
    "serife.fon" requires a high value, despite the resulting font size being
    small.

    Example:

        "fontSmallSize": 32

=> fontRegularSize

    The font size used for regular text.

    Example:

        "fontRegularSize": 16

--------------------------------------------------------------------------------
Gameplay
--------------------------------------------------------------------------------

=> levelName

    Name of the level to load (should be placed in the `res/maps` directory).

    Example:

        "levelToLoad": "my_map.sco"

--------------------------------------------------------------------------------
Logging
--------------------------------------------------------------------------------

=> logLevel

    Level of detail to use for log messages.

    Acceptable values are:

        "off", 
        "trace"
        "debug"
        "info"
        "warn"
        "error"
        "critical"

=> logToConsole

    Whether to log to the console window when running from a Command Prompt
    (true / false).

=> logToFile

    Whether to log to a file (true / false).

    Log files are saved to a `logs` directory.
