#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Rival {

struct WavHeader
{
    // RIFF Chunk Descriptor
    std::uint8_t riff[4];     // RIFF Header Magic header
    std::uint32_t chunkSize;  // RIFF Chunk Size
    std::uint8_t wave[4];     // WAVE Header

    // "fmt" sub-chunk
    std::uint8_t fmt[4];          // FMT header
    std::uint32_t subchunk1Size;  // Size of the fmt chunk
    std::uint16_t audioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    std::uint16_t numChannels;    // Number of channels 1=Mono 2=Stereo
    std::uint32_t samplesPerSec;  // Sampling frequency in Hz
    std::uint32_t bytesPerSec;    // bytes per second
    std::uint16_t blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    std::uint16_t bitsPerSample;  // Number of bits per sample

    // "data" sub-chunk
    std::uint8_t subchunk2ID[4];  // "data" string
    std::uint32_t subchunk2Size;  // Sampled data length
};

class WaveFile
{

public:
    WavHeader header;
    std::vector<std::uint8_t> soundData;
    std::string filename;

    WaveFile(std::string filename);
};

}  // namespace Rival
