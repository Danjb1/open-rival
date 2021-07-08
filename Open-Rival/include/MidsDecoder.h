#ifndef MIDS_DECODER_H
#define MIDS_DECODER_H

#include "MidiContainer.h"

/**
 * Decoder for the obscure RIFF/MIDS format.
 *
 * Based on the midi_processing library by Christopher Snowhill:
 * https://github.com/kode54/midi_processing/blob/master/midi_processor_mids.cpp
 *
 * Used with permission.
 */

namespace Rival {

#ifndef _countof
    template <typename T, size_t N>
    char (&_ArraySizeHelper(T (&array)[N]))[N];
#define _countof(array) (sizeof(_ArraySizeHelper(array)))
#endif

    class MidsDecoder {
    public:
        static const std::uint8_t end_of_track[2];
        static const std::uint8_t loop_start[11];
        static const std::uint8_t loop_end[9];

        static bool is_mids(std::vector<std::uint8_t> const& p_file);
        static bool process_mids(
                std::vector<std::uint8_t> const& p_file, midi_container& p_out);
    };

}  // namespace Rival

#endif  // MIDS_DECODER_H
