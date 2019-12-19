#include "pch.h"

#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////
#ifdef WIN32

#include <windows.h>

/**
 * Attempts to create the given directory.
 */
bool create_directory(const char* filename) {
    return CreateDirectoryA(filename, NULL) ||
        ERROR_ALREADY_EXISTS == GetLastError();
}

#endif ////////////////////////////////////////////////////////////////////////

/**
 * Reads "SOUNDS.dat" from the current directory,
 * and extracts the audio files therein to an "output" directory.
 */
int main() {

    // Open the data file
    std::ifstream input("SOUNDS.dat", std::ios::binary);
    if (!input.is_open()) {
        std::cerr << "Unable to open file\n";
        return -1;
    }

    // Create the "output" directory
    if (!create_directory("output")) {
        std::cerr << "Could not create \"output\" directory\n";
        return -1;
    }

    // Find and extract all audio files
    int numFiles = 0;
    while (!input.eof()) {

        // Look for the RIFF header
        // (4 chars plus null terminator)
        char header[5];
        input.get(&header[0], 5);
        if (std::strcmp(header, "RIFF") != 0) {
            continue;
        }

        std::cout << "Found RIFF header!\n";

        // Read file length
        uint32_t length;
        input.read(reinterpret_cast<char*>(&length), sizeof length);

        // Add 8 bits because of the fields we've just read!
        length += 8;

        // Determine the position of the start of this file
        std::streampos offset = input.tellg();
        if (offset == -1) {
            std::cerr << "Failed to retrieve offset in file\n";
            input.close();
            return 1;
        }
        offset -= 8;

        // Determine what type of RIFF file it is
        char type[5];
        input.get(type, 5);
        std::string ext = ".riff";

        // AVI
        if (std::strcmp(type, "AVI ") == 0) {
            ext = ".avi";

            // WAVE
        } else if (std::strcmp(type, "WAVE") == 0) {
            char subType[5];
            input.get(subType, 5);

            if (std::strcmp(subType, "fmt ") == 0) {
                ext = ".wav";
            }

            // MIDS
        } else if (std::strcmp(type, "MIDS") == 0) {
            char subType[5];
            input.get(subType, 5);
            if (std::strcmp(subType, "fmt ") == 0) {
                ext = ".mid";
            }
        }

        // We have found our Audio file!
        std::cout << "File " << numFiles << "\n";
        std::cout << "File offset is: " << offset << "\n";
        std::cout << "File length is: " << length << "\n";
        std::cout << "File extension is: " << ext << "\n" << std::flush;

        // Read the whole file
        input.seekg(offset);
        char* data = new char[length];
        input.read(&data[0], length);

        // Write the data to a new file
        std::ostringstream filename;
        filename << "output/" << std::setw(3) << std::setfill('0') << numFiles << ext;
        std::ofstream output(filename.str(), std::ios::binary);
        if (!output.is_open()) {
            std::cerr << "Failed to open file for writing\n";
            return -1;
        }
        output.write(&data[0], length);
        output.close();

        // Free memory
        delete[] data;

        numFiles++;

        // Skip over the remaining bytes of the file
        std::streamoff lengthOffset = length;
        input.seekg(offset + lengthOffset);
    }

    input.close();

}
