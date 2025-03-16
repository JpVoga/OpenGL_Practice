#include "vertex.hpp"
#include "vertexBuffer.hpp"

namespace CG {
    void VertexBuffer::clearFields() {
        id = 0;
        data = nullptr;
        size = 0;
        enableVertexAttributes = [](){};
        drawMode = DrawMode::Static;
    }

    void VertexBuffer::moveFrom(VertexBuffer &other) {
        if (this == &other) return;

        id = other.id;
        data = other.data;
        size = other.size;
        enableVertexAttributes = std::move(other.enableVertexAttributes);
        drawMode = other.drawMode;

        other.clearFields();
    }

    void VertexBuffer::destroy() {
        if (exists()) glDeleteBuffers(1, &id);
        clearFields();
    }

    VertexBuffer::VertexBuffer(VertexBuffer &&other) {this->moveFrom(other);}

    bool VertexBuffer::exists() const noexcept {return (id != 0);}

    const void *VertexBuffer::getData() const noexcept {return data;}

    DrawMode VertexBuffer::getDrawMode() const noexcept {return drawMode;}

    void VertexBuffer::setDrawMode(DrawMode drawMode) {
        bind();
        glBufferData(GL_ARRAY_BUFFER, this->size, this->data, static_cast<GLenum>(drawMode));

        this->drawMode = drawMode;
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        enableVertexAttributes();
    }

    VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) {
        moveFrom(other);
        return (*this);
    }

    VertexBuffer::~VertexBuffer() noexcept {destroy();}
}