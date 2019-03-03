/**
 * Script to extract the Rival Realms images from "IMAGES.DAT".
 *
 * @author 0xa0000 - All the hard work (this guy is a wizard!).
 * @author Danjb - Tweaks and formatting changes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Largest image in "IMAGES.DAT" is 128 x 128
const int IMAGE_WIDTH = 128;
const int IMAGE_HEIGHT = 128;

// 4 byte header: number of images in file
const int HEADER_SIZE = 4;

// The game's colour palette
const uint32_t PALETTE[256] = {
	0x000000, 0xccb78f, 0xa4a494, 0x8c846c,	0x9c845c, 0x9c7c54, 0x94744c, 0x8c7454,
	0x846c54, 0x7b6747, 0x74644c, 0x6c6454,	0xeacf09, 0xf0a705, 0xfe7f31, 0xfe5027,
	0xd10404, 0x9d1a1a, 0x645c4c, 0x6c5c44,	0x64543c, 0x5c543c, 0x545444, 0x4c5444,
	0x4c4c3c, 0x544c3c, 0x544c34, 0x5c4c34,	0x644c2c, 0x64542c, 0x6c5434, 0x745c34,
	0x7c542c, 0x84542c, 0x8c5c2c, 0x8c5424,	0x9c6434, 0xa46c3c, 0xb4743c, 0xbc742c,
	0xc47c34, 0xbc844c, 0xbc8c54, 0xb48454,	0xac7c4c, 0xcc8c4c, 0xe88f37, 0xf49c54,
	0xcc7414, 0xdc6c04, 0xbc640c, 0xac5c0c,	0xac6c2c, 0x8c6c44, 0x846444, 0x7c5c44,
	0x6c4c2c, 0x644424, 0x5c442c, 0x54442c,	0x4c3c2c, 0x443c2c, 0x3c3c2c, 0x3c3c34,
	0x343c34, 0x2c3434, 0x34342c, 0x3d2e2e,	0x3c2c24, 0x3c3424, 0x443424, 0x4c341c,
	0x5c3c24, 0x643c1c, 0x5c5a20, 0x444424,	0x444434, 0x24342c, 0x242c2c, 0x2c2424,
	0x2c2c1c, 0x34241c, 0x1c1c1c, 0x14140c,	0x0c0c0c, 0x060605, 0x707522, 0x849324,
	0x94ac24, 0xa7c921, 0xb4dc24, 0xd4fc2c,	0x041404, 0x0c1c04, 0x101104, 0x142c04,
	0x1c3404, 0x143414, 0x143c14, 0x144414,	0x144c14, 0x145414, 0x145c14, 0x0c4c0c,
	0x0c440c, 0x0c3c0c, 0x04540c, 0x2c4c0c,	0x2c440c, 0x344c0c, 0x34540c, 0x44640c,
	0x4c740c, 0x547c14, 0x548414, 0x5c9414,	0x649c14, 0x6cb41c, 0x7cd41c, 0x7c8444,
	0x6c6c3c, 0x8c0000, 0x5b2b10, 0x201911,	0x2c1c14, 0x361f07, 0x3f2910, 0x463415,
	0xfbee9a, 0xeecc81, 0xd3c7a5, 0xcfcab4,	0xc9baa3, 0xb7b099, 0xaca48c, 0x9a9897,
	0x88949b, 0x8c8c8c, 0x7d848a, 0x76716d,	0x747c65, 0x766a57, 0x81693a, 0x946c2c,
	0x978872, 0x9f947a, 0xbe9861, 0xeab456,	0xfc641c, 0x666b72, 0xa4a4a4, 0xb4b4b4,
	0xb9bdc1, 0xccd0cf, 0xe4d4c4, 0xc0321b,	0x7d4e22, 0x605434, 0x595856, 0x2c1c2c,
	0xff4155, 0xc7000e, 0x88000d, 0x68000a,	0x500008, 0x380006, 0x44c1fc, 0x0680c1,
	0x045b84, 0x044564, 0x04354c, 0x042534,	0x41ff96, 0x00c760, 0x00883e, 0x006830,
	0x005024, 0x003819, 0xffdd41, 0xc7aa00,	0x887100, 0x685600, 0x504200, 0x382e00,
	0xff8841, 0xc75100, 0x883400, 0x682800,	0x501e00, 0x381500, 0xff41c1, 0xc7007f,
	0x88005a, 0x680045, 0x500036, 0x380026,	0x4241ff, 0x0700c7, 0x010088, 0x010068,
	0x000050, 0x000038, 0xa2a2a2, 0x616161,	0x3e3e3e, 0x2b2b2b, 0x1c1c1c, 0x0b0b0b,
	0xffffff, 0xb1b1b1, 0x808080, 0x626262,	0x484848, 0x252525, 0x843c0c, 0xd46414,
	0xfc9424, 0xfca424, 0xfcc434, 0xffe432,	0xfcfc3c, 0xfcf46c, 0xfcfc9c, 0xe4c44c,
	0x050583, 0x06066e, 0x0202c4, 0x0202a0,	0x196c97, 0x115585, 0x10516d, 0x1c80dc,
	0x1770ac, 0x2476d1, 0x255698, 0x134572,	0x57350c, 0x3e280d, 0x31230c, 0x040c3c,
	0x0c1c64, 0x2c3cac, 0x0c4ccc, 0x3c4cec,	0x4c5ce4, 0x5c6cd4, 0x844cc4, 0x5414f4,
	0x1c84e4, 0x3474a4, 0x1c741c, 0x1c9c1c,	0x34d434, 0x44fc44, 0xfca4ac, 0xffffff,
};

// Buffer to which the image will be rendered
uint8_t image[IMAGE_WIDTH * IMAGE_HEIGHT];

// Team colours to use when rendering
uint8_t team_color[6] = { 160, 161, 162, 163, 164, 165 };

///////////////////////////////////////////////////////////////////////////////
#ifdef WIN32

#include <windows.h>

/**
 * Makes a section of memory executable.
 */
BOOL make_executable(void* data, uint32_t size) {
	DWORD old;
	return VirtualProtect(data, size, PAGE_EXECUTE_READWRITE, &old);
}

#endif ////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER

/**
 * Calls the assembly code at the given memory location.
 */
void call_code(void* code) {
	__asm {
		pushad
		mov esi, offset team_color  // 6-byte array containing the team colors
		mov edi, offset image       // image pointer
		mov edx, IMAGE_WIDTH        // stride
		call[code]
		popad
	}
}
#endif ////////////////////////////////////////////////////////////////////////

/**
 * Reads a file to memory.
 */
void* read_file(const char* filename, uint32_t* size) {

	FILE* fp = fopen(filename, "rb");
	if (!fp) {
		perror(filename);
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);
	*size = (uint32_t)ftell(fp);
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
	// Color index 255 (0xff) is never written by the code (in "IMAGES.DAT")
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

	fputc(0x00, fp); // ID Length
	fputc(0x00, fp); // Color map type
	fputc(0x02, fp); // Image type (uncompressed true-color)

	// Color map specification
	fputc(0x00, fp);
	fputc(0x00, fp);
	fputc(0x00, fp);
	fputc(0x00, fp);
	fputc(0x00, fp);

	// Image specification
	fputc(0x00, fp); // X-origin
	fputc(0x00, fp);
	fputc(0x00, fp); // Y-origin
	fputc(0x00, fp);

	fputc((uint8_t)w, fp); // width
	fputc((uint8_t)(w >> 8), fp);
	fputc((uint8_t)h, fp); // height
	fputc((uint8_t)(h >> 8), fp);
	fputc(32, fp); // bits per pixel
	fputc(8 | 1 << 5, fp); // image descriptor byte (8 = number of alpha bits) bit5: image origin is upper left corner

	// Pixel data
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {

			const uint8_t index = image[x + y * IMAGE_WIDTH];

			// Coloured pixels
			if (index != 0xff) {
				const uint32_t col = PALETTE[index];
				const uint8_t red = (uint8_t)((col & 0x00FF0000) >> 16);
				const uint8_t green = (uint8_t)((col & 0x0000FF00) >> 8);
				const uint8_t blue = (uint8_t)(col & 0x000000FF);
				fputc(blue, fp);
				fputc(green, fp);
				fputc(red, fp);
				fputc(0xff, fp); // Alpha

			// Transparent pixels
			} else {
				fputc(0, fp);
				fputc(0, fp);
				fputc(0, fp);
				fputc(0, fp);
			}
		}
	}

	fclose(fp);
	return 1;
}

#define MODRM_MOD(modrm) (((modrm) >> 6) & 0x3)
#define MODRM_REG(modrm) (((modrm) >> 3) & 0x7)
#define MODRM_RM(modrm)  (((modrm) >> 0) & 0x7)

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
		const int op_size = inst == 0x66; // Operand size prefix
		if (op_size) inst = *start++;

		if ((inst & 0xf0) == 0x40) {
			// inc/dec r16/r32
		}
		else if ((inst & 0xf8) == 0xb0) {
			// MOV r8, imm8
			start++; // imm8
		}
		else if ((inst & 0xf8) == 0xb8) {
			// MOV  r16/r32, imm16/imm32
			start += op_size ? 2 : 4;
		}
		else if (inst == 0x03) {
			// ADD 	r16/32 	r/m16/32
			start++; // MODR/M
		}
		else if (inst == 0x83) {
			// ALUOP r32, imm8
			start++; // MODR/M
			start++; // imm8
		}
		else if (inst == 0x8a || inst == 0x8b) {
			// MOV 	r16/32 	r/m16/32
			const uint8_t modrm = *start++; // MODR/M
			if (MODRM_MOD(modrm) == 1) {
				start++; // disp8
			}
			if (MODRM_MOD(modrm) != 0x03/* && MODRM_RM(modrm) == 0x6*/) {
				*read_from_esi = 1;
			}
		}
		else if (inst == 0x69) {
			// IMUL r16/32 	r/m16/32 imm16/32
			start++; // MODR/M
			start += op_size ? 2 : 4;
		}
		else if (inst == 0x6b) {
			// IMUL r16/32 r/m16/32 imm8
			start++; // MODR/M
			start++; // imm8
		}
		else if (inst == 0x86) {
			// XCHG r8, r/m8
			start++; // MODR/M
		}
		else if (inst == 0xaa) {
			// stosb
		}
		else if (inst == 0xab) {
			// stosw/stosd
		}
		else if (inst == 0xc3) {
			// ret!
			break;
		}
		else {
			printf("Unhandled instruction %02X\n", inst);
			abort();
		}
	}

	return start;
}

/**
 * Entry point for the application.
 */
int main() {

	// Read the file
	uint32_t size;
	uint8_t* data = (uint8_t*) read_file("IMAGES.DAT", &size);
	if (!data) {
		printf("Error reading IMAGES.DAT");
		return 1;
	}

	// Make the code contained within "IMAGES.DAT" executable
	if (!make_executable(data, size)) {
		printf("Error making memory executable");
		free(data);
		return 2;
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
					if (x > w) w = x + 1;
					if (y > h) h = y + 1;
				}
			}
		}

		printf("images/img_%04d_%08X.tga: %dx%d\n", i, code - data, w, h);

		// Save the rendered image to disk
		char filename[32];
		snprintf(filename, sizeof(filename), "images/img_%04d_%08X.tga", i, code - data);
		write_image(filename, w, h);

		// Jump to the next image
		code = code_end;
	}

	free(data);
	return 0;
}
