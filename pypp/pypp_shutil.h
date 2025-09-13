#pragma once
#include "exceptions/filesystem.h"
#include "py_str.h"
#include <filesystem>
#include <string>

namespace pypp {
namespace shutil {
// Recursively deletes a directory and all its contents, like Python's
// shutil.rmtree(path)
inline void rmtree(const PyStr &path) {
    std::error_code ec;
    std::filesystem::path fs_path(path.str());
    std::filesystem::remove_all(fs_path, ec);
    if (ec == std::errc::no_such_file_or_directory) {
        throw FileNotFoundError(
            PyStr("The system cannot find the file specified: ") + path);
    }
    if (ec == std::errc::not_a_directory) {
        throw NotADirectoryError(PyStr("The directory name is invalid: ") +
                                 path);
    }
    if (ec == std::errc::permission_denied) {
        throw PermissionError(PyStr("Permission denied: ") + path);
    }
    if (ec) {
        throw OSError(PyStr("[Error code: ") + std::to_string(ec.value()) +
                      PyStr("] shutil::rmtree failed: ") + path);
    }
}
} // namespace shutil
} // namespace pypp