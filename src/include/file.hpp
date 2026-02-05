#pragma once
#include "buffer.hpp"
#include <string>

namespace File {
    bool load(const std::string& path, Buffer& buf);
    bool save(const std::string& path, const Buffer& buf);
}

