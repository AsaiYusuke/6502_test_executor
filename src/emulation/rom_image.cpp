#include <fstream>

#include "emulation/rom_image.h"
#include "exception/file_open.h"

rom_image::rom_image(debug_info *debug)
{
    for (auto element : debug->get_segment_def_map())
        load_rom_image(debug->get_path(), element.second);
}

void rom_image::load_rom_image(filesystem::path debug_path, debug_segment segment_def)
{
    int segment_id = segment_def.get_id();
    string path = segment_def.get_image_file_name();
    if (path.empty() || segment_id_image_id_map.count(segment_id) != 0)
        return;

    debug_path = absolute(debug_path.parent_path());
    ifstream *file;
    while(1)
    {
        auto append_path = debug_path / path;
        file = new ifstream(append_path, ios::in | ios::binary | ios::ate);
        if (file->is_open())
            break;
        if (!debug_path.has_parent_path())
            throw file_open_error(path);
        debug_path = debug_path.parent_path();
    }

    streampos size = file->tellg();
    auto image = new char[size];
    file->seekg(0, ios::beg);
    file->read(image, size);
    file->close();

    auto image_id = images.size();
    images.push_back(image);

    if (image_name_image_id_map.count(path) == 0)
        image_name_image_id_map[path] = image_id;

    segment_id_image_id_map[segment_id] = image_id;
}

platform_type rom_image::detect_platform()
{
    for (auto image : images)
    {
        if (image[0] == 'N' && image[1] == 'E' && image[2] == 'S' && image[3] == 0x1A)
            return platform_type::NES;
    }
    return platform_type::UNKNOWN;
}

char *rom_image::get(int segment_id)
{
    return images[segment_id_image_id_map[segment_id]];
}
