#pragma once

#include "exceptions/filesystem.h"
#include "py_list.h"
#include "py_str.h"
#include "py_tuple.h"
#include <filesystem>
#include <numeric>
#include <vector>

namespace pypp {
namespace os {

namespace fs = std::filesystem;

inline void makedirs(const PyStr &p) {
    fs::path path = fs::path(p.str());
    if (fs::is_directory(path))
        throw FileExistsError(PyStr("os.makedirs(dir) dir already exists: ") +
                              p);
    try {
        if (!fs::create_directories(path))
            throw OSError(PyStr("os.makedirs(dir) dir not created: ") + p);
    } catch (const fs::filesystem_error &e) {
        if (e.code() == std::errc::permission_denied)
            throw PermissionError(PyStr(e.what()));
        throw OSError(PyStr(e.what()));
    }
}

inline void remove(const PyStr &p) {
    fs::path path = fs::path(p.str());
    if (fs::is_directory(path))
        throw OSError(PyStr("os.remove(path) path is a directory: ") + p);
    if (!fs::exists(path))
        throw FileNotFoundError(PyStr("No such file or directory: ") + p);
    try {
        if (!fs::remove(path))
            throw OSError(PyStr("File not removed: ") + p);
    } catch (const fs::filesystem_error &e) {
        if (e.code() == std::errc::permission_denied)
            throw PermissionError(PyStr(e.what()));
        throw OSError(PyStr(e.what()));
    }
}

inline void mkdir(const PyStr &p) {
    fs::path path = fs::path(p.str());
    if (fs::is_directory(path))
        throw FileExistsError(PyStr("os.mkdir(dir) dir already exists: ") + p);
    try {
        if (!fs::create_directory(path))
            throw OSError(PyStr("os.mkdir(dir) dir not created: ") + p);
    } catch (const fs::filesystem_error &e) {
        if (e.code() == std::errc::no_such_file_or_directory)
            throw FileNotFoundError(PyStr(e.what()));
        if (e.code() == std::errc::permission_denied)
            throw PermissionError(PyStr(e.what()));
        throw OSError(PyStr(e.what()));
    }
}

inline void rmdir(const PyStr &p) {
    try {
        fs::path path_obj(p.str());
        if (!fs::is_directory(path_obj))
            throw FileNotFoundError(
                PyStr("os.rmdir(dir) dir does not exist: " + p.str()));
        if (!fs::remove(path_obj))
            throw OSError(PyStr("os.rmdir(dir) dir is not empty: " + p.str()));
    } catch (const fs::filesystem_error &e) {
        if (e.code() == std::errc::permission_denied)
            throw PermissionError(PyStr(e.what()));
        throw OSError(PyStr(e.what()));
    }
}

inline void rename(const PyStr &src, const PyStr &dst) {
    if (fs::exists(fs::path(dst.str())))
        throw FileExistsError(
            PyStr("os.rename(src, dst) dst already exists: ") + dst);
    try {
        fs::rename(fs::path(src.str()), fs::path(dst.str()));
    } catch (const fs::filesystem_error &e) {
        if (e.code() == std::errc::no_such_file_or_directory)
            throw FileNotFoundError(PyStr(e.what()));
        if (e.code() == std::errc::permission_denied)
            throw PermissionError(PyStr(e.what()));
        throw OSError(PyStr(e.what()));
    }
}

inline PyList<PyStr> listdir(const PyStr &p) {
    PyList<PyStr> entries;
    try {
        fs::path path_obj(p.str());
        if (!fs::exists(path_obj))
            throw FileNotFoundError(PyStr("No such directory: " + p.str()));
        if (!fs::is_directory(path_obj))
            throw NotADirectoryError(PyStr("Not a directory: " + p.str()));
        for (const auto &entry : fs::directory_iterator(path_obj)) {
            entries.append(PyStr(entry.path().filename().string()));
        }
    } catch (const fs::filesystem_error &e) {
        if (e.code() == std::errc::permission_denied)
            throw PermissionError(PyStr(e.what()));
        throw OSError(PyStr(e.what()));
    }
    return entries;
}

namespace path {

/**
 * @brief Joins one or more path components intelligently.
 */
template <typename... Args> PyStr join(const PyStr &base, const Args &...args) {
    fs::path result(base.str());
    // This fold expression efficiently joins all arguments
    (result /= ... /= fs::path(args.str()));
    return result.string();
}

/**
 * @brief Checks if a path exists.
 */
inline bool exists(const PyStr &p) { return fs::exists(fs::path(p.str())); }

/**
 * @brief Checks if a path is a directory.
 */
inline bool isdir(const PyStr &p) {
    return fs::is_directory(fs::path(p.str()));
}

/**
 * @brief Checks if a path is a file.
 */
inline bool isfile(const PyStr &p) {
    return fs::is_regular_file(fs::path(p.str()));
}

/**
 * @brief Returns the directory name of a path.
 */
inline PyStr dirname(const PyStr &p) {
    return PyStr(fs::path(p.str()).parent_path().string());
}

/**
 * @brief Returns the base name of a path.
 */
inline PyStr basename(const PyStr &p) {
    return PyStr(fs::path(p.str()).filename().string());
}

/**
 * @brief Splits a path into a (head, tail) pair.
 */
inline PyTup<PyStr, PyStr> split(const PyStr &p) {
    fs::path path_obj(p.str());
    return PyTup(PyStr(path_obj.parent_path().string()),
                 PyStr(path_obj.filename().string()));
}

/**
 * @brief Returns the absolute version of a path.
 */
inline PyStr abspath(const PyStr &p) {
    return PyStr(fs::absolute(fs::path(p.str())).string());
}

} // namespace path
} // namespace os
} // namespace pypp