#pragma once

#include "_cgControl.hpp"
#include "hash.hpp"

namespace CG {
    enum class ShaderStage {
        Vertex         = GL_VERTEX_SHADER         ,
        TessControl    = GL_TESS_CONTROL_SHADER   ,
        TessEvaluation = GL_TESS_EVALUATION_SHADER,
        Geometry       = GL_GEOMETRY_SHADER       ,
        Fragment       = GL_FRAGMENT_SHADER       ,
        Compute        = GL_COMPUTE_SHADER
    };


    class Shader {
        private:
            // Move only RAII wrapper for compiled shader stage
            struct CompiledShaderStage {
                uint32_t id = 0;

                CompiledShaderStage() = delete;
                CompiledShaderStage(const CompiledShaderStage&) = delete;
                CompiledShaderStage(const std::filesystem::path &path);
                CompiledShaderStage(CompiledShaderStage &&other) noexcept;

                CompiledShaderStage& operator=(CompiledShaderStage &&other) noexcept;

                CompiledShaderStage& operator=(const CompiledShaderStage&) = delete;

                ~CompiledShaderStage() noexcept;
            };

            static std::optional<ShaderStage> getShaderStageFromExtension(const std::filesystem::path &path);
            static CompiledShaderStage compileShaderStage(const std::filesystem::path &path);

            std::filesystem::path path = "";
            uint32_t id = 0;
            std::unordered_map<std::string, int32_t, StringViewHash, StringViewEqual> uniformLocations;

            void clearFields() noexcept;
            void moveFrom(Shader &other);
            void destroy();
            void initializeUniformLocations();
            int32_t getUniformLocation(std::string_view name) const;

        public:
            Shader() = delete;
            Shader(const Shader &);
            explicit Shader(const std::filesystem::path &path);
            Shader(Shader &&other);

            Shader &operator=(const Shader &);
            Shader &operator=(Shader &&other);

            bool exists() const noexcept;
            void bind() const noexcept;

            template<typename T> void setUniform(std::string_view name, const T &value) requires (!(std::is_pointer_v<T>) && !(std::is_array_v<T>));
            template<typename T> void setUniform(std::string_view name, std::span<const T> values);

            ~Shader() noexcept;
    };
}