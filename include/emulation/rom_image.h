#pragma once

#include <map>
#include <string>

#include "emulation/debug_info.h"
#include "enum/platform_type.h"

using namespace std;

class rom_image
{
private:
    map<int, char *> rom_map;

    void load_rom_image(debug_segment segment_def);

public:
    rom_image(debug_info *debug);
    platform_type detect_platform();
    char *get(int id);
};
