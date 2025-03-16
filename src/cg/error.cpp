#include "error.hpp"
#include "window.hpp"

namespace CG {
    [[noreturn]] void onGlError(
        [[maybe_unused]] GLenum source,
        [[maybe_unused]] GLenum type,
        [[maybe_unused]] GLuint id,
        [[maybe_unused]] GLenum severity,
        [[maybe_unused]] GLsizei length,
        const GLchar *message,
        [[maybe_unused]] const void *userParam) {
        const Window &window = *(Window::getCurrentContext());
        const auto stackTrace = std::stacktrace::current();

        const std::string finalMessage =
            std::string("From window \"") + window.getTitle().data() + "\":" +
            message + '\n' +
            std::to_string(stackTrace)
        ;

        throw std::runtime_error(finalMessage);
    }
}