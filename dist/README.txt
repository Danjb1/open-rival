================================================================================
Open-Rival v0.2.0
  by Danjb (admin@danjb.com)
================================================================================

-------------
Introduction
-------------

Thank you for downloading Open-Rival and supporting this classic game!

    https://github.com/Danjb1/open-rival

------
Setup
------

The assets are not provided, and must be extracted from the original game.

Run "setup.exe" to perform this extraction.

-------------
Run
-------------

Launch "Open-Rival.exe" to run the game!

-------------
Settings
-------------

Some settings can be configured by editing "config.json":

--- Audio ---

# midiEnabled

    Whether or not the MIDI soundtrack should be played (true/false).

# soundEnabled

--- Fonts ---

    Whether or not sound effects should be played (true/false).

# fontDirs

    An array of directories that should be searched when looking for font files.
    They will be searched in the order listed.

    Example:

        "fontDirs": [
            "res\\fonts\\",
            "C:\\Windows\\Fonts\\",
            "C:\\Users\\Danjb\\AppData\\Local\\Microsoft\\Windows\\Fonts\\"
        ]

# fontSmall

    The name of the font file to use for small text.

    Example:

        "fontSmall": "serife.fon"

# fontRegular

    The name of the font file to use for regular text.

    Example:

        "fontRegular": "Procopius Regular.ttf"

# fontSmallSize

    The font size used for small text.

    Note that different fonts may require very different values. For example,
    "serife.fon" requires a high value, despite the resulting font size being
    small.

    Example:

        "fontSmallSize": 32

# fontRegularSize

    The font size used for regular text.

    Example:

        "fontRegularSize": 16
