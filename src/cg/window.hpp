#pragma once

#include <vector>
#include <string>
#include <functional>
#include <string_view>
#include "_cgControl.hpp"

namespace CG {
    using WindowSize = Vector<int, 2>;

    class Window {
        private:
            static std::vector<Window *> instances;

            GLFWwindow *w = nullptr;
            std::string title = "";
            WindowSize size = WindowSize(0, 0);
            std::function<void()> onRenderLoop = [](){};
            uint32_t vaoId = 0;

            void clearFields();
            void moveFrom(Window &);

        public:
            static const std::vector<Window *> &getInstances();
            static Window *find(GLFWwindow *ptr);
            static Window *getCurrentContext();

            Window() = default;
            Window(std::string_view title, int width, int height);
            Window(const Window &) = delete;
            Window(Window &&other);

            void makeContextCurrent() const;
            void renderLoop() const;
            void close();
            bool shouldClose() const;
            bool exists() const;

            GLFWwindow *getWPtr() const;

            std::string_view getTitle() const;
            void setTitle(std::string_view);

            WindowSize getSize() const;
            void setSize(const WindowSize &);
            void setSize(int width, int height);

            int getWidth() const;
            void setWidth(int);

            int getHeight() const;
            void setHeight(int);

            int getAspectRatio() const;

            std::function<void()> getOnRenderLoop() const;
            void setOnRenderLoop(const std::function<void()> &);

            Window &operator=(const Window &) = delete;
            Window &operator=(Window &&other);
            bool operator==(const Window &other) const;

            ~Window();
    };
}