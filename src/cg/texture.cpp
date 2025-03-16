#include "texture.hpp"
#include "deleters.hpp"

namespace CG {
    thread_local std::vector<uint32_t> Texture::slottedTextureIds = {};
    thread_local std::unordered_map<uint32_t, int32_t> Texture::idToSlotMap = {};

    std::vector<uint8_t> Texture::verticallyFlip(const uint8_t *data, size_t width, size_t height, size_t channels) {
        std::vector<uint8_t> flippedPixels;

        flippedPixels.reserve(channels * width * height);
        for (size_t i = 0; i < height; ++i) {    // Height is number of rows
            for (size_t j = 0; j < width; ++j) { // Width is number of columns
                const size_t pixelIndexInData = ((((height - 1 - i) * width) + j) * channels);
                for (size_t k = 0; k < channels; ++k) {
                    flippedPixels.emplace_back(data[pixelIndexInData + k]);
                }
            }
        }

        return flippedPixels;
    }

    void Texture::clearFields() {
        id = 0;
        width = 0;
        height = 0;
        channels = 0;
        wrapModeU = TextureWrapMode::Default;
        wrapModeV = TextureWrapMode::Default;
        minificationMode = TextureMinificationMode::Default;
        magnificationMode = TextureMagnificationMode::Default;
    }

    void Texture::moveFrom(Texture &other) {
        if (this == &other) return;

        id = other.id;
        width = other.width;
        height = other.height;
        channels = other.channels;
        wrapModeU = other.wrapModeU;
        wrapModeV = other.wrapModeV;
        minificationMode = other.minificationMode;
        magnificationMode = other.magnificationMode;

        other.clearFields();
    }

    void Texture::destroy() {
        if (exists()) {
            glDeleteTextures(1, &id);

            // Make sure this is not in slots
            slottedTextureIds.erase(std::remove(slottedTextureIds.begin(), slottedTextureIds.end(), id), slottedTextureIds.end());
            idToSlotMap.erase(id);
        }

        clearFields();
    }

    std::vector<uint8_t> Texture::calculatePixelsInternal(bool verticallyFlip) const {
        std::vector<uint8_t> result;
        result.resize((width * height * channels), 0);

        glBindTexture(GL_TEXTURE_2D, id);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glGetTexImage(GL_TEXTURE_2D, 0, (externalFormats[channels-1]), GL_UNSIGNED_BYTE, result.data());

        if (verticallyFlip) return Texture::verticallyFlip(result.data(), width, height, channels);
        else return result;
    }

    void Texture::setPixelsInternal(const uint8_t *data, size_t width, size_t height, size_t channels, bool loadedFromFile, bool verticallyFlip) {
        ASSERT((channels > 0) && (channels <= 4));

        const GLenum internalFormat = internalFormats[channels-1];
        const GLenum externalFormat = externalFormats[channels-1];

        glBindTexture(GL_TEXTURE_2D, id);

        if (loadedFromFile) { // Loaded from STB image library, already aligned
            glPixelStorei(GL_UNPACK_ALIGNMENT, defaultAlignment);
        }
        else {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }

        std::vector<uint8_t> flippedPixels;
        if (verticallyFlip) {
            flippedPixels = Texture::verticallyFlip(data, width, height, channels);
            data = flippedPixels.data();
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, externalFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        this->width = width;
        this->height = height;
        this->channels = channels;
    }

    Texture::Texture(std::span<const uint8_t> data, size_t width, size_t height, size_t channels) {
        glGenTextures(1, &this->id);
        this->setPixels(data, width, height, channels);
    }

    Texture::Texture(const std::filesystem::path &path) {
        glGenTextures(1, &this->id);

        stbi_set_flip_vertically_on_load(true);

        int width, height, channels;
        const std::unique_ptr<uint8_t, LoadedImagePixelsDeleter> data(stbi_load(path.string().data(), &width, &height, &channels, 0));

        this->setPixelsInternal(data.get(), width, height, channels, true, false);
    }

    Texture::Texture(const Texture &other) {
        glGenTextures(1, &(this->id));
        this->setPixelsInternal(other.calculatePixelsInternal(false).data(), other.width, other.height, other.channels, false, false);
    }

    Texture::Texture(Texture &&other) {this->moveFrom(other);}

    bool Texture::exists() const noexcept {return (id != 0);}

    std::vector<uint8_t> Texture::calculatePixels() const {
        return calculatePixelsInternal(true);
    }

    void Texture::setPixels(std::span<const uint8_t> data, size_t width, size_t height, size_t channels) {
        this->setPixelsInternal(data.data(), width, height, channels, false, true);
    }

    size_t Texture::getWidth() const noexcept {return width;}
    size_t Texture::getHeight() const noexcept {return height;}
    size_t Texture::getChannels() const noexcept {return channels;}


    TextureWrapMode Texture::getWrapModeU() const noexcept {return wrapModeU;}

    void Texture::setWrapModeU(TextureWrapMode value) {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(value));
        wrapModeU = value;
    }

    TextureWrapMode Texture::getWrapModeV() const noexcept {return wrapModeV;}

    void Texture::setWrapModeV(TextureWrapMode value) {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(value));
        wrapModeV = value;
    }

    TextureMinificationMode Texture::getMinificationMode() const noexcept {return minificationMode;}

    void Texture::setMinificationMode(TextureMinificationMode value) {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(value));
        minificationMode = value;
    }

    TextureMagnificationMode Texture::getMagnificationMode() const noexcept {return magnificationMode;}

    void Texture::setMagnificationMode(TextureMagnificationMode value) {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(value));
        magnificationMode = value;
    }

    void Texture::slot(int32_t slotNumber) {
        // Set texture to slot for each context
        const auto windows = Window::getInstances();
        for (size_t i = 0; i < windows.size(); ++i) {
            windows[i]->makeContextCurrent();
            glActiveTexture(GL_TEXTURE0 + slotNumber);
            glBindTexture(GL_TEXTURE_2D, id);
        }

        if (std::cmp_less(slottedTextureIds.size(), (slotNumber+1))) slottedTextureIds.resize((slotNumber+1), 0); // Ensure minimum size.
        slottedTextureIds[slotNumber] = id;

        idToSlotMap[id] = slotNumber;
    }

    int32_t Texture::getSlot() const {
        const auto findSlot = idToSlotMap.find(this->id);
        if (findSlot == idToSlotMap.end()) return noSlot;
        else return findSlot->second;
    }

    Texture &Texture::operator=(const Texture &other) {
        (*this) = Texture(other);
        return (*this);
    }

    Texture &Texture::operator=(Texture &&other) {
        this->moveFrom(other);
        return (*this);
    }

    Texture::~Texture() noexcept {destroy();}
}