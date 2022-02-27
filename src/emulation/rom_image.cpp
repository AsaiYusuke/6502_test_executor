#include <fstream>

#include "emulation/rom_image.h"
#include "exception/file_open.h"

rom_image::rom_image(debug_info *debug)
{
    for (auto element : debug->get_segment_def_map())
        load_rom_image(element.second);
}

void rom_image::load_rom_image(debug_segment segment_def)
{
    int id = segment_def.get_id();
    string path = segment_def.get_image_file_name();
    if (path.empty() || rom_map.count(id) != 0)
        return;

    streampos size;
    ifstream file(path, ios::in | ios::binary | ios::ate);
    if (!file.is_open())
        throw file_open_error(path);

    size = file.tellg();
    rom_map[id] = new char[size];
    file.seekg(0, ios::beg);
    file.read(rom_map[id], size);
    file.close();
}

platform_type rom_image::detect_platform()
{
    for (auto element : rom_map)
    {
        auto rom = element.second;
        if (rom[0] == 'N' && rom[1] == 'E' && rom[2] == 'S' && rom[3] == 0x1A)
            return platform_type::NES;
    }
    return platform_type::UNKNOWN;
}

char *rom_image::get(int id)
{
    return rom_map[id];
}
