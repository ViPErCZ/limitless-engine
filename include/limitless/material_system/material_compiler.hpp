#pragma once

#include <limitless/core/shader_compiler.hpp>

namespace LimitlessEngine {
    enum class MaterialShader;
    enum class ModelShader;
    class CustomMaterial;
    class Material;
    class Assets;

    class MaterialCompiler : public ShaderCompiler {
    protected:
        Assets& assets;

        std::string getCustomMaterialScalarUniforms(const CustomMaterial& material) noexcept;
        std::string getCustomMaterialSamplerUniforms(const CustomMaterial& material) noexcept;
        std::string getMaterialDefines(const Material& material) noexcept;
        std::string getModelDefines(const ModelShader& type);

        void replaceMaterialSettings(Shader& shader, const Material& material, ModelShader model_shader) noexcept;
        void replaceRenderSettings(Shader& src) noexcept;
    public:
        MaterialCompiler(Context& context, Assets& assets) noexcept;
        ~MaterialCompiler() override = default;

        using ShaderCompiler::compile;
        void compile(const Material& material, MaterialShader material_shader, ModelShader model_shader);
    };
}
