#include "init.hpp"

namespace CG {
    static bool glfwInitialized = false;
    static bool gladInitialized = false;

    bool isGlfwInitialized() {return glfwInitialized;}
    bool isGladInitialized() {return gladInitialized;}

    void initializeGlfw() {
        if (isGlfwInitialized()) return;

        if (glfwInit() == GLFW_TRUE) [[likely]] {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

            glfwInitialized = true;
        }
        else [[unlikely]] {
            std::clog << "Failed to initialize GLFW!\n";
            std::exit(1);
        }
    }

    void initializeGlad() {
        if (gladInitialized) return;

        if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) [[likely]] {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_TEXTURE_2D);

            gladInitialized = true;
        }
        else [[unlikely]] {
            std::clog << "Failed to initialize GLAD! Please make sure to have a GLFW context first!\n";
            std::exit(1);
        }
    }
}