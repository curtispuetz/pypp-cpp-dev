#include "py_dict.h"
#include "pypp_os.h"
#include "pypp_resources.h"
#include "pypp_text_io.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <pypp_util/main_error_handler.h>
#include <vector>

int main() {
    try {
        // Get the resources path
        PyStr dir_path = pypp_get_resources(
            PyStr("test")); // Ensure resources can be accessed
        std::cout << "dir path: " << dir_path << std::endl;

        // Create a directory in resources
        if (os::makedirs(dir_path)) {
            std::cout << "Created directory: " << dir_path << std::endl;
        }

        // create and write to a file in the directory
        PyStr file_path = os::path::join(dir_path, PyStr("test_file.txt"));
        {
            PyTextIO file(file_path, PyStr("w"));
            file.write(PyStr("Hello!This is a test file.\n"));
            file.write(PyStr("Second line.\n"));
        }

        // rename the file
        PyStr new_file_path =
            os::path::join(dir_path, PyStr("renamed_file.txt"));
        os::rename(file_path, new_file_path);

        // check that the file exists
        if (os::path::exists(new_file_path)) {
            std::cout << "File exists: " << new_file_path << std::endl;
        }

        // Check if directory or file
        std::cout << "Is '" << dir_path << "' a directory? " << std::boolalpha
                  << os::path::isdir(dir_path) << std::endl;
        std::cout << "Is '" << new_file_path << "' a file? " << std::boolalpha
                  << os::path::isfile(new_file_path) << std::endl;

        // Get path components
        std::cout << "Directory name: " << os::path::dirname(file_path)
                  << std::endl;
        std::cout << "Base name: " << os::path::basename(file_path)
                  << std::endl;

        // read the file completely
        {
            PyTextIO file(new_file_path, PyStr("r"));
            PyStr content = file.read();
            std::cout << "File content:\n" << content.str() << std::endl;
        }

        // read the file line by line
        {
            PyTextIO file(new_file_path, PyStr("r"));
            std::cout << "first line: " << file.readline().str() << std::endl;
            std::cout << "second line: " << file.readline().str() << std::endl;
            std::cout << "third line: " << file.readline().str() << std::endl;
        }

        // readlines()
        {
            PyTextIO file(new_file_path, PyStr("r"));
            PyList<PyStr> lines = file.readlines();
            lines.print();
        }

        // writelines()
        {
            PyTextIO file(new_file_path, PyStr("a"));
            PyList<PyStr> lines_to_write = {
                PyStr("Line 1\n"), PyStr("Line 2\n"), PyStr("Line 3\n")};
            file.writelines(lines_to_write);
            std::cout << "Wrote multiple lines to the file." << std::endl;
        }

        // Things to check for is:
        // remove a directory
        // remove dirs
        // mkdir
        // split
        // abspath
        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}