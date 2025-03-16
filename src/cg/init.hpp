#pragma once

#include "_cgControl.hpp"

namespace CG {
    bool isGlfwInitialized();
    bool isGladInitialized();
    void initializeGlfw();
    void initializeGlad();
}