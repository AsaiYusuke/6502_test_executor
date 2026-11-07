#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <vector>

#include "enum/platform_type.hpp"

using namespace std;

class debug_info;
class debug_segment;

class rom_image
{
private:
    map<int, filesystem::path> segment_id_image_id_map;
    map<filesystem::path, vector<uint8_t>> image_map;

    void load_rom_image(filesystem::path debug_path, debug_segment *segment_def);

public:
    explicit rom_image(debug_info *debug);
    platform_type detect_platform() const;
    uint8_t get(int segment_id, uint64_t image_offset) const;
};
