#include "pch.h"

#include "MidsDecoder.h"

namespace Rival {

const std::uint8_t MidsDecoder::end_of_track[2] = { 0xFF, 0x2F };
const std::uint8_t MidsDecoder::loop_start[11] = { 0xFF, 0x06, 'l', 'o', 'o', 'p', 'S', 't', 'a', 'r', 't' };
const std::uint8_t MidsDecoder::loop_end[9] = { 0xFF, 0x06, 'l', 'o', 'o', 'p', 'E', 'n', 'd' };

bool MidsDecoder::is_mids(std::vector<std::uint8_t> const& p_file)
{
    if (p_file.size() < 8)
        return false;
    if (p_file[0] != 'R' || p_file[1] != 'I' || p_file[2] != 'F' || p_file[3] != 'F')
        return false;
    std::uint32_t size = p_file[4] | (p_file[5] << 8) | (p_file[6] << 16) | (p_file[7] << 24);
    if (size < 8 || (p_file.size() < size + 8))
        return false;
    if (p_file[8] != 'M' || p_file[9] != 'I' || p_file[10] != 'D' || p_file[11] != 'S' || p_file[12] != 'f'
        || p_file[13] != 'm' || p_file[14] != 't' || p_file[15] != ' ')
        return false;
    return true;
}

bool MidsDecoder::process_mids(std::vector<std::uint8_t> const& p_file, midi_container& p_out)
{
    if (p_file.size() < 20)
        return false;
    std::vector<std::uint8_t>::const_iterator it = p_file.begin() + 16;
    std::vector<std::uint8_t>::const_iterator end = p_file.end();

    std::uint32_t fmt_size = it[0] | (it[1] << 8) | (it[2] << 16) | (it[3] << 24);
    it += 4;
    if ((unsigned long) (end - it) < fmt_size)
        return false;

    std::uint32_t time_format = 1;
    /*uint32_t max_buffer = 0;*/
    std::uint32_t flags = 0;

    if (fmt_size >= 4)
    {
        time_format = it[0] | (it[1] << 8) | (it[2] << 16) | (it[3] << 24);
        it += 4;
        fmt_size -= 4;
        if (!time_format)  // dtx == 0, will cause division by zero on tempo
                           // calculations
            return false;
    }
    if (fmt_size >= 4)
    {
        /*max_buffer = it[ 0 ] | ( it[ 1 ] << 8 ) | ( it[ 2 ] << 16 ) | ( it[ 3
         * ] << 24 );*/
        it += 4;
        fmt_size -= 4;
    }
    if (fmt_size >= 4)
    {
        flags = it[0] | (it[1] << 8) | (it[2] << 16) | (it[3] << 24);
        it += 4;
        fmt_size -= 4;
    }

    it += fmt_size;
    if (it == end)
        return false;
    if (fmt_size & 1)
        ++it;

    p_out.initialize(0, time_format);

    if (end - it < 4)
        return false;
    if (it[0] != 'd' || it[1] != 'a' || it[2] != 't' || it[3] != 'a')
        return false; /*throw exception_io_data( "MIDS missing RIFF data chunk"
                     );*/

    it += 4;

    {
        midi_track track;
        track.add_event(midi_event(0, midi_event::extended, 0, end_of_track, _countof(end_of_track)));
        p_out.add_track(track);
    }

    if (end - it < 4)
        return false;
    std::uint32_t data_size = it[0] | (it[1] << 8) | (it[2] << 16) | (it[3] << 24);
    it += 4;

    std::vector<std::uint8_t>::const_iterator body_end = it + data_size;

    if (body_end - it < 4)
        return false;
    std::uint32_t segment_count = it[0] | (it[1] << 8) | (it[2] << 16) | (it[3] << 24);
    it += 4;

    bool is_eight_byte = !!(flags & 1);

    midi_track track;

    unsigned current_timestamp = 0;

    for (unsigned i = 0; i < segment_count; ++i)
    {
        if (end - it < 12)
            return false;
        it += 4;
        std::uint32_t segment_size = it[0] | (it[1] << 8) | (it[2] << 16) | (it[3] << 24);
        it += 4;
        std::vector<std::uint8_t>::const_iterator segment_end = it + segment_size;
        while (it != segment_end && it != body_end)
        {
            if (segment_end - it < 4)
                return false;
            std::uint32_t delta = it[0] | (it[1] << 8) | (it[2] << 16) | (it[3] << 24);
            it += 4;
            std::uint32_t event;
            current_timestamp += delta;
            if (!is_eight_byte)
            {
                if (segment_end - it < 4)
                    return false;
                it += 4;
            }
            if (segment_end - it < 4)
                return false;
            event = it[0] | (it[1] << 8) | (it[2] << 16) | (it[3] << 24);
            it += 4;
            if (event >> 24 == 0x01)
            {
                std::uint8_t buffer[5] = { 0xFF, 0x51 };
                buffer[2] = (uint8_t) (event >> 16);
                buffer[3] = (uint8_t) (event >> 8);
                buffer[4] = (uint8_t) event;
                p_out.add_track_event(
                        0, midi_event(current_timestamp, midi_event::extended, 0, buffer, sizeof(buffer)));
            }
            else if (!(event >> 24))
            {
                unsigned event_code = (event & 0xF0) >> 4;
                if (event_code >= 0x8 && event_code <= 0xE)
                {
                    unsigned bytes_to_write = 1;
                    std::uint8_t buffer[2];
                    buffer[0] = (uint8_t) (event >> 8);
                    if (event_code != 0xC && event_code != 0xD)
                    {
                        buffer[1] = (uint8_t) (event >> 16);
                        bytes_to_write = 2;
                    }
                    track.add_event(midi_event(
                            current_timestamp,
                            (midi_event::event_type)(event_code - 8),
                            event & 0x0F,
                            buffer,
                            bytes_to_write));
                }
            }
        }
    }

    track.add_event(midi_event(current_timestamp, midi_event::extended, 0, end_of_track, _countof(end_of_track)));

    p_out.add_track(track);

    return true;
}

}  // namespace Rival
