/**
 * Script to extract the Rival Realms images from "IMAGES.DAT".
 *
 * The images are not stored as data; instead the rendering routines are stored directly as assembly code.
 * We execute this code using a simple x86 emulator, which is more portable than trying to run the assembly directly
 * (albeit a tad slower).
 *
 * Thanks to Reddit user 0xa0000 for this solution.
 */

#include "image-extractor.h"

#include <cassert>
#include <cstdint>  // std::uint8_t
#include <iostream>
#include <stdexcept>  // std::runtime_error

#include "FileUtils.h"
#include "Image.h"
#include "MathUtils.h"
#include "Palette.h"
#include "setup-utils.h"

namespace Rival { namespace Setup {

// Largest image in "IMAGES.DAT" is 128 x 128
static constexpr int maxWidth = 128;
static constexpr int maxHeight = 128;

// Size of pixel data, in bytes
static constexpr int imageSize = maxWidth * maxHeight;

// 4 byte header: number of images in file
static constexpr int headerSize = 4;

// Team colors to use when rendering
// (these 6 colors correspond to a single team)
const constexpr int numTeamColors = 6;
static const std::uint8_t teamColors[numTeamColors] = { 160, 161, 162, 163, 164, 165 };

// Pixel value corresponding to transparency
static constexpr std::uint8_t transparentColor = 0xff;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// x86 Emulation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum Register
{
    AX,
    CX,
    DX,
    BX,
    SP,
    BP,
    SI,
    DI,
    NREGS
};

enum class OpSize : std::uint8_t
{
    SIZE8 = 1,
    SIZE16 = 2,
    SIZE32 = 4
};

class CpuEmu
{
public:
    // Addresses that we will read/write from/to.
    // The actual values we use here are not important since we know in advance that all writes will be to our image
    // buffer, and all reads will be from our team color array. So, all we really care when we see a read/write
    // operation about is the offset *from* these addresses.
    // We use some recognisable values here, just so that we can verify this assumption.
    static constexpr std::uint32_t image_base = 0x10000000;
    static constexpr std::uint32_t team_color_base = 0x20000000;

    explicit CpuEmu(const std::uint8_t* code, std::size_t code_size)
        : code_ { code }
        , code_size_ { code_size }
    {
    }

    bool done() const
    {
        return pc_ >= code_size_;
    }

    void extract_image(std::vector<std::uint8_t>& image_, int imageWidth, int imageHeight)
    {
        // Initialize registers to some arbitrary value.
        // This is not strictly necessary but should help catch any bugs that might arise from unexpected behavior
        // (e.g. when porting or trying to use the "emulator" with unsupported code).
        memset(regs_, 0xCC, sizeof(regs_));

        regs_[Register::DX] = imageWidth;       // stride
        regs_[Register::DI] = image_base;       // dest
        regs_[Register::SI] = team_color_base;  // team colors

        for (;;)
        {
            std::uint8_t inst = pc_u8();
            opsize_ = OpSize::SIZE32;
            if (inst == 0x66)
            {
                opsize_ = OpSize::SIZE16;
                inst = pc_u8();
            }

            if (inst == 0x03)
            {
                // add r16/32, r/m16/32
                modrm();
                write_reg(reg_, read_reg(reg_) + read_rm());
            }
            else if ((inst & 0xf0) == 0x40)
            {
                // inc/dec r16/32
                const int r = inst & 7;
                write_reg(r, read_reg(r) + (inst & 8 ? -1 : 1));
            }
            else if (inst == 0x69)
            {
                // imul r16/32, r/m16/32, imm16/32
                modrm();
                write_reg(reg_, read_rm() * imm());
            }
            else if (inst == 0x6b)
            {
                // imul r16/32, r/m16/32, imm8
                modrm();
                write_reg(reg_, read_rm() * imm8());
            }
            else if (inst == 0x83)
            {
                // aluop r/m16/32, imm8
                modrm();
                if (reg_ != 0)
                    halt();
                write_rm(read_rm() + imm8());
            }
            else if (inst == 0x8a)
            {
                // mov r8, r/m8
                modrm();
                opsize_ = OpSize::SIZE8;
                write_reg(reg_, read_rm());
            }
            else if (inst == 0x86)
            {
                // xchg r8, r/m8
                modrm();
                opsize_ = OpSize::SIZE8;
                auto tmp = read_reg(reg_);
                write_reg(reg_, read_rm());
                write_rm(tmp);
            }
            else if (inst == 0x8b)
            {
                // mov r16/32, r/m16/32
                modrm();
                write_reg(reg_, read_rm());
            }
            else if (inst == 0xaa)
            {
                // stosb
                opsize_ = OpSize::SIZE8;
                stos(image_, imageWidth * imageHeight);
            }
            else if (inst == 0xab)
            {
                // stosw/stosd
                stos(image_, imageWidth * imageHeight);
            }
            else if ((inst & 0xf0) == 0xb0)
            {
                // mov reg, imm
                if (!(inst & 8))
                    opsize_ = OpSize::SIZE8;
                write_reg(inst & 7, imm());
            }
            else if (inst == 0xc3)
            {
                // ret
                break;
            }
            else
            {
                halt();
            }
        }
    }

private:
    std::uint32_t regs_[Register::NREGS] = {};
    std::uint32_t pc_ = 0;
    const std::uint8_t* code_;
    std::size_t code_size_;
    OpSize opsize_ = OpSize::SIZE32;
    // modrm
    std::uint8_t mod_ {}, reg_ {}, rm_ {};
    std::uint32_t addr_ {};

    [[noreturn]] void halt()
    {
        throw std::runtime_error { "Fatal error, halting" };
    }

    // Handles all writes to the image
    void stos(std::vector<std::uint8_t>& image_, int size)
    {
        auto& edi = regs_[Register::DI];
        std::uint32_t sz = (std::uint32_t) opsize_;
        if (edi < image_base || edi + sz > image_base + size)
        {
            halt();
        }
        auto val = regs_[Register::AX];
        while (sz--)
        {
            image_[edi++ - image_base] = (std::uint8_t) val;
            val >>= 8;
        }
    }

    int32_t read_reg(int r)
    {
        assert(r >= 0 && r < Register::NREGS);
        if (opsize_ == OpSize::SIZE8)
        {
            if (r & 4)
            {
                return (int8_t) ((regs_[r & 3] >> 8) & 0xff);
            }
            else
            {
                return (int8_t) (regs_[r & 3] & 0xff);
            }
        }
        else if (opsize_ == OpSize::SIZE16)
        {
            return (int16_t) (regs_[r] & 0xffff);
        }
        else
        {
            return regs_[r];
        }
    }

    // Handles all reads of team colors
    int32_t read_rm()
    {
        if (mod_ == 0b11)
        {
            return read_reg(rm_);
        }

        const std::uint32_t sz = (std::uint32_t) opsize_;
        if (addr_ < team_color_base || addr_ + sz > team_color_base + numTeamColors)
        {
            halt();
        }

        auto ofs = addr_ - team_color_base;

        std::uint32_t res = 0;
        for (std::uint32_t i = 0; i < sz; ++i)
        {
            res |= teamColors[ofs + i] << (8 * i);
        }
        return res;
    }

    void write_reg(int r, std::uint32_t val)
    {
        assert(r >= 0 && r < Register::NREGS);
        if (opsize_ == OpSize::SIZE8)
        {
            if (r & 4)
            {
                regs_[r & 3] = (regs_[r & 3] & 0xffff00ff) | ((val & 0xff) << 8);
            }
            else
            {
                regs_[r] = (regs_[r] & 0xffffff00) | (val & 0xff);
            }
        }
        else if (opsize_ == OpSize::SIZE16)
        {
            regs_[r] = (regs_[r] & 0xffff0000) | (val & 0xffff);
        }
        else
        {
            regs_[r] = val;
        }
    }

    void write_rm(std::uint32_t val)
    {
        if (mod_ != 0b11)
        {
            halt();
        }
        write_reg(rm_, val);
    }

    int32_t imm8()
    {
        return (int8_t) pc_u8();
    }

    int32_t imm32()
    {
        return (int32_t) pc_u32();
    }

    int32_t imm()
    {
        if (opsize_ == OpSize::SIZE8)
        {
            return imm8();
        }
        else if (opsize_ == OpSize::SIZE16)
        {
            return (int16_t) pc_u16();
        }
        else
        {
            return imm32();
        }
    }

    void modrm()
    {
        const std::uint8_t mrm = pc_u8();

        mod_ = (mrm >> 6) & 0x3;
        reg_ = (mrm >> 3) & 0x7;
        rm_ = (mrm >> 0) & 0x7;

        if (mod_ != 0b11)
        {
            if (rm_ == 0b100)
            {
                halt();
            }
            addr_ = regs_[rm_];
            if (mod_ == 0b01)
            {
                addr_ += imm8();
            }
            else if (mod_ == 0b10)
            {
                addr_ += imm32();
            }
        }
    }

    std::uint8_t pc_u8()
    {
        assert(pc_ < code_size_);
        return code_[pc_++];
    }

    uint16_t pc_u16()
    {
        uint16_t res = pc_u8();
        res |= pc_u8() << 8;
        return res;
    }

    std::uint32_t pc_u32()
    {
        std::uint32_t res = pc_u8();
        res |= pc_u8() << 8;
        res |= pc_u8() << 16;
        res |= pc_u8() << 24;
        return res;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Image Extraction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Extracts images from the given "IMAGES.DAT" file, to the given output directory.
 */
void extractImages(std::string inputFile, std::string outputDir)
{
    // Read the file
    auto imagesDatFile = FileUtils::readBinaryFile(inputFile);

    // Execute the instructions using our x86 emulator
    std::uint8_t* codeStart = imagesDatFile.data() + headerSize;
    std::size_t codeSize = imagesDatFile.size() - headerSize;
    CpuEmu emu { codeStart, codeSize };

    for (int i = 0; !emu.done(); ++i)
    {
        std::vector<std::uint8_t> imageData(imageSize, transparentColor);
        emu.extract_image(imageData, maxWidth, maxHeight);

        // Figure out the image dimensions based on what was drawn
        int w = 0;
        int h = 0;
        for (int y = 0; y < maxHeight; ++y)
        {
            for (int x = 0; x < maxWidth; ++x)
            {
                if (imageData[x + y * maxWidth] != transparentColor)
                {
                    if (x > w)
                    {
                        w = x + 1;
                    }
                    if (y > h)
                    {
                        h = y + 1;
                    }
                }
            }
        }

        // Ensure the image is not empty (should never happen)
        if (w == 0)
        {
            w = 1;
        }
        if (h == 0)
        {
            h = 1;
        }

        // Ensure the image is square
        if (w > h)
        {
            h = w;
        }
        else if (h > w)
        {
            w = h;
        }

        // Round image dimensions to nearest power of 2
        w = h = MathUtils::nextPowerOf2(w);

        // Save the rendered image to disk
        std::string filename = outputDir + "\\img_" + zeroPad(i, 4) + ".tga";
        ImageProperties props;
        props.stride = maxWidth;
        Image image = Image::createByMove(w, h, std::move(imageData), props);
        writeImage(image, Palette::paletteGame, filename);

        std::cout << "Saving image " << filename << "\n";
    }
}

}}  // namespace Rival::Setup
