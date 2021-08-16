/**
 * Script to extract the Rival Realms images from "IMAGES.DAT".
 *
 * @author 0xa0000 - All the hard work (this guy is a wizard!).
 * @author Danjb - Tweaks and formatting changes.
 */

// Formatter does not play nice with our preprocessor directives and ASM code
/* clang-format off */

#include "pch.h"
#include "image-extractor.h"

#include <cstdint>
#include <stdexcept>
#include <stdio.h>
#include <stdint.h>
#include <windows.h>

#include "FileUtils.h"
#include "Image.h"
#include "Palette.h"
#include "setup-utils.h"

namespace Rival {
namespace Setup {

    // Largest image in "IMAGES.DAT" is 128 x 128
    const int maxWidth = 128;
    const int maxHeight = 128;

    // Size of pixel data, in bytes
    const int imageSize = maxWidth * maxHeight;

    // 4 byte header: number of images in file
    const int headerSize = 4;

    // Team colors to use when rendering
    // (these 6 colors correspond to a single team)
    std::uint8_t teamColor[6] = { 160, 161, 162, 163, 164, 165 };

    // Pixel value corresponding to tranparency
    int transparentColor = 0xff;

///////////////////////////////////////////////////////////////////////////////
#ifdef WIN32

    /**
     * Makes a section of memory executable.
     */
    BOOL makeExecutable(void* data, std::uint32_t size) {
        DWORD old;
        return VirtualProtect(data, size, PAGE_EXECUTE_READWRITE, &old);
    }

#endif  //////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER

    /**
     * Calls the assembly code at the given memory location.
     */
    void callAssemblyCode(void* code, std::uint8_t* image) {
        __asm {
            pushad
            mov esi, offset teamColor    // 6-byte array containing the team colors
            mov edi, image               // image pointer
            mov edx, maxWidth          // stride
            call [code]
            popad
        }
    }

#endif  //////////////////////////////////////////////////////////////////////

#define MODRM_MOD(modrm) (((modrm) >> 6) & 0x3)
#define MODRM_REG(modrm) (((modrm) >> 3) & 0x7)
#define MODRM_RM(modrm) (((modrm) >> 0) & 0x7)

    /**
     * Finds the end of the function at the given memory location.
     *
     * Specifically, returns one past next 'ret' (0xC3) instruction.
     *
     * Recognizes only the subset of x86 actually used and takes as many shortcuts
     * as possible.
     */
    std::uint8_t* findFunctionEnd(uint8_t* start, std::uint8_t* end, bool* readFromEsi) {
        *readFromEsi = 0;

        while (start < end) {
            std::uint8_t inst = *start++;
            const int opSize = inst == 0x66;  // Operand size prefix
            if (opSize)
                inst = *start++;

            if ((inst & 0xf0) == 0x40) {
                // inc/dec r16/r32
            } else if ((inst & 0xf8) == 0xb0) {
                // MOV r8, imm8
                start++;  // imm8
            } else if ((inst & 0xf8) == 0xb8) {
                // MOV  r16/r32, imm16/imm32
                start += opSize ? 2 : 4;
            } else if (inst == 0x03) {
                // ADD     r16/32     r/m16/32
                start++;  // MODR/M
            } else if (inst == 0x83) {
                // ALUOP r32, imm8
                start++;  // MODR/M
                start++;  // imm8
            } else if (inst == 0x8a || inst == 0x8b) {
                // MOV     r16/32     r/m16/32
                const std::uint8_t modrm = *start++;  // MODR/M
                if (MODRM_MOD(modrm) == 1) {
                    start++;  // disp8
                }
                if (MODRM_MOD(modrm) != 0x03 /* && MODRM_RM(modrm) == 0x6*/) {
                    *readFromEsi = true;
                }
            } else if (inst == 0x69) {
                // IMUL r16/32     r/m16/32 imm16/32
                start++;  // MODR/M
                start += opSize ? 2 : 4;
            } else if (inst == 0x6b) {
                // IMUL r16/32 r/m16/32 imm8
                start++;  // MODR/M
                start++;  // imm8
            } else if (inst == 0x86) {
                // XCHG r8, r/m8
                start++;  // MODR/M
            } else if (inst == 0xaa) {
                // stosb
            } else if (inst == 0xab) {
                // stosw/stosd
            } else if (inst == 0xc3) {
                // ret!
                break;
            } else {
                printf("Unhandled instruction %02X\n", inst);
                abort();
            }
        }

        return start;
    }

    /**
     * Extracts images from the given "IMAGES.DAT" file,
     * to the given output directory.
     */
    void extractImages(std::string inputFile, std::string outputDir) {

        // Read the file
        auto data = FileUtils::readBinaryFile(inputFile);
        auto size = data.size();

        // Make the code contained within "IMAGES.DAT" executable
        if (!makeExecutable(data.data(), size)) {
            throw std::runtime_error("Failed to make memory executable");
        }

        int i = 0;
        std::uint8_t* start = data.data() + headerSize;
        std::uint8_t* end = data.data() + size;

        // Extract the images!
        for (uint8_t* code = start; code < end; ++i) {

            // We don't currently use this, but it can tell us if this image
            // made use of the team color parameter
            bool readFromEsi = false;

            // Figure out where the next function starts
            std::uint8_t* const codeEnd = findFunctionEnd(code, end, &readFromEsi);

            // Create an empty image buffer
            std::vector<std::uint8_t> data(imageSize, transparentColor);

            // Draw the image with our desired team color
            callAssemblyCode(code, data.data());

            // Figure out the image dimensions based on what was drawn
            int w = 0;
            int h = 0;
            for (int y = 0; y < maxHeight; ++y) {
                for (int x = 0; x < maxWidth; ++x) {
                    if (data[x + y * maxWidth] != transparentColor) {
                        if (x > w) {
                            w = x + 1;
                        }
                        if (y > h) {
                            h = y + 1;
                        }
                    }
                }
            }

            // Ensure the image is not empty (should never happen)
            if (w == 0) {
                w = 1;
            }
            if (h == 0) {
                h = 1;
            }

            // Ensure the image is square
            if (w > h) {
                h = w;
            } else if (h > w) {
                w = h;
            }

            // Round image dimensions to nearest power of 2
            w = h = nextPowerOf2(w);

            // Save the rendered image to disk
            char filename[256];
            snprintf(filename,
                    sizeof(filename),
                    "%s\\img_%04d.tga",
                    outputDir.c_str(), i);
            Image image(w, h,
                    std::make_unique<std::vector<std::uint8_t>>(data),
                    maxWidth);
            writeImage(image, Palette::paletteGame, filename);

            // Jump to the next image
            code = codeEnd;
        }
    }

}}  // namespace Rival::Setup

/* clang-format on */
