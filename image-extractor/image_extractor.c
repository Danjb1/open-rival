#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define IMAGE_WIDTH  2048
#define IMAGE_HEIGHT 2048

const uint32_t PALETTE[256] = {
    0x000000,
    0xccb78f,
    0xa4a494,
    0x8c846c,
    0x9c845c,
    0x9c7c54,
    0x94744c,
    0x8c7454,
    0x846c54,
    0x7b6747,
    0x74644c,
    0x6c6454,
    0xeacf09,
    0xf0a705,
    0xfe7f31,
    0xfe5027,
    0xd10404,
    0x9d1a1a,
    0x645c4c,
    0x6c5c44,
    0x64543c,
    0x5c543c,
    0x545444,
    0x4c5444,
    0x4c4c3c,
    0x544c3c,
    0x544c34,
    0x5c4c34,
    0x644c2c,
    0x64542c,
    0x6c5434,
    0x745c34,
    0x7c542c,
    0x84542c,
    0x8c5c2c,
    0x8c5424,
    0x9c6434,
    0xa46c3c,
    0xb4743c,
    0xbc742c,
    0xc47c34,
    0xbc844c,
    0xbc8c54,
    0xb48454,
    0xac7c4c,
    0xcc8c4c,
    0xe88f37,
    0xf49c54,
    0xcc7414,
    0xdc6c04,
    0xbc640c,
    0xac5c0c,
    0xac6c2c,
    0x8c6c44,
    0x846444,
    0x7c5c44,
    0x6c4c2c,
    0x644424,
    0x5c442c,
    0x54442c,
    0x4c3c2c,
    0x443c2c,
    0x3c3c2c,
    0x3c3c34,
    0x343c34,
    0x2c3434,
    0x34342c,
    0x3d2e2e,
    0x3c2c24,
    0x3c3424,
    0x443424,
    0x4c341c,
    0x5c3c24,
    0x643c1c,
    0x5c5a20,
    0x444424,
    0x444434,
    0x24342c,
    0x242c2c,
    0x2c2424,
    0x2c2c1c,
    0x34241c,
    0x1c1c1c,
    0x14140c,
    0x0c0c0c,
    0x060605,
    0x707522,
    0x849324,
    0x94ac24,
    0xa7c921,
    0xb4dc24,
    0xd4fc2c,
    0x041404,
    0x0c1c04,
    0x101104,
    0x142c04,
    0x1c3404,
    0x143414,
    0x143c14,
    0x144414,
    0x144c14,
    0x145414,
    0x145c14,
    0x0c4c0c,
    0x0c440c,
    0x0c3c0c,
    0x04540c,
    0x2c4c0c,
    0x2c440c,
    0x344c0c,
    0x34540c,
    0x44640c,
    0x4c740c,
    0x547c14,
    0x548414,
    0x5c9414,
    0x649c14,
    0x6cb41c,
    0x7cd41c,
    0x7c8444,
    0x6c6c3c,
    0x8c0000,
    0x5b2b10,
    0x201911,
    0x2c1c14,
    0x361f07,
    0x3f2910,
    0x463415,
    0xfbee9a,
    0xeecc81,
    0xd3c7a5,
    0xcfcab4,
    0xc9baa3,
    0xb7b099,
    0xaca48c,
    0x9a9897,
    0x88949b,
    0x8c8c8c,
    0x7d848a,
    0x76716d,
    0x747c65,
    0x766a57,
    0x81693a,
    0x946c2c,
    0x978872,
    0x9f947a,
    0xbe9861,
    0xeab456,
    0xfc641c,
    0x666b72,
    0xa4a4a4,
    0xb4b4b4,
    0xb9bdc1,
    0xccd0cf,
    0xe4d4c4,
    0xc0321b,
    0x7d4e22,
    0x605434,
    0x595856,
    0x2c1c2c,
    0xff4155,
    0xc7000e,
    0x88000d,
    0x68000a,
    0x500008,
    0x380006,
    0x44c1fc,
    0x0680c1,
    0x045b84,
    0x044564,
    0x04354c,
    0x042534,
    0x41ff96,
    0x00c760,
    0x00883e,
    0x006830,
    0x005024,
    0x003819,
    0xffdd41,
    0xc7aa00,
    0x887100,
    0x685600,
    0x504200,
    0x382e00,
    0xff8841,
    0xc75100,
    0x883400,
    0x682800,
    0x501e00,
    0x381500,
    0xff41c1,
    0xc7007f,
    0x88005a,
    0x680045,
    0x500036,
    0x380026,
    0x4241ff,
    0x0700c7,
    0x010088,
    0x010068,
    0x000050,
    0x000038,
    0xa2a2a2,
    0x616161,
    0x3e3e3e,
    0x2b2b2b,
    0x1c1c1c,
    0x0b0b0b,
    0xffffff,
    0xb1b1b1,
    0x808080,
    0x626262,
    0x484848,
    0x252525,
    0x843c0c,
    0xd46414,
    0xfc9424,
    0xfca424,
    0xfcc434,
    0xffe432,
    0xfcfc3c,
    0xfcf46c,
    0xfcfc9c,
    0xe4c44c,
    0x050583,
    0x06066e,
    0x0202c4,
    0x0202a0,
    0x196c97,
    0x115585,
    0x10516d,
    0x1c80dc,
    0x1770ac,
    0x2476d1,
    0x255698,
    0x134572,
    0x57350c,
    0x3e280d,
    0x31230c,
    0x040c3c,
    0x0c1c64,
    0x2c3cac,
    0x0c4ccc,
    0x3c4cec,
    0x4c5ce4,
    0x5c6cd4,
    0x844cc4,
    0x5414f4,
    0x1c84e4,
    0x3474a4,
    0x1c741c,
    0x1c9c1c,
    0x34d434,
    0x44fc44,
    0xfca4ac,
    0xffffff,
};

uint8_t image[IMAGE_WIDTH*IMAGE_HEIGHT];

#ifdef WIN32
#include <windows.h>
BOOL make_executable(void* data, uint32_t size) {
    DWORD old;
    return VirtualProtect(data, size, PAGE_EXECUTE_READWRITE, &old);
}
#endif

#ifdef _MSC_VER
void call_code(void* code) {
    __asm {
        pushad                      // code seems to expect some register to be preserved?
        mov     edi, offset image   // image pointer
        mov     edx, IMAGE_WIDTH    // stride
        call    [code]
        popad
    }
}
#endif

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

int write_image(const char* filename, int w, int h) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror(filename);
        return 0;
    }
    fprintf(fp, "P6\n%d %d 255\n", w, h);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            const uint8_t index = image[x+y*IMAGE_WIDTH];
            const uint32_t col = PALETTE[index];
            
            const uint8_t red   = (col & 0x00FF0000) >> 16;
            const uint8_t green = (col & 0x0000FF00) >> 8;
            const uint8_t blue  = (col & 0x000000FF);
            
            fputc(red, fp);
            fputc(green, fp);
            fputc(blue, fp);
        }
    }
    fclose(fp);
    return 1;
}

//
// Returns one past next 'ret' (0xC3) instruction
//
// Recognizes only the subset of x86 actually used and takes
// as many shortcuts as possible
uint8_t* find_end(uint8_t* start, uint8_t* end) {
    while (start < end) {
        uint8_t inst = *start++;
        const int op_size = inst == 0x66; // Operand size prefix
        if (op_size) inst = *start++;

        if ((inst&0xf0) == 0x40) {
            // inc/dec r16/r32
        } else if ((inst&0xf8) == 0xb0) {
            // MOV r8, imm8
            start++; // imm8
        } else if ((inst&0xf8) == 0xb8) {
            // MOV  r16/r32, imm16/imm32
            start += op_size ? 2 : 4;
        } else if (inst == 0x03) {
            // ADD 	r16/32 	r/m16/32
            start++; // MODR/M
        } else if (inst == 0x83) {
            // ALUOP r32, imm8
            start++; // MODR/M
            start++; // imm8
        } else if (inst == 0x8a || inst == 0x8b) {
         	// MOV 	r16/32 	r/m16/32
            const uint8_t modrm = *start++; // MODR/M
            if (modrm&0x40) start++;
        } else if (inst == 0x69) {
            // IMUL r16/32 	r/m16/32 imm16/32
            start++; // MODR/M
            start += op_size ? 2 : 4;
        } else if (inst == 0x6b) {
            // IMUL r16/32 r/m16/32 imm8
            start++; // MODR/M
            start++; // imm8
        } else if (inst == 0x86) {
            // XCHG r8, r/m8
            start++; // MODR/M
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

int main() {
    uint32_t size;
    uint8_t* data = read_file("images.dat", &size);
    if (!data) {
        return 1;
    }
	if (!make_executable(data, size)) {
		free(data);
		return 2;
	}
    int cnt = 0, maxw = 0, maxh = 0;
    // 4 byte header?
    for (uint8_t* code = data + 4; code < data + size; ++cnt) {
        memset(image, 0, sizeof(image));
        call_code(code);
        int w=0,h=0;
        for (int y = 0; y < IMAGE_HEIGHT; ++y) {
            for (int x = 0; x < IMAGE_WIDTH; ++x) {
                if (image[x+y*IMAGE_WIDTH]) {
                    if (x > w) w = x + 1;
                    if (y > h) h = y + 1;
                }
            }
        }
        printf("%08X: %dx%d\n", code-data-4, w, h);
        if (w > maxw) maxw = w;
        if (h > maxh) maxh = h;

        char filename[32];
        snprintf(filename, sizeof(filename), "images/img%06X.ppm", code-data);
        write_image(filename, w, h);

        code = find_end(code, data + size);
    }
    printf("Max size: %d x %d\n", maxw, maxh);
    free(data);
    return 0;
}