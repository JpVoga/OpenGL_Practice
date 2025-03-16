#include "shader.hpp"

namespace CG {
    std::optional<ShaderStage> Shader::getShaderStageFromExtension(const std::filesystem::path &path) {
        const auto extension = path.extension();

             if (extension.compare(".vert") == 0) return ShaderStage::Vertex        ;
        else if (extension.compare(".tesc") == 0) return ShaderStage::TessControl   ;
        else if (extension.compare(".tese") == 0) return ShaderStage::TessEvaluation;
        else if (extension.compare(".geom") == 0) return ShaderStage::Geometry      ;
        else if (extension.compare(".frag") == 0) return ShaderStage::Fragment      ;
        else if (extension.compare(".comp") == 0) return ShaderStage::Compute       ;

        return std::nullopt;
    }

    void Shader::clearFields() noexcept {
        path.clear();
        id = 0;
        uniformLocations.clear();
    }

    void Shader::moveFrom(Shader &other) {
        if (this == &other) return;

        path = std::move(other.path);
        id = other.id;
        uniformLocations = std::move(other.uniformLocations);

        other.clearFields();
    }

    void Shader::destroy() {
        if (exists()) glDeleteProgram(id);
        clearFields();
    }

    void Shader::initializeUniformLocations() {
        int32_t uniformCount;
        glGetProgramiv(this->id, GL_ACTIVE_UNIFORMS, &uniformCount);
        this->uniformLocations.reserve(static_cast<size_t>(uniformCount));

        for (size_t i = 0; i < static_cast<size_t>(uniformCount); ++i) {
            int32_t maxUniformSizeWithNullTerminator; // The character count of the largest uniform name, counting the null terminator
            std::string name = "";

            glGetProgramiv(this->id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformSizeWithNullTerminator); // This counts the null terminator
            name.resize(static_cast<size_t>(maxUniformSizeWithNullTerminator-1), '\0');                // This does not!
            glGetActiveUniformName(this->id, i, maxUniformSizeWithNullTerminator, &maxUniformSizeWithNullTerminator, name.data());
            while (true) { // Remove array square brackets from name
                const size_t start = name.find('[');
                const size_t end   = name.find(']');

                if ((start == std::string::npos) && (end == std::string::npos)) break;
                else if ((start != std::string::npos) && (end == std::string::npos)) name.erase(start, 1);
                else if ((start == std::string::npos) && (end != std::string::npos)) name.erase(end, 1);
                else name.erase(start, (end - start));
            }
            while (name[name.size() - 1] == '\0') name.pop_back(); // Eliminate null terminator characters at the end

            this->uniformLocations[name] = glGetUniformLocation(this->id, name.c_str());
        }
    }

    int32_t Shader::getUniformLocation(std::string_view name) const {
        return uniformLocations.find(name)->second;
    }

    Shader::Shader(const Shader &other): Shader(other.path) {}

    Shader::Shader(const std::filesystem::path &path) {
        using namespace std::string_literals;

        const YAML::Node file = YAML::LoadFile(path.string());
        ASSERT((!(file["vertex"].IsNull())) && (!(file["fragment"].IsNull())));

        const std::array<CompiledShaderStage, 6> stages = {
            CompiledShaderStage(file["vertex"        ].as<std::string>(""s)),
            CompiledShaderStage(file["tessControl"   ].as<std::string>(""s)),
            CompiledShaderStage(file["tessEvaluation"].as<std::string>(""s)),
            CompiledShaderStage(file["geometry"      ].as<std::string>(""s)),
            CompiledShaderStage(file["fragment"      ].as<std::string>(""s)),
            CompiledShaderStage(file["compute"       ].as<std::string>(""s))
        };

        this->id = glCreateProgram();

        for (const auto &stage: stages) {
            if (stage.id != 0) glAttachShader(this->id, stage.id);
        }

        glLinkProgram(this->id);
        glValidateProgram(this->id);

        initializeUniformLocations();
    }

    Shader::Shader(Shader &&other) {
        this->moveFrom(other);
    }

    Shader &Shader::operator=(const Shader &other) {
        (*this) = Shader(other.path);
        return (*this);
    }

    Shader &Shader::operator=(Shader &&other) {
        this->moveFrom(other);
        return (*this);
    }

    bool Shader::exists() const noexcept {return (id != 0);}

    void Shader::bind() const noexcept {glUseProgram(id);}

    Shader::~Shader() noexcept {destroy();}
}