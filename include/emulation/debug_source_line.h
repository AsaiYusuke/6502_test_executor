#pragma once

#include <string>

using namespace std;

class debug_source_line
{
private:
    int file_id;
    int segment_id;
    int line_number;
    bool cover;

public:
    debug_source_line(int _file_id, int _segment_id, int _line_number);
    int get_file_id();
    int get_segment_id();
    int get_line_number();
    bool is_cover();
    void set_cover();
};
