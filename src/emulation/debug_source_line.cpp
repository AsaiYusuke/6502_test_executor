#include "emulation/debug_source_line.h"

debug_source_line::debug_source_line(int _file_id, int _segment_id, int _line_number)
{
    file_id = _file_id;
    segment_id = _segment_id;
    line_number = _line_number;
    cover = false;
}

int debug_source_line::get_file_id()
{
    return file_id;
}

int debug_source_line::get_segment_id()
{
    return segment_id;
}

int debug_source_line::get_line_number()
{
    return line_number;
}

int debug_source_line::get_cover()
{
    return cover;
}

void debug_source_line::add_cover()
{
    cover++;
}
