#include "pch.h"

#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace Rival {
namespace Setup {

    /**
     * Reads the given "SOUNDS.dat" file and extracts the audio files therein to
     * the given output directory.
     */
    void extractAudio(std::string inputFile, std::string outputDir) {

        // Open the data file
        std::ifstream input(inputFile, std::ios::binary);
        if (!input.is_open()) {
            throw std::runtime_error("Unable to open file: " + inputFile);
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

            // Read file length
            std::uint32_t length;
            input.read(reinterpret_cast<char*>(&length), sizeof length);

            // Add 8 bits because of the fields we've just read!
            length += 8;

            // Determine the position of the start of this file
            std::streampos offset = input.tellg();
            if (offset == -1) {
                input.close();
                throw std::runtime_error("Failed to retrieve offset in file");
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

            // Read the whole file
            input.seekg(offset);
            char* data = new char[length];
            input.read(&data[0], length);

            // Write the data to a new file
            std::ostringstream filename;
            filename << outputDir
                     << "\\"
                     << std::setw(3)
                     << std::setfill('0')
                     << numFiles
                     << ext;
            std::ofstream output(filename.str(), std::ios::binary);
            if (!output.is_open()) {
                throw std::runtime_error(
                        "Failed to open file for writing:\n"
                        + filename.str()
                        + "\n");
            }
            output.write(&data[0], length);
            output.close();

            // Free memory
            delete[] data;

            ++numFiles;

            // Skip over the remaining bytes of the file
            std::streamoff lengthOffset = length;
            input.seekg(offset + lengthOffset);
        }
    }

}}  // namespace Rival::Setup
