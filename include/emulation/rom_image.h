#pragma once

#include <map>
#include <string>
#include <filesystem>

#include "enum/platform_type.h"

using namespace std;

class debug_info;
class debug_segment;

class rom_image
{
private:
    map<int, string> segment_id_image_id_map;
    map<string, char *> image_map;

    void load_rom_image(filesystem::path debug_path, debug_segment *segment_def);

public:
    rom_image(debug_info *debug);
    platform_type detect_platform();
    char *get(int segment_id);
};
