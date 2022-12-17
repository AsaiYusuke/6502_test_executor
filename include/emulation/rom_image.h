#pragma once

#include <map>
#include <string>
#include <filesystem>

#include "emulation/debug_info.h"
#include "enum/platform_type.h"

using namespace std;

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
