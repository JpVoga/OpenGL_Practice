#pragma once

#include "_cgControl.hpp"

namespace CG {
    struct LoadedImagePixelsDeleter {
        void operator()(void *ptr) {
            stbi_image_free(ptr);
        }
    };
}