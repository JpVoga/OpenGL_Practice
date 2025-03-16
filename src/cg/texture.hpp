#pragma once

#include "_cgControl.hpp"
#include "window.hpp"

namespace CG {
    enum class TextureWrapMode {
        Default        = GL_REPEAT         ,
        Repeat         = GL_REPEAT         ,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        ClampToEdge    = GL_CLAMP_TO_EDGE  ,
        ClampToBorder  = GL_CLAMP_TO_BORDER,
    };

    enum class TextureMinificationMode {
        Default              = GL_LINEAR_MIPMAP_LINEAR  ,
        Nearest              = GL_NEAREST               ,
        Linear               = GL_LINEAR                ,
        NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
        LinearMipmapNearest  = GL_LINEAR_MIPMAP_NEAREST ,
        NearestMipmapLinear  = GL_NEAREST_MIPMAP_LINEAR ,
        LinearMipmapLinear   = GL_LINEAR_MIPMAP_LINEAR
    };

    enum class TextureMagnificationMode {
        Default = GL_LINEAR ,
        Nearest = GL_NEAREST,
        Linear  = GL_LINEAR ,
    };


    class Texture {
        private:
            static constexpr size_t defaultAlignment = 4;
            static constexpr std::array<GLenum, 4> internalFormats = {GL_R8 , GL_RG8, GL_RGB8, GL_RGBA8};
            static constexpr std::array<GLenum, 4> externalFormats = {GL_RED, GL_RG , GL_RGB , GL_RGBA };

            static thread_local std::vector<uint32_t> slottedTextureIds; // Slot number to texture id.
            static thread_local std::unordered_map<uint32_t, int32_t> idToSlotMap;

            static std::vector<uint8_t> verticallyFlip(const uint8_t *data, size_t width, size_t height, size_t channels);

            uint32_t id = 0;
            size_t width = 0, height = 0, channels = 0;
            TextureWrapMode wrapModeU = TextureWrapMode::Default, wrapModeV = TextureWrapMode::Default;
            TextureMinificationMode minificationMode = TextureMinificationMode::Default;
            TextureMagnificationMode magnificationMode = TextureMagnificationMode::Default;

            void clearFields();
            void moveFrom(Texture &other);
            void destroy();

            std::vector<uint8_t> calculatePixelsInternal(bool verticallyFlip) const;
            void setPixelsInternal(const uint8_t *data, size_t width, size_t height, size_t channels, bool loadedFromFile, bool verticallyFlip);

        public:
            static constexpr int32_t noSlot = -1;

            Texture() = delete;
            Texture(std::span<const uint8_t> data, size_t width, size_t height, size_t channels);
            explicit Texture(const std::filesystem::path &path);
            Texture(const Texture &other);
            Texture(Texture &&other);

            bool exists() const noexcept;
            std::vector<uint8_t> calculatePixels() const;
            void setPixels(std::span<const uint8_t> data, size_t width, size_t height, size_t channels);
            size_t getWidth() const noexcept;
            size_t getHeight() const noexcept;
            size_t getChannels() const noexcept;
            TextureWrapMode getWrapModeU() const noexcept;
            void setWrapModeU(TextureWrapMode value);
            TextureWrapMode getWrapModeV() const noexcept;
            void setWrapModeV(TextureWrapMode value);
            TextureMinificationMode getMinificationMode() const noexcept;
            void setMinificationMode(TextureMinificationMode value);
            TextureMagnificationMode getMagnificationMode() const noexcept;
            void setMagnificationMode(TextureMagnificationMode value);
            void slot(int32_t slotNumber);
            int32_t getSlot() const;

            Texture &operator=(const Texture &other);
            Texture &operator=(Texture &&other);

            ~Texture() noexcept;
    };
}