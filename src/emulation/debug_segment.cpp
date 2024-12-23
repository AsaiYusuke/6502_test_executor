#include "emulation/debug_segment.hpp"

debug_segment::debug_segment(int _id, const string &_name, uint16_t _start, int _size, bool _writable, const string &_image_file_name, int _image_file_offset)
{
    id = _id;
    name = _name;
    start = _start;
    size = _size;
    writable = _writable;
    image_file_name = _image_file_name;
    image_file_offset = _image_file_offset;
}

bool debug_segment::contains(uint16_t address) const
{
    return start <= address && address < start + size;
}

bool debug_segment::is_nes_cpu_memory() const
{
    return start != 0x0 || image_file_name.empty();
}

uint16_t debug_segment::get_image_file_address(uint16_t address) const
{
    return image_file_offset + address - start;
}

int debug_segment::get_id() const
{
    return id;
}

string debug_segment::get_name() const
{
    return name;
}

uint16_t debug_segment::get_start() const
{
    return start;
}

int debug_segment::get_size() const
{
    return size;
}

bool debug_segment::is_writable() const
{
    return writable;
}

bool debug_segment::is_readonly() const
{
    return !writable;
}

string debug_segment::get_image_file_name() const
{
    return image_file_name;
}

int debug_segment::get_image_file_offset() const
{
    return image_file_offset;
}
