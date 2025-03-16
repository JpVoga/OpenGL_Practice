#pragma once

#include "_cgControl.hpp"

namespace CG {
    enum class DrawMode {
        Static  = GL_STATIC_DRAW ,
        Dynamic = GL_DYNAMIC_DRAW,
        Stream  = GL_STREAM_DRAW ,
        All     = GL_STATIC_DRAW|GL_DYNAMIC_DRAW|GL_STREAM_DRAW
    };
}