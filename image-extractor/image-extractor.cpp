/**
 * Script to extract the Rival Realms images from "IMAGES.DAT".
 *
 * @author 0xa0000 - All the hard work (this guy is a wizard!).
 * @author Danjb - Tweaks and formatting changes.
 */

#include "pch.h"
#include "image-extractor.h"

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

namespace ImageExtractor {

    // Largest image in "IMAGES.DAT" is 128 x 128
    const int IMAGE_WIDTH = 128;
    const int IMAGE_HEIGHT = 128;

    // 4 byte header: number of images in file
    const int HEADER_SIZE = 4;

    // Number of colours in the palette
    const int PALETTE_SIZE = 256;

    // The game's colour palette.
    // The last colour (index 0xff) is used for transparent areas;
    // this colour is never written by the code in "IMAGES.DAT".
    /* clang-format off */
    const uint32_t PALETTE[PALETTE_SIZE] = {
        0x000000ff, 0xccb78fff, 0xa4a494ff, 0x8c846cff,    0x9c845cff, 0x9c7c54ff, 0x94744cff, 0x8c7454ff,
        0x846c54ff, 0x7b6747ff, 0x74644cff, 0x6c6454ff,    0xeacf09ff, 0xf0a705ff, 0xfe7f31ff, 0xfe5027ff,
        0xd10404ff, 0x9d1a1aff, 0x645c4cff, 0x6c5c44ff,    0x64543cff, 0x5c543cff, 0x545444ff, 0x4c5444ff,
        0x4c4c3cff, 0x544c3cff, 0x544c34ff, 0x5c4c34ff,    0x644c2cff, 0x64542cff, 0x6c5434ff, 0x745c34ff,
        0x7c542cff, 0x84542cff, 0x8c5c2cff, 0x8c5424ff,    0x9c6434ff, 0xa46c3cff, 0xb4743cff, 0xbc742cff,
        0xc47c34ff, 0xbc844cff, 0xbc8c54ff, 0xb48454ff,    0xac7c4cff, 0xcc8c4cff, 0xe88f37ff, 0xf49c54ff,
        0xcc7414ff, 0xdc6c04ff, 0xbc640cff, 0xac5c0cff,    0xac6c2cff, 0x8c6c44ff, 0x846444ff, 0x7c5c44ff,
        0x6c4c2cff, 0x644424ff, 0x5c442cff, 0x54442cff,    0x4c3c2cff, 0x443c2cff, 0x3c3c2cff, 0x3c3c34ff,
        0x343c34ff, 0x2c3434ff, 0x34342cff, 0x3d2e2eff,    0x3c2c24ff, 0x3c3424ff, 0x443424ff, 0x4c341cff,
        0x5c3c24ff, 0x643c1cff, 0x5c5a20ff, 0x444424ff,    0x444434ff, 0x24342cff, 0x242c2cff, 0x2c2424ff,
        0x2c2c1cff, 0x34241cff, 0x1c1c1cff, 0x14140cff,    0x0c0c0cff, 0x060605ff, 0x707522ff, 0x849324ff,
        0x94ac24ff, 0xa7c921ff, 0xb4dc24ff, 0xd4fc2cff,    0x041404ff, 0x0c1c04ff, 0x101104ff, 0x142c04ff,
        0x1c3404ff, 0x143414ff, 0x143c14ff, 0x144414ff,    0x144c14ff, 0x145414ff, 0x145c14ff, 0x0c4c0cff,
        0x0c440cff, 0x0c3c0cff, 0x04540cff, 0x2c4c0cff,    0x2c440cff, 0x344c0cff, 0x34540cff, 0x44640cff,
        0x4c740cff, 0x547c14ff, 0x548414ff, 0x5c9414ff,    0x649c14ff, 0x6cb41cff, 0x7cd41cff, 0x7c8444ff,
        0x6c6c3cff, 0x8c0000ff, 0x5b2b10ff, 0x201911ff,    0x2c1c14ff, 0x361f07ff, 0x3f2910ff, 0x463415ff,
        0xfbee9aff, 0xeecc81ff, 0xd3c7a5ff, 0xcfcab4ff,    0xc9baa3ff, 0xb7b099ff, 0xaca48cff, 0x9a9897ff,
        0x88949bff, 0x8c8c8cff, 0x7d848aff, 0x76716dff,    0x747c65ff, 0x766a57ff, 0x81693aff, 0x946c2cff,
        0x978872ff, 0x9f947aff, 0xbe9861ff, 0xeab456ff,    0xfc641cff, 0x666b72ff, 0xa4a4a4ff, 0xb4b4b4ff,
        0xb9bdc1ff, 0xccd0cfff, 0xe4d4c4ff, 0xc0321bff,    0x7d4e22ff, 0x605434ff, 0x595856ff, 0x2c1c2cff,
        0xff4155ff, 0xc7000eff, 0x88000dff, 0x68000aff,    0x500008ff, 0x380006ff, 0x44c1fcff, 0x0680c1ff,
        0x045b84ff, 0x044564ff, 0x04354cff, 0x042534ff,    0x41ff96ff, 0x00c760ff, 0x00883eff, 0x006830ff,
        0x005024ff, 0x003819ff, 0xffdd41ff, 0xc7aa00ff,    0x887100ff, 0x685600ff, 0x504200ff, 0x382e00ff,
        0xff8841ff, 0xc75100ff, 0x883400ff, 0x682800ff,    0x501e00ff, 0x381500ff, 0xff41c1ff, 0xc7007fff,
        0x88005aff, 0x680045ff, 0x500036ff, 0x380026ff,    0x4241ffff, 0x0700c7ff, 0x010088ff, 0x010068ff,
        0x000050ff, 0x000038ff, 0xa2a2a2ff, 0x616161ff,    0x3e3e3eff, 0x2b2b2bff, 0x1c1c1cff, 0x0b0b0bff,
        0xffffffff, 0xb1b1b1ff, 0x808080ff, 0x626262ff,    0x484848ff, 0x252525ff, 0x843c0cff, 0xd46414ff,
        0xfc9424ff, 0xfca424ff, 0xfcc434ff, 0xffe432ff,    0xfcfc3cff, 0xfcf46cff, 0xfcfc9cff, 0xe4c44cff,
        0x050583ff, 0x06066eff, 0x0202c4ff, 0x0202a0ff,    0x196c97ff, 0x115585ff, 0x10516dff, 0x1c80dcff,
        0x1770acff, 0x2476d1ff, 0x255698ff, 0x134572ff,    0x57350cff, 0x3e280dff, 0x31230cff, 0x040c3cff,
        0x0c1c64ff, 0x2c3cacff, 0x0c4cccff, 0x3c4cecff,    0x4c5ce4ff, 0x5c6cd4ff, 0x844cc4ff, 0x5414f4ff,
        0x1c84e4ff, 0x3474a4ff, 0x1c741cff, 0x1c9c1cff,    0x34d434ff, 0x44fc44ff, 0xfca4acff, 0xffffff00
    };
    /* clang-format on */

    // Buffer to which the image will be rendered
    uint8_t image[IMAGE_WIDTH * IMAGE_HEIGHT];

    // Team colours to use when rendering
    uint8_t team_color[6] = { 160, 161, 162, 163, 164, 165 };

///////////////////////////////////////////////////////////////////////////////
#ifdef WIN32

    /**
     * Makes a section of memory executable.
     */
    BOOL make_executable(void* data, uint32_t size) {
        DWORD old;
        return VirtualProtect(data, size, PAGE_EXECUTE_READWRITE, &old);
    }

#endif  //////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER

    /**
     * Calls the assembly code at the given memory location.
     */
    void call_code(void* code) {
        __asm {
            /* clang-format off */
            pushad
            mov esi, offset team_color  // 6-byte array containing the team colors
            mov edi, offset image       // image pointer
            mov edx, IMAGE_WIDTH        // stride
            call[code]
            popad
            /* clang-format on */
        }
    }
#endif  //////////////////////////////////////////////////////////////////////

    void* read_file(std::wstring filename, uint32_t* size) {

        FILE* fp = _wfopen(filename.c_str(), L"rb");
        if (!fp) {
            return NULL;
        }

        fseek(fp, 0L, SEEK_END);
        *size = (uint32_t) ftell(fp);
        fseek(fp, 0L, SEEK_SET);

        void* data = malloc(*size);
        if (!data) {
            fclose(fp);
            return NULL;
        }

        fread(data, *size, 1, fp);

        if (ferror(fp)) {
            free(data);
            data = NULL;
        }
        fclose(fp);

        return data;
    }

    /**
     * Clears our image buffer.
     */
    void reset_image(void) {
        // Fill with our transparent colour
        memset(image, 0xff, sizeof(image));
    }

    /**
     * Writes an image to disk.
     */
    int write_image(const char* filename, int w, int h) {

        // Open file for writing
        FILE* fp = fopen(filename, "wb");
        if (!fp) {
            perror(filename);
            return 0;
        }

        /*
         * Write using TGA format:
         * http://tfc.duke.free.fr/coding/tga_specs.pdf
         */

        fputc(0x00, fp);  // ID Length
        fputc(0x01, fp);  // Color map type
        fputc(0x01, fp);  // Image type (uncompressed, colour-mapped)

        // Color map specification
        fputc(0, fp);  // Index of first entry
        fputc(0x00, fp);
        fputc(0x00, fp);  // Number of entries (256)
        fputc(0x01, fp);
        fputc(32, fp);  // Entry size (32-bit RGBA)

        // Image specification
        fputc(0x00, fp);  // X-origin
        fputc(0x00, fp);
        fputc(0x00, fp);  // Y-origin
        fputc(0x00, fp);
        fputc((uint8_t) w, fp);  // Width
        fputc((uint8_t)(w >> 8), fp);
        fputc((uint8_t) h, fp);  // Height
        fputc((uint8_t)(h >> 8), fp);
        fputc(8, fp);  // Bits per pixel

        // Image descriptor byte
        // (8 = number of alpha bits, bit5: upper-left origin)
        fputc(8 | 1 << 5, fp);

        // Colour map data
        for (int i = 0; i < PALETTE_SIZE; ++i) {

            const uint32_t col = PALETTE[i];
            const uint8_t red = (uint8_t)((col & 0xFF000000) >> 24);
            const uint8_t green = (uint8_t)((col & 0x00FF0000) >> 16);
            const uint8_t blue = (uint8_t)((col & 0x0000FF00) >> 8);
            const uint8_t alpha = (uint8_t)(col & 0x000000FF);

            fputc(blue, fp);
            fputc(green, fp);
            fputc(red, fp);
            fputc(alpha, fp);
        }

        // Pixel data
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                const uint8_t index = image[x + y * IMAGE_WIDTH];
                fputc(index, fp);
            }
        }

        fclose(fp);
        return 1;
    }

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
    uint8_t* find_end(uint8_t* start, uint8_t* end, int* read_from_esi) {

        *read_from_esi = 0;

        while (start < end) {
            uint8_t inst = *start++;
            const int op_size = inst == 0x66;  // Operand size prefix
            if (op_size)
                inst = *start++;

            if ((inst & 0xf0) == 0x40) {
                // inc/dec r16/r32
            } else if ((inst & 0xf8) == 0xb0) {
                // MOV r8, imm8
                start++;  // imm8
            } else if ((inst & 0xf8) == 0xb8) {
                // MOV  r16/r32, imm16/imm32
                start += op_size ? 2 : 4;
            } else if (inst == 0x03) {
                // ADD     r16/32     r/m16/32
                start++;  // MODR/M
            } else if (inst == 0x83) {
                // ALUOP r32, imm8
                start++;  // MODR/M
                start++;  // imm8
            } else if (inst == 0x8a || inst == 0x8b) {
                // MOV     r16/32     r/m16/32
                const uint8_t modrm = *start++;  // MODR/M
                if (MODRM_MOD(modrm) == 1) {
                    start++;  // disp8
                }
                if (MODRM_MOD(modrm) != 0x03 /* && MODRM_RM(modrm) == 0x6*/) {
                    *read_from_esi = 1;
                }
            } else if (inst == 0x69) {
                // IMUL r16/32     r/m16/32 imm16/32
                start++;  // MODR/M
                start += op_size ? 2 : 4;
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
     * Rounds a number up to the nearest power of 2.
     *
     * See:
     * http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
     */
    int nextPowerOf2(int v) {

        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;

        return v;
    }

    /**
     * Extracts images from the given "IMAGES.DAT" file,
     * to the given output directory.
     */
    void extractImages(std::wstring inputFile, std::string outputDir) {

        // Read the file
        uint32_t size;
        uint8_t* data = (uint8_t*) read_file(inputFile, &size);
        if (!data) {
            throw std::runtime_error("Unable to open IMAGES.DAT\n");
        }

        // Make the code contained within "IMAGES.DAT" executable
        if (!make_executable(data, size)) {
            free(data);
            throw std::runtime_error("Failed to make memory executable\n");
        }

        int i = 0;
        uint8_t* start = data + HEADER_SIZE;
        uint8_t* end = data + size;

        // Extract the images!
        for (uint8_t* code = start; code < end; ++i) {

            // Figure out where the next function starts
            int read_from_esi;
            uint8_t* const code_end = find_end(code, end, &read_from_esi);

            // Draw the image with our desired team colour
            reset_image();
            call_code(code);

            // Figure out the image dimensions based on what was drawn
            int w = 0, h = 0;
            for (int y = 0; y < IMAGE_HEIGHT; ++y) {
                for (int x = 0; x < IMAGE_WIDTH; ++x) {
                    if (image[x + y * IMAGE_WIDTH] != 0xff) {
                        if (x > w) {
                            w = x + 1;
                        }
                        if (y > h) {
                            h = y + 1;
                        }
                    }
                }
            }

            // Ensure the image is not empty
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
                    "%s\\img_%04d_%08X.tga",
                    outputDir.c_str(), i, code - data);
            write_image(filename, w, h);

            // Jump to the next image
            code = code_end;
        }

        free(data);
    }

}  // namespace ImageExtractor
