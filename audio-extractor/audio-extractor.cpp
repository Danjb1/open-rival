#include "pch.h"

#include <iostream>

int main() {

    /* AUDIO EXTRACTION PSEUDO-CODE

    while {still more bytes to read){
      // Look for the RIFF header
      if (readBytesAsString(4) != "RIFF"){
        return;
      }

      // next 4 bytes is the file length (add on 8 for these 2 fields we've already read)
      int length = readInt() + 8;
      long offset = getCurrentOffset() - 8;

      // Now read a few more bytes to help identify what type of RIFF file it is
      String type2 = readBytesAsString(4);
      String ext = ".riff";

      if (type2.equals("AVI ")) {
        ext = ".avi";
      }
      else if (type2.equals("WAVE") &&  readBytesAsString(4) .equals("fmt ")) {
        ext = ".wav";
      }
      else if (type2.equals("MIDS") &&  readBytesAsString(4) .equals("fmt ")) {
        ext = ".mid";
      }

      // found the Audio file, skip over the remaining bytes of the file, and start looking for the next file
      fileSeek(offset+length);
    }
    */
}
