#pragma once

#include "_cgControl.hpp"

namespace CG {
    template<typename T>
    concept validVertexType =
        requires(T t) {{auto(t.position)} -> std::same_as<Vector3>;} && // Use 'auto' or else it takes a reference to the field!
        requires() {{T::enableVertexAttributes()} -> std::same_as<void>;}
    ;


    struct Vertex {
        static void enableVertexAttributes();

        Vector3 position = Vector3::zero;
        Vector2 uv = Vector2::zero;
        Vector3 normal = Vector3::forward;
    };
}