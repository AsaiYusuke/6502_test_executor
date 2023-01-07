#pragma once

#include <string>

using namespace std;

class debug_source_line
{
private:
    int file_id;
    int segment_id;
    int line_number;
    int cover;

public:
    debug_source_line(int _file_id, int _segment_id, int _line_number);
    int get_file_id() const;
    int get_segment_id() const;
    int get_line_number() const;
    int get_cover() const;
    void add_cover();
};
