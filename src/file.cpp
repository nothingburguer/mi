#include "include/file.hpp"
#include <fstream>

bool File::load(const std::string& path, Buffer& buf) {
    std::ifstream file(path);
    if (!file) return false;

    buf.lines.clear();

    std::string line;
    while (std::getline(file, line)) {
        buf.lines.push_back(line);
    }

    if (buf.lines.empty())
        buf.lines.push_back("");

    buf.cursor.x = 0;
    buf.cursor.y = 0;
    buf.set_modified(false);

    return true;
}

bool File::save(const std::string& path, const Buffer& buf) {
    std::ofstream file(path);
    if (!file) return false;

    for (const auto& line : buf.lines)
        file << line << '\n';

    return true;
}

