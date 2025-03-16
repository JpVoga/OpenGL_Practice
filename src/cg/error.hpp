#pragma once

#include "_cgControl.hpp"

namespace CG {
    [[noreturn]] void onGlError(
        [[maybe_unused]] GLenum source,
        [[maybe_unused]] GLenum type,
        [[maybe_unused]] GLuint id,
        [[maybe_unused]] GLenum severity,
        [[maybe_unused]] GLsizei length,
        const GLchar *message,
        [[maybe_unused]] const void *userParam);
}