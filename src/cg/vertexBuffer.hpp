#pragma once

#include "_cgControl.hpp"
#include "drawMode.hpp"
#include "vertex.hpp"

namespace CG {
    class VertexBuffer {
        private:
            uint32_t id = 0;
            const void *data = nullptr;
            size_t size = 0;
            std::function<void()> enableVertexAttributes = [](){};
            DrawMode drawMode = DrawMode::Static;

            void clearFields();
            void moveFrom(VertexBuffer &other);
            void destroy();

        public:
            VertexBuffer() = delete;
            VertexBuffer(const VertexBuffer &) = delete;
            template<validVertexType T, size_t spanCount> explicit VertexBuffer(std::span<const T, spanCount> data, DrawMode drawMode = DrawMode::Static);
            template<validVertexType T, size_t arrayCount> explicit VertexBuffer(const std::array<T, arrayCount> &vertices, DrawMode drawMode = DrawMode::Static);
            template<validVertexType T> explicit VertexBuffer(const std::vector<T> &vertices, DrawMode drawMode = DrawMode::Static);
            VertexBuffer(VertexBuffer &&other);

            bool exists() const noexcept;
            const void *getData() const noexcept;
            template<validVertexType T, size_t spanCount> void setData(std::span<const T, spanCount> data);
            template<validVertexType T, size_t spanCount> void setData(std::span<const T, spanCount> data, DrawMode drawMode);
            template<validVertexType T, size_t arrayCount> void setData(const std::array<T, arrayCount> &data);
            template<validVertexType T, size_t arrayCount> void setData(const std::array<T, arrayCount> &data, DrawMode drawMode);
            template<validVertexType T> void setData(const std::vector<T> &data);
            template<validVertexType T> void setData(const std::vector<T> &data, DrawMode drawMode);
            DrawMode getDrawMode() const noexcept;
            void setDrawMode(DrawMode drawMode);
            void bind() const;

            VertexBuffer &operator=(const VertexBuffer &) = delete;
            VertexBuffer &operator=(VertexBuffer &&other);

            ~VertexBuffer() noexcept;
    };


    template<validVertexType T, size_t spanCount>
    VertexBuffer::VertexBuffer(std::span<const T, spanCount> data, DrawMode drawMode) {
        glGenBuffers(1, &this->id);
        this->setData(data, drawMode);
    }

    template<validVertexType T, size_t arrayCount>
    VertexBuffer::VertexBuffer(const std::array<T, arrayCount> &vertices, DrawMode drawMode): VertexBuffer(std::span(vertices), drawMode) {}

    template<validVertexType T>
    VertexBuffer::VertexBuffer(const std::vector<T> &vertices, DrawMode drawMode): VertexBuffer(std::span(vertices), drawMode) {}

    template<validVertexType T, size_t spanCount>
    void VertexBuffer::setData(std::span<const T, spanCount> data) {
        setData(data, this->drawMode);
    }

    template<validVertexType T, size_t spanCount>
    void VertexBuffer::setData(std::span<const T, spanCount> data, DrawMode drawMode) {
        if (!(exists())) return;

        bind();
        glBufferData(GL_ARRAY_BUFFER, data.size_bytes(), data.data(), static_cast<GLenum>(drawMode));

        this->data = data.data();
        this->size = size;
        if constexpr (validVertexType<T>) this->enableVertexAttributes = T::enableVertexAttributes;
        this->drawMode = drawMode;
    }

    template<validVertexType T, size_t arrayCount> void VertexBuffer::setData(const std::array<T, arrayCount> &data) {setData(std::span(data));}
    template<validVertexType T, size_t arrayCount> void VertexBuffer::setData(const std::array<T, arrayCount> &data, DrawMode drawMode) {setData(std::span(data), drawMode);}

    template<validVertexType T> void VertexBuffer::setData(const std::vector<T> &data) {setData(std::span(data));}
    template<validVertexType T> void VertexBuffer::setData(const std::vector<T> &data, DrawMode drawMode) {setData(std::span(data), drawMode);}
}