#include <iostream>
#include "init.hpp"
#include "error.hpp"
#include "window.hpp"

namespace CG {

    std::vector<Window *> Window::instances = {};

    void Window::clearFields() {
        w = nullptr;
        title = "";
        size = WindowSize(0, 0);
        onRenderLoop = [](){};
        vaoId = 0;
    }

    void Window::moveFrom(Window &other) {
        if (this == &other) return;

        this->w = other.w;
        this->title = other.title;
        this->size = other.size;
        this->onRenderLoop = other.onRenderLoop;
        this->vaoId = other.vaoId;

        // Add this to instances
        const auto thisIter = std::find(instances.begin(), instances.end(), this);
        if (thisIter == instances.end()) instances.push_back(this);

        // Remove temporary from instances
        const auto otherIter = std::find(instances.begin(), instances.end(), &other);
        if (otherIter != instances.end()) instances.erase(otherIter);

        other.clearFields();
    }

    const std::vector<Window *> &Window::getInstances() {
        return instances;
    }

    Window *Window::find(GLFWwindow *ptr) {
        for (const auto &window: instances) {
            if (window->w == ptr) return window;
        }
        return nullptr;
    }

    Window *Window::getCurrentContext() {
        return find(glfwGetCurrentContext());
    }

    Window::Window(std::string_view title, int width, int height) {
        if (!isGlfwInitialized()) initializeGlfw();

        this->w = glfwCreateWindow(width, height, title.data(), nullptr, (instances.empty()? nullptr:instances[0]->w)); // Always share context with first possible window
        this->title = title;
        this->size = WindowSize(width, height);

        if (!(this->w)) {
            std::clog << "Failed to create window titled \"" << title << "\"\n";
            this->clearFields();
            return;
        }

        glfwMakeContextCurrent(this->w);
        if (!isGladInitialized()) initializeGlad();

        glGenVertexArrays(1, &this->vaoId);
        glBindVertexArray(this->vaoId);

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(onGlError, nullptr);

        glViewport(0, 0, width, height);
        glfwSetFramebufferSizeCallback(this->w, [](GLFWwindow *w, int width, int height) -> void {
            const auto previousContext = glfwGetCurrentContext();
            glfwMakeContextCurrent(w);
            glViewport(0, 0, width, height);
            glfwMakeContextCurrent(previousContext);
        });

        instances.push_back(this);
    }

    Window::Window(Window &&other) {
        moveFrom(other);
    }

    void Window::makeContextCurrent() const {
        glfwMakeContextCurrent(this->w);
    }

    void Window::renderLoop() const {
        makeContextCurrent();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        onRenderLoop();
        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    void Window::close() {
        if (this->exists()) glfwDestroyWindow(w);
        clearFields();
        auto windowIt = std::find(instances.begin(), instances.end(), this);
        if (windowIt != instances.end()) instances.erase(windowIt);
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(w);
    }

    bool Window::exists() const {
        return (w != nullptr);
    }

    GLFWwindow *Window::getWPtr() const {
        return w;
    }

    std::string_view Window::getTitle() const {
        return title;
    }

    void Window::setTitle(std::string_view newTitle) {
        title = newTitle;
        glfwSetWindowTitle(w, title.c_str());
    }

    WindowSize Window::getSize() const {
        return size;
    }

    void Window::setSize(const WindowSize &newSize) {
        size = newSize;
        glfwSetWindowSize(w, size.x, size.y);
    }

    void Window::setSize(int width, int height) {
        setSize(WindowSize(width, height));
    }

    int Window::getWidth() const {
        return size.x;
    }

    void Window::setWidth(int newWidth) {
        setSize(newWidth, getHeight());
    }

    int Window::getHeight() const {
        return size.y;
    }

    void Window::setHeight(int newHeight) {
        setSize(getWidth(), newHeight);
    }

    int Window::getAspectRatio() const {
        return (size.x / size.y);
    }

    std::function<void()> Window::getOnRenderLoop() const {
        return onRenderLoop;
    }

    void Window::setOnRenderLoop(const std::function<void()> &newOnRenderLoop) {
        onRenderLoop = newOnRenderLoop;
    }

    Window &Window::operator=(Window &&other) {
        moveFrom(other);
        return (*this);
    }

    bool Window::operator==(const Window &other) const {
        return (this->w == other.w);
    }

    Window::~Window() {
        close();
    }
}