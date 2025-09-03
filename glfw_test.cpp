#include "pypp_util/main_error_handler.h"
#include <GLFW/glfw3.h>
#include <cstdlib> // Required for EXIT_FAILURE
#include <string>

int main() {
    try {
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        std::string title = "Hello World";
        GLFWwindow *window =
            glfwCreateWindow(640, 480, title.c_str(), NULL, NULL);
        if (!window) {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}