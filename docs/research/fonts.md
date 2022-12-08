# Font Key

## Finding the Fonts

The game predominantly uses 2 fonts (regular and small). Examples of each can be found alongside this file.

### Initial Observations

These observations were made when trying to render the following characters in the original game:

    abcdefghijklmnopqrstuvwxyz
    ABCDEFGHIJKLMNOPQRSTUVWXYZ
    1234567890
    !"£$%^&*()-_+={}[];:'@#~,.<>/?\|`¬

 - Neither font is monospaced
 - `£` is rendered as `ù` for some reason
 - The characters `{}¬` are not rendered at all

### Regular Font

The online tool [WhatTheFont](https://www.myfonts.com/WhatTheFont) has identified this as [Procopius](https://www.myfonts.com/fonts/scriptorium/procopius) by the late [Dave Nalle](http://www.fontcraft.com/fontcraft/about/). Some characters are different, but overall it looks very similar.

> **Note:** "Scriptuale Pro Regular" is almost identical to Procopius, except for some slight differences in punctuation.

A shareware version of this font can be downloaded [here](https://fontsgeek.com/fonts/Procopius-Regular), but it is missing accented characters.

### Small Font

This is "MS Serif", size 11/12.

## Distribution Options

The Procopius font uses a commercial license, so distributing it with the game could be problematic. Here are our options, in preference order, depending on what permissions we can obtain:

1. Distribute the complete font file directly with the game so that we can programmatically extract font metrics. Pack the font into a texture for rendering.

2. Same as (1), but save the font metrics directly to a data file and pre-render the texture so that we need not distribute the font itself.

3. Same as (1), but using the shareware version of the font. Accents will not be available.

4. Same as (2), but using the shareware version of the font. Accents will not be available.

5. Do not distribute the font at all, but use it if is present on the system; use a substitute font otherwise.

> To date, I have reached out twice via [this form](http://www.fontcraft.com/fontcraft/contact/), but I have had no reply.

Regardless, we will provide a means for fonts to be configurable, so distribution issues will not prevent anyone from playing the game.

## Using the Fonts

### Procopius

This is a TrueType Font file which can be easily loaded with the FreeType library.

### MS Serif

This font is a little more archaic - it is a Windows bitmap font (`.fon` file).

Fortunately, FreeType supports this format through its [`winfonts` module](https://www.freetype.org/freetype2/docs/design/design-5.html), which is included as standard.
