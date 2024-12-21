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
    debug_segment(int id, const string &name, uint16_t start, int size, bool writable, const string &image_file_name, int image_file_offset);
    bool contains(uint16_t address) const;
    bool is_nes_cpu_memory() const;
    uint16_t get_image_file_address(uint16_t address) const;
    int get_id() const;
    string get_name() const;
    uint16_t get_start() const;
    int get_size() const;
    bool is_writable() const;
    bool is_readonly() const;
    string get_image_file_name() const;
    int get_image_file_offset() const;
};
