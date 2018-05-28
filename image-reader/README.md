# Rival Realms Image Extractor

Rival Realms comes with 2 files, *IMAGES.DAT* and *IMOFFS.DAT*. The latter contains the image offsets, while the former contains the image data.

The format of these files has not yet been determined. A friend of mine who specialises in reverse engineering has suggested that the image data is actually dynamically-loaded code that renders the images, and one of the original developers has confirmed this to be the case, at least for the character images.

However, Watto, developer of [Game Extractor](http://www.watto.org/game_extractor.html), has attempted to decipher the format. His findings can be found below, and within this project can be found my attempts to read the files based on his suggested structure.

    +--------------------------------------+
    | Rival Realms IMOFFS.DAT + IMAGES.DAT |
    +--------------------------------------+

    // The IMOFFS file contains the directory, and IMAGES file contains the image data
    // Some of the Offsets in the Groups Directory are null, or are duplicates of previous offsets


    // IMAGES.DAT FILE

      // HEADER
        4 - Number of Images
        
      // FILE DATA
        X - Unknown
        
        // for each image in the Main Section
          X - Image Data (RLE Encoding?)
          
        // for each image in the Final Section
          X - Image Data
          

    // IMOFFS.DAT FILE

      // GROUPS DIRECTORY
        // for each group
          4 - Offset to Group Details (in the Details Directory)
          
      // DETAILS DIRECTORY
        // for each group
          1 - Number of Sub-Groups
          1 - Unknown
          1 - Image Width? (for each image in this group)
          1 - Image Height? (for each image in this group)
          
          // for each sub-group
            1 - Number of Primary Images in this sub-group
            1 - Number of Secondary Images in this sub-group
          
            // for each primary and secondary image in this sub-group
              3 - File Offset (in IMAGES.DAT file) (if ImageInFinalSection, offset is relative to the start of the Final Section)
              1 - Unknown (0=Image in Main Section, 1=Image in Final Section, 128=?)
              2 - File Length
              1 - Unknown
              1 - Unknown



    This is what I have so far...

    1. The IMAGES.DAT seems to have 3 sections in it. There is 309700 bytes of data at the beginning, then there are all the images (which I'm calling the Main Section) (from offset 309700 to offset 16777744), and at the end there is another small block of images (which I'm calling the Final Section) (from offset 16777744 to offset 17513502).

    2. If you read through the specs above, you will see the 3 byte field for the File Offset, then a 1-byte field following it...
            a. If the 1-byte field is 0, this is an image, and the File Offset and File Length values are correct.
            b. If the 1-byte field is 1, this image appears in the small block of images at the end (the Final Section). File Offset is relative to the start of the Final Section. File Length values look correct 
            c. If the 1-byte field is 128, I'm not sure what it is. The File Offset values are all very small, so am thinking it might be something else, just just an entry that describes some image properties.

    3. All the images in the Main Section start with a single (byte)185, maybe it's a magic value to indicate that it's an image?
