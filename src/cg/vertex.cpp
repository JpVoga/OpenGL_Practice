#include "vertex.hpp"

namespace CG {
    void Vertex::enableVertexAttributes() {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), std::bit_cast<const void *>(offsetof(Vertex, position)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), std::bit_cast<const void *>(offsetof(Vertex, uv)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), std::bit_cast<const void *>(offsetof(Vertex, normal)));
    }
}