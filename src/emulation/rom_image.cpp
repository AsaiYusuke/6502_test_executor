#include "emulation/rom_image.hpp"

#include <fstream>
#include <limits>
#include <stdexcept>
#include <utility>

#include "emulation/debug_info.hpp"
#include "emulation/debug_segment.hpp"

rom_image::rom_image(debug_info *debug)
{
    for (auto element : debug->get_segment_def_map())
        load_rom_image(debug->get_path(), element.second);
}

void rom_image::load_rom_image(filesystem::path debug_path, debug_segment *segment_def)
{
    int segment_id = segment_def->get_id();
    string path = segment_def->get_image_file_name();
    if (segment_id_image_id_map.count(segment_id) != 0)
        return;

    if (path.empty())
    {
        if (segment_def->is_readonly())
            throw invalid_argument("Readonly segment has no ROM image: " + segment_def->get_name());
        return;
    }

    if (segment_def->get_image_file_offset() < 0 || segment_def->get_size() <= 0)
        throw invalid_argument("Invalid ROM range for segment: " + segment_def->get_name());

    debug_path = absolute(debug_path).parent_path();
    ifstream file;
    filesystem::path image_path;
    while (true)
    {
        image_path = debug_path / path;
        file.open(image_path, ios::in | ios::binary | ios::ate);
        if (file.is_open())
            break;
        if (debug_path.root_path() == debug_path)
            throw invalid_argument("Unable to open ROM image: " + path);
        debug_path = debug_path.parent_path();
        file.clear();
    }

    if (image_map.count(image_path) == 0)
    {
        streampos end = file.tellg();
        if (end < 0)
            throw invalid_argument("Unable to determine ROM image size: " + image_path.string());

        auto file_size = static_cast<uint64_t>(end);
        uint64_t image_begin = static_cast<uint64_t>(segment_def->get_image_file_offset());
        uint64_t image_end = image_begin + static_cast<uint64_t>(segment_def->get_size());
        if (image_end < image_begin || image_end > file_size)
            throw invalid_argument(
                "ROM image is too small for segment " + segment_def->get_name() +
                " in " + image_path.string());
        vector<uint8_t> image;
        if (file_size > numeric_limits<size_t>::max() ||
            file_size > static_cast<uint64_t>(numeric_limits<streamsize>::max()) ||
            file_size > image.max_size())
            throw invalid_argument("ROM image is too large to load: " + image_path.string());

        image.resize(static_cast<size_t>(file_size));
        file.seekg(0, ios::beg);
        if (!file || (!image.empty() &&
                      (!file.read(reinterpret_cast<char *>(image.data()), static_cast<streamsize>(image.size())) ||
                       file.gcount() != static_cast<streamsize>(image.size()))))
            throw invalid_argument("Unable to read complete ROM image: " + image_path.string());

        image_map.emplace(image_path, std::move(image));
    }
    else
    {
        const auto &image = image_map.at(image_path);
        uint64_t image_begin = static_cast<uint64_t>(segment_def->get_image_file_offset());
        uint64_t image_end = image_begin + static_cast<uint64_t>(segment_def->get_size());
        if (image_end < image_begin || image_end > image.size())
            throw invalid_argument(
                "ROM image is too small for segment " + segment_def->get_name() +
                " in " + image_path.string());
    }

    segment_id_image_id_map[segment_id] = image_path;
}

platform_type rom_image::detect_platform() const
{
    for (auto entry : image_map)
    {
        const auto &image = entry.second;
        if (image.size() >= 4 &&
            image[0] == 'N' && image[1] == 'E' && image[2] == 'S' && image[3] == 0x1A)
            return platform_type::NES;
    }
    return platform_type::UNKNOWN;
}

uint8_t rom_image::get(int segment_id, uint64_t image_offset) const
{
    auto segment = segment_id_image_id_map.find(segment_id);
    if (segment == segment_id_image_id_map.end())
        throw invalid_argument("ROM image not found for segment id: " + to_string(segment_id));

    const auto &image = image_map.at(segment->second);
    if (image_offset >= image.size())
        throw invalid_argument(
            "ROM image offset is out of range for segment id: " + to_string(segment_id));
    return image[static_cast<size_t>(image_offset)];
}
