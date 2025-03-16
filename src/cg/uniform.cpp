#include "shader.hpp"

namespace CG {
    // Scalars:

    template<>
    void Shader::setUniform<int32_t>(std::string_view name, const int32_t &value) {
        bind();
        glUniform1i(getUniformLocation(name), value);
    }

    template<>
    void Shader::setUniform<int32_t>(std::string_view name, std::span<const int32_t> values) {
        bind();
        glUniform1iv(getUniformLocation(name), values.size(), values.data());
    }

    template<>
    void Shader::setUniform<uint32_t>(std::string_view name, const uint32_t &value) {
        bind();
        glUniform1ui(getUniformLocation(name), value);
    }

    template<>
    void Shader::setUniform<uint32_t>(std::string_view name, std::span<const uint32_t> values) {
        bind();
        glUniform1uiv(getUniformLocation(name), values.size(), values.data());
    }

    template<>
    void Shader::setUniform<float32_t>(std::string_view name, const float32_t &value) {
        bind();
        glUniform1f(getUniformLocation(name), value);
    }

    template<>
    void Shader::setUniform<float32_t>(std::string_view name, std::span<const float32_t> values) {
        bind();
        glUniform1fv(getUniformLocation(name), values.size(), values.data());
    }

    template<>
    void Shader::setUniform<float64_t>(std::string_view name, const float64_t &value) {
        bind();
        glUniform1d(getUniformLocation(name), value);
    }

    template<>
    void Shader::setUniform<float64_t>(std::string_view name, std::span<const float64_t> values) {
        bind();
        glUniform1dv(getUniformLocation(name), values.size(), values.data());
    }


    // Int32 vectors:
    
    template<>
    void Shader::setUniform<Vector1Int32>(std::string_view name, const Vector1Int32 &value) {
        bind();
        glUniform1i(getUniformLocation(name), value.x);
    }

    template<>
    void Shader::setUniform<Vector1Int32>(std::string_view name, std::span<const Vector1Int32> values) {
        bind();
        glUniform1iv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Vector2Int32>(std::string_view name, const Vector2Int32 &value) {
        bind();
        glUniform2i(getUniformLocation(name), value.x, value.y);
    }

    template<>
    void Shader::setUniform<Vector2Int32>(std::string_view name, std::span<const Vector2Int32> values) {
        bind();
        glUniform2iv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Vector3Int32>(std::string_view name, const Vector3Int32 &value) {
        bind();
        glUniform3i(getUniformLocation(name), value.x, value.y, value.z);
    }

    template<>
    void Shader::setUniform<Vector3Int32>(std::string_view name, std::span<const Vector3Int32> values) {
        bind();
        glUniform3iv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Vector4Int32>(std::string_view name, const Vector4Int32 &value) {
        bind();
        glUniform4i(getUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    template<>
    void Shader::setUniform<Vector4Int32>(std::string_view name, std::span<const Vector4Int32> values) {
        bind();
        glUniform4iv(getUniformLocation(name), values.size(), &(values[0].x));
    }


    // UInt32 vectors:

    template<>
    void Shader::setUniform<Vector1UInt32>(std::string_view name, const Vector1UInt32 &value) {
        bind();
        glUniform1ui(getUniformLocation(name), value.x);
    }

    template<>
    void Shader::setUniform<Vector1UInt32>(std::string_view name, const std::span<const Vector1UInt32> values) {
        bind();
        glUniform1uiv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Vector2UInt32>(std::string_view name, const Vector2UInt32 &value) {
        bind();
        glUniform2ui(getUniformLocation(name), value.x, value.y);
    }

    template<>
    void Shader::setUniform<Vector2UInt32>(std::string_view name, std::span<const Vector2UInt32> values) {
        bind();
        glUniform2uiv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Vector3UInt32>(std::string_view name, const Vector3UInt32 &value) {
        bind();
        glUniform3ui(getUniformLocation(name), value.x, value.y, value.z);
    }

    template<>
    void Shader::setUniform<Vector3UInt32>(std::string_view name, std::span<const Vector3UInt32> values) {
        bind();
        glUniform3uiv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Vector4UInt32>(std::string_view name, const Vector4UInt32 &value) {
        bind();
        glUniform4ui(getUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    template<>
    void Shader::setUniform<Vector4UInt32>(std::string_view name, std::span<const Vector4UInt32> values) {
        bind();
        glUniform4uiv(getUniformLocation(name), values.size(), &(values[0].x));
    }


    // Float32 vectors:

    template<>
    void Shader::setUniform<Vector1>(std::string_view name, const Vector1 &value) {
        bind();
        glUniform1f(getUniformLocation(name), value.x);
    }

    template<>
    void Shader::setUniform<Vector1>(std::string_view name, std::span<const Vector1> values) {
        bind();
        glUniform1fv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Vector2>(std::string_view name, const Vector2 &value) {
        bind();
        glUniform2f(getUniformLocation(name), value.x, value.y);
    }

    template<>
    void Shader::setUniform<Vector2>(std::string_view name, std::span<const Vector2> values) {
        bind();
        glUniform2fv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Vector3>(std::string_view name, const Vector3 &value) {
        bind();
        glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
    }

    template<>
    void Shader::setUniform<Vector3>(std::string_view name, std::span<const Vector3> values) {
        bind();
        glUniform3fv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Vector4>(std::string_view name, const Vector4 &value) {
        bind();
        glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    template<>
    void Shader::setUniform<Vector4>(std::string_view name, std::span<const Vector4> values) {
        bind();
        glUniform4fv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Color>(std::string_view name, const Color &value) {
        bind();
        glUniform4f(getUniformLocation(name), value.r, value.g, value.b, value.a);
    }

    template<>
    void Shader::setUniform<Color>(std::string_view name, std::span<const Color> values) {
        bind();
        glUniform4fv(getUniformLocation(name), values.size(), &(values[0].r));
    }


    // Float64 vectors:

    template<>
    void Shader::setUniform<Vector1Flt64>(std::string_view name, const Vector1Flt64 &value) {
        bind();
        glUniform1d(getUniformLocation(name), value.x);
    }

    template<>
    void Shader::setUniform<Vector1Flt64>(std::string_view name, std::span<const Vector1Flt64> values) {
        bind();
        glUniform1dv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Vector2Flt64>(std::string_view name, const Vector2Flt64 &value) {
        bind();
        glUniform2d(getUniformLocation(name), value.x, value.y);
    }

    template<>
    void Shader::setUniform<Vector2Flt64>(std::string_view name, const std::span<const Vector2Flt64> values) {
        bind();
        glUniform2dv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Vector3Flt64>(std::string_view name, const Vector3Flt64 &value) {
        bind();
        glUniform3d(getUniformLocation(name), value.x, value.y, value.z);
    }

    template<>
    void Shader::setUniform<Vector3Flt64>(std::string_view name, std::span<const Vector3Flt64> values) {
        bind();
        glUniform3dv(getUniformLocation(name), values.size(), &(values[0].x));
    }

    template<>
    void Shader::setUniform<Vector4Flt64>(std::string_view name, const Vector4Flt64 &value) {
        bind();
        glUniform4d(getUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    template<>
    void Shader::setUniform<Vector4Flt64>(std::string_view name, std::span<const Vector4Flt64> values) {
        bind();
        glUniform4dv(getUniformLocation(name), values.size(), &(values[0].x));
    }


    // Float32 matrices:

    template<>
    void Shader::setUniform<Matrix2>(std::string_view name, const Matrix2 &value) {
        bind();
        glUniformMatrix2fv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix2>(std::string_view name, std::span<const Matrix2> values) {
        bind();
        glUniformMatrix2fv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix2x3>(std::string_view name, const Matrix2x3 &value) {
        bind();
        glUniformMatrix2x3fv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix2x3>(std::string_view name, std::span<const Matrix2x3> values) {
        bind();
        glUniformMatrix2x3fv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix2x4>(std::string_view name, const Matrix2x4 &value) {
        bind();
        glUniformMatrix2x4fv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix2x4>(std::string_view name, std::span<const Matrix2x4> values) {
        bind();
        glUniformMatrix2x4fv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix3x2>(std::string_view name, const Matrix3x2 &value) {
        bind();
        glUniformMatrix3x2fv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix3x2>(std::string_view name, std::span<const Matrix3x2> values) {
        bind();
        glUniformMatrix3x2fv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix3>(std::string_view name, const Matrix3 &value) {
        bind();
        glUniformMatrix3fv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix3>(std::string_view name, std::span<const Matrix3> values) {
        bind();
        glUniformMatrix3fv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix3x4>(std::string_view name, const Matrix3x4 &value) {
        bind();
        glUniformMatrix3x4fv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix3x4>(std::string_view name, std::span<const Matrix3x4> values) {
        bind();
        glUniformMatrix3x4fv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix4x2>(std::string_view name, const Matrix4x2 &value) {
        bind();
        glUniformMatrix4x2fv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix4x2>(std::string_view name, std::span<const Matrix4x2> values) {
        bind();
        glUniformMatrix4x2fv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix4x3>(std::string_view name, const Matrix4x3 &value) {
        bind();
        glUniformMatrix4x3fv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix4x3>(std::string_view name, std::span<const Matrix4x3> values) {
        bind();
        glUniformMatrix4x3fv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix4>(std::string_view name, const Matrix4 &value) {
        bind();
        glUniformMatrix4fv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix4>(std::string_view name, std::span<const Matrix4> values) {
        bind();
        glUniformMatrix4fv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }


    // Float64 matrices:

    template<>
    void Shader::setUniform<Matrix2Flt64>(std::string_view name, const Matrix2Flt64 &value) {
        bind();
        glUniformMatrix2dv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix2Flt64>(std::string_view name, std::span<const Matrix2Flt64> values) {
        bind();
        glUniformMatrix2dv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix2x3Flt64>(std::string_view name, const Matrix2x3Flt64 &value) {
        bind();
        glUniformMatrix2x3dv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix2x3Flt64>(std::string_view name, std::span<const Matrix2x3Flt64> values) {
        bind();
        glUniformMatrix2x3dv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix2x4Flt64>(std::string_view name, const Matrix2x4Flt64 &value) {
        bind();
        glUniformMatrix2x4dv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix2x4Flt64>(std::string_view name, std::span<const Matrix2x4Flt64> values) {
        bind();
        glUniformMatrix2x4dv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix3x2Flt64>(std::string_view name, const Matrix3x2Flt64 &value) {
        bind();
        glUniformMatrix3x2dv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix3x2Flt64>(std::string_view name, std::span<const Matrix3x2Flt64> values) {
        bind();
        glUniformMatrix3x2dv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix3Flt64>(std::string_view name, const Matrix3Flt64 &value) {
        bind();
        glUniformMatrix3dv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix3Flt64>(std::string_view name, std::span<const Matrix3Flt64> values) {
        bind();
        glUniformMatrix3dv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix3x4Flt64>(std::string_view name, const Matrix3x4Flt64 &value) {
        bind();
        glUniformMatrix3x4dv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix3x4Flt64>(std::string_view name, std::span<const Matrix3x4Flt64> values) {
        bind();
        glUniformMatrix3x4dv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix4x2Flt64>(std::string_view name, const Matrix4x2Flt64 &value) {
        bind();
        glUniformMatrix4x2dv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix4x2Flt64>(std::string_view name, std::span<const Matrix4x2Flt64> values) {
        bind();
        glUniformMatrix4x2dv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix4x3Flt64>(std::string_view name, const Matrix4x3Flt64 &value) {
        bind();
        glUniformMatrix4x3dv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix4x3Flt64>(std::string_view name, std::span<const Matrix4x3Flt64> values) {
        bind();
        glUniformMatrix4x3dv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }

    template<>
    void Shader::setUniform<Matrix4Flt64>(std::string_view name, const Matrix4Flt64 &value) {
        bind();
        glUniformMatrix4dv(getUniformLocation(name), 1, false, &value.at(0, 0));
    }

    template<>
    void Shader::setUniform<Matrix4Flt64>(std::string_view name, std::span<const Matrix4Flt64> values) {
        bind();
        glUniformMatrix4dv(getUniformLocation(name), values.size(), false, &(values[0].at(0, 0)));
    }
}