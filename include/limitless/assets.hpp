#pragma once

#include <limitless/util/resource_container.hpp>
#include <limitless/shader_storage.hpp>
#include <limitless/util/filesystem.hpp>

namespace Limitless::ms {
    class Material;
}

namespace Limitless {
    class AbstractMesh;
    class AbstractModel;
    class EffectInstance;
    class Texture;
    class Skybox;
    class FontAtlas;
    class Context;
    class RenderSettings;

    class Assets {
    protected:
        fs::path base_dir;
        fs::path shader_dir;
    public:
        ShaderStorage shaders;
        ResourceContainer<AbstractModel> models;
        ResourceContainer<AbstractMesh> meshes;
        ResourceContainer<Texture> textures;
        ResourceContainer<ms::Material> materials;
        ResourceContainer<Skybox> skyboxes;
        ResourceContainer<EffectInstance> effects;
        ResourceContainer<FontAtlas> fonts;

        explicit Assets(const fs::path& base_dir) noexcept;
        Assets(fs::path base_dir, fs::path shader_dir) noexcept;

        virtual ~Assets() = default;

        virtual void load(Context& context);

        static PassShaders getRequiredPassShaders(const RenderSettings& settings);
        void compileMaterial(Context& ctx, const RenderSettings& settings, const std::shared_ptr<ms::Material>& material);
        void compileEffect(Context& ctx, const RenderSettings& settings, const std::shared_ptr<EffectInstance>& effect);
        void compileSkybox(Context& ctx, const RenderSettings& settings, const std::shared_ptr<Skybox>& skybox);

        virtual void compileShaders(Context& ctx, const RenderSettings& settings);
        void recompileShaders(Context& ctx, const RenderSettings& settings);

        void add(const Assets& other);

        [[nodiscard]] const auto& getBaseDir() const noexcept { return base_dir; }
        [[nodiscard]] const auto& getShaderDir() const noexcept { return shader_dir; }
    };
}
