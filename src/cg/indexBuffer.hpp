#pragma once

#include "_cgControl.hpp"
#include "drawMode.hpp"

namespace CG {
    class IndexBuffer {
        private:
            uint32_t id = 0;
            const void *data = nullptr;
            size_t size = 0;
            DrawMode drawMode = DrawMode::Static;

            void clearFields();
            void moveFrom(IndexBuffer &other);
            void destroy();
            template<validIndexType T> void setDataInternal(const T *data, size_t size, DrawMode drawMode);

        public:
            IndexBuffer() = delete;
            IndexBuffer(const IndexBuffer &) = delete;
            template <validIndexType T> IndexBuffer(std::span<const T> values, DrawMode drawMode = DrawMode::Static);
            explicit IndexBuffer(const IndexList &indices, DrawMode drawMode = DrawMode::Static);
            IndexBuffer(IndexBuffer &&other);

            bool exists() const noexcept;
            const void *getData() const noexcept;
            template<validIndexType T> void setData(std::span<const T> values, DrawMode drawMode = DrawMode::Static);
            void setData(const IndexList &indices, DrawMode drawMode = DrawMode::Static);
            DrawMode getDrawMode() const noexcept;
            void setDrawMode(DrawMode drawMode);
            void bind() const;

            IndexBuffer &operator=(const IndexBuffer &) = delete;
            IndexBuffer &operator=(IndexBuffer &&other);

            ~IndexBuffer() noexcept;
    };


    template <validIndexType T>
    inline void IndexBuffer::setDataInternal(const T *data, size_t size, DrawMode drawMode) {
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, static_cast<GLenum>(drawMode));

        this->data = data;
        this->size = size;
        this->drawMode = drawMode;
    }

    template <validIndexType T>
    inline IndexBuffer::IndexBuffer(std::span<const T> values, DrawMode drawMode) {
        glGenBuffers(1, &this->id);
        this->setDataInternal(values.data(), values.size_bytes(), drawMode);
    }

    template <validIndexType T>
    inline void IndexBuffer::setData(std::span<const T> values, DrawMode drawMode) {
        this->setDataInternal(values.data(), values.size_bytes(), drawMode);
    }
}