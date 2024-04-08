#pragma once

#include <string>
#include <filesystem>

inline std::filesystem::path GetFileDir(std::string file) {
    std::filesystem::path path{std::move(file)};
    if (path.is_absolute()) {
        return path.parent_path();
    } else {
        throw std::runtime_error{"Bad file name"};
    }
}
