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
        pypp::PyStr resource_path =
            pypp::pypp_get_resource_dir(); // Ensure resources can be accessed
        pypp::PyStr dir_path =
            pypp::os::path::join(resource_path, pypp::PyStr("test"));
        std::cout << "dir path: " << dir_path << std::endl;

        // Create a directory in resources
        pypp::os::makedirs(dir_path);
        std::cout << "Created directory: " << dir_path << std::endl;

        // create and write to a file in the directory
        pypp::PyStr file_path =
            pypp::os::path::join(dir_path, pypp::PyStr("test_file.txt"));
        {
            pypp::PyTextIO file(file_path, pypp::PyStr("w"));
            file.write(pypp::PyStr("Hello!This is a test file.\n"));
            file.write(pypp::PyStr("Second line.\n"));
        }

        // rename the file
        pypp::PyStr new_file_path =
            pypp::os::path::join(dir_path, pypp::PyStr("renamed_file.txt"));
        pypp::os::rename(file_path, new_file_path);

        // check that the file exists
        if (pypp::os::path::exists(new_file_path)) {
            std::cout << "File exists: " << new_file_path << std::endl;
        }

        // Check if directory or file
        std::cout << "Is '" << dir_path << "' a directory? " << std::boolalpha
                  << pypp::os::path::isdir(dir_path) << std::endl;
        std::cout << "Is '" << new_file_path << "' a file? " << std::boolalpha
                  << pypp::os::path::isfile(new_file_path) << std::endl;

        // Get path components
        std::cout << "Directory name: " << pypp::os::path::dirname(file_path)
                  << std::endl;
        std::cout << "Base name: " << pypp::os::path::basename(file_path)
                  << std::endl;

        // read the file completely
        {
            pypp::PyTextIO file(new_file_path, pypp::PyStr("r"));
            pypp::PyStr content = file.read();
            std::cout << "File content:\n" << content.str() << std::endl;
        }

        // read the file line by line
        {
            pypp::PyTextIO file(new_file_path, pypp::PyStr("r"));
            std::cout << "first line: " << file.readline().str() << std::endl;
            std::cout << "second line: " << file.readline().str() << std::endl;
            std::cout << "third line: " << file.readline().str() << std::endl;
        }

        // readlines()
        {
            pypp::PyTextIO file(new_file_path, pypp::PyStr("r"));
            pypp::PyList<pypp::PyStr> lines = file.readlines();
            lines.print();
        }

        // writelines()
        {
            pypp::PyTextIO file(new_file_path, pypp::PyStr("a"));
            pypp::PyList<pypp::PyStr> lines_to_write = {
                pypp::PyStr("Line 1\n"), pypp::PyStr("Line 2\n"),
                pypp::PyStr("Line 3\n")};
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
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}