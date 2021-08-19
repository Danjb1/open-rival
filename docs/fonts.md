# Font Key

## Characters Tested

    abcdefghijklmnopqrstuvwxyz
    ABCDEFGHIJKLMNOPQRSTUVWXYZ
    1234567890
    !"£$%^&*()-_+={}[];:'@#~,.<>/?\|`¬

## Observations

 - Neither font is monospaced
 - `£` is rendered as `ù`
 - The characters `{}¬` are not rendered at all

## Regular Font

The online tool [WhatTheFont](https://www.myfonts.com/WhatTheFont) has identified this as [Procopius](https://www.myfonts.com/fonts/scriptorium/procopius) by the late [Dave Nalle](http://www.fontcraft.com/fontcraft/about/). Some characters are different, but overall it looks very similar.

> **Note:** "Scriptuale Pro Regular" is almost identical to Procopius, except for some slight differences in punctuation.

A shareware version of this font can be downloaded [here](https://fontsgeek.com/fonts/Procopius-Regular), but it is missing accented characters.

### Options

Here are our options, in preference order, depending on what permissions we can obtain:

1. Distribute the complete font file directly with the game so that we can programmatically extract font metrics. For rendering, we will probably want to bake the font into a texture at compile time.

2. Same as (1), but save the font metrics directly to a data file so that we need not distribute the font itself.

3. Same as (1), but using the shareware version of the font. Accents will not be available.

4. Same as (2), but using the shareware version of the font. Accents will not be available.

5. Do not distribute the font at all, but use it if is present on the system; use a substitute font otherwise.

> To date, I have reached out twice via [this form](http://www.fontcraft.com/fontcraft/contact/), but I have had no reply.

## Small Font

This is "MS Serif", size 11/12.
