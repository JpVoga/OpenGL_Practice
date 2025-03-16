#include "shader.hpp"

namespace CG {
    Shader::CompiledShaderStage::CompiledShaderStage(const std::filesystem::path &path) {
        const std::optional<ShaderStage> stageOptional = Shader::getShaderStageFromExtension(path);
        if (!stageOptional.has_value()) {
            this->id = 0;
            return;
        }

        const ShaderStage stage = stageOptional.value();

        std::ifstream file(path);
        if (!file) throw std::runtime_error("Failed to open \"" + path.string() + "\":\n" + std::to_string(std::stacktrace::current()));

        std::string line, src;
        while (std::getline(file, line)) {
            src.append(line);
            src.push_back('\n');
        }

        file.close();

        this->id = glCreateShader(static_cast<GLenum>(stage));
        const char *const srcPtr = src.c_str();
        int compilationSuccessful;

        glShaderSource(this->id, 1, &srcPtr, nullptr);
        glCompileShader(this->id);
        glGetShaderiv(this->id, GL_COMPILE_STATUS, &compilationSuccessful);

        if (!compilationSuccessful) {
            GLsizei messageLength;
            glGetShaderInfoLog(this->id, 0, &messageLength, nullptr);

            std::string msg;
            msg.resize(static_cast<size_t>(messageLength), ' ');

            glGetShaderInfoLog(this->id, messageLength, &messageLength, msg.data());

            throw std::runtime_error("Error compiling shader stage \"" + path.string() + "\":" + msg + '\n' + std::to_string(std::stacktrace::current()));
        }
    }

    Shader::CompiledShaderStage::CompiledShaderStage(Shader::CompiledShaderStage &&other) noexcept: id(other.id) {
        other.id = 0;
    }

    Shader::CompiledShaderStage &Shader::CompiledShaderStage::operator=(Shader::CompiledShaderStage &&other) noexcept {
        this->id = other.id;
        other.id = 0;
        return (*this);
    }

    Shader::CompiledShaderStage::~CompiledShaderStage() noexcept {
        glDeleteShader(this->id);
        this->id = 0;
    }
}