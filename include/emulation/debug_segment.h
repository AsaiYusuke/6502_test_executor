#pragma once

#include <string>

using namespace std;

class debug_segment
{
private:
    int id;
    string name;
    uint16_t start;
    int size;
    bool writable;
    string image_file_name;
    int image_file_offset;

public:
    debug_segment(int id, string name, uint16_t start, int size, bool writable, string image_file_name, int image_file_offset);
    bool contains(uint16_t address);
    bool is_nes_cpu_memory();
    uint16_t get_image_file_address(uint16_t address);
    int get_id();
    string get_name();
    uint16_t get_start();
    int get_size();
    bool is_writable();
    bool is_readonly();
    string get_image_file_name();
    int get_image_file_offset();
};
