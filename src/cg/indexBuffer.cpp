#include "indexBuffer.hpp"

namespace CG {
    void IndexBuffer::clearFields() {
        id = 0;
        data = nullptr;
        size = 0;
        drawMode = DrawMode::Static;
    }

    void IndexBuffer::moveFrom(IndexBuffer &other) {
        if (this == &other) return;

        id = other.id;
        data = other.data;
        size = other.size;
        drawMode = other.drawMode;

        other.clearFields();
    }

    void IndexBuffer::destroy() {
        if (exists()) glDeleteBuffers(1, &id);
        clearFields();
    }

    IndexBuffer::IndexBuffer(const IndexList &indices, DrawMode drawMode) {
        glGenBuffers(1, &this->id);
        this->setData(indices, drawMode);
    }

    IndexBuffer::IndexBuffer(IndexBuffer &&other)
    {
        this->moveFrom(other);
    }

    bool IndexBuffer::exists() const noexcept {
        return (id != 0);
    }

    const void *IndexBuffer::getData() const noexcept {return data;}

    void IndexBuffer::setData(const IndexList &indices, DrawMode drawMode) {
        switch (indices.getTypeEnum()) {
            case IndexType::ui8bit:
                this->setDataInternal(std::bit_cast<const uint8_t *>(indices.getDataPtr()), indices.getMemorySize(), drawMode);
                break;
            case IndexType::ui16bit:
                this->setDataInternal(std::bit_cast<const uint16_t *>(indices.getDataPtr()), indices.getMemorySize(), drawMode);
                break;
            case IndexType::ui32bit:
                this->setDataInternal(std::bit_cast<const uint32_t *>(indices.getDataPtr()), indices.getMemorySize(), drawMode);
                break;
        };
    }

    DrawMode IndexBuffer::getDrawMode() const noexcept {return drawMode;}

    void IndexBuffer::setDrawMode(DrawMode drawMode) {
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, static_cast<GLenum>(drawMode));
        this->drawMode = drawMode;
    }

    void IndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) {
        this->moveFrom(other);
        return (*this);
    }

    IndexBuffer::~IndexBuffer() noexcept {destroy();}
}