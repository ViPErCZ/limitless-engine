#include <render.hpp>

#include <assets.hpp>
#include <render_settings.hpp>
#include <core/texture_builder.hpp>
#include <material_system/material.hpp>
#include <shader_types.hpp>
#include <elementary_instance.hpp>
#include <camera.hpp>
#include <scene.hpp>
#include <iostream>

using namespace GraphicsEngine;

struct FrontToBackSorter {
    const Camera& camera;

    explicit FrontToBackSorter(const Camera& _camera) noexcept : camera{_camera} {}

    bool operator()(const AbstractInstance* const lhs, const AbstractInstance* const rhs) const noexcept {
        const auto& a_pos = lhs->getPosition();
        const auto& b_pos = rhs->getPosition();
        const auto& c_pos = camera.getPosition();

        return glm::distance(c_pos, a_pos) < glm::distance(c_pos, b_pos);
    }
};

struct BackToFrontSorter {
    const Camera& camera;

    explicit BackToFrontSorter(const Camera& _camera) noexcept : camera{_camera} {}

    bool operator()(const AbstractInstance* const lhs, const AbstractInstance* const rhs) const noexcept {
        const auto& a_pos = lhs->getPosition();
        const auto& b_pos = rhs->getPosition();
        const auto& c_pos = camera.getPosition();

        return glm::distance(c_pos, a_pos) > glm::distance(c_pos, b_pos);
    }
};

void Renderer::dispatchInstances(const std::vector<AbstractInstance*>& instances, Context& context, MaterialShader shader_type, Blending blending) const {
    switch (blending) {
        case Blending::Opaque:
            context.enable(GL_DEPTH_TEST);
            context.setDepthFunc(DepthFunc::Less);
            context.setDepthMask(GL_TRUE);
            context.disable(GL_BLEND);
            break;
        case Blending::Additive:
            context.enable(GL_DEPTH_TEST);
            context.setDepthFunc(DepthFunc::Less);
            context.setDepthMask(GL_FALSE);
            context.enable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        case Blending::Modulate:
            context.enable(GL_DEPTH_TEST);
            context.setDepthFunc(DepthFunc::Less);
            context.setDepthMask(GL_FALSE);
            context.enable(GL_BLEND);
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;
        case Blending::Translucent:
            context.enable(GL_DEPTH_TEST);
            context.setDepthFunc(DepthFunc::Less);
            context.setDepthMask(GL_FALSE);
            context.enable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
    }

    for (auto* const instance : instances) {
        instance->isWireFrame() ? context.setPolygonMode(CullFace::FrontBack, PolygonMode::Line) : context.setPolygonMode(CullFace::FrontBack, PolygonMode::Fill);

        instance->draw(shader_type, blending);
    }
}

void Renderer::renderLightsVolume(Context& context, Scene& scene) const {
    if (scene.lighting.point_lights.empty()) {
        return;
    }

    context.enable(GL_DEPTH_TEST);
    context.setDepthFunc(DepthFunc::Less);
    context.setDepthMask(GL_FALSE);
    context.disable(GL_BLEND);

    static auto sphere_instance = ElementaryInstance(assets.models["sphere"], assets.materials["default"], glm::vec3(0.0f));

    context.setPolygonMode(CullFace::FrontBack, PolygonMode::Line);
    for (const auto& light : scene.lighting.point_lights) {
        sphere_instance.setPosition(light.position);
        sphere_instance.setScale(glm::vec3(light.radius));
        sphere_instance.draw(MaterialShader::Default, Blending::Opaque);
    }
    context.setPolygonMode(CullFace::FrontBack, PolygonMode::Fill);
}

void Renderer::initializeOffscreenBuffer(ContextEventObserver& ctx) {
    auto param_set = [] (Texture& texture) {
        texture << TexParameter<GLint>{GL_TEXTURE_MAG_FILTER, GL_LINEAR}
                << TexParameter<GLint>{GL_TEXTURE_MIN_FILTER, GL_LINEAR};
    };
    auto color0 = TextureBuilder::build(Texture::Type::Tex2D, 1, Texture::InternalFormat::RGBA16F, ctx.getSize(), Texture::Format::RGBA, Texture::DataType::Float, nullptr, param_set);
    auto depth = TextureBuilder::build(Texture::Type::Tex2D, 1, Texture::InternalFormat::Depth32F, ctx.getSize(), Texture::Format::DepthComponent, Texture::DataType::Float, nullptr, param_set);

    offscreen.bind();
    offscreen << TextureAttachment{FramebufferAttachment::Color0, color0}
              << TextureAttachment{FramebufferAttachment::Depth, depth};

    offscreen.drawBuffer(FramebufferAttachment::Color0);
    offscreen.checkStatus();
    offscreen.unbind();
}

Renderer::Renderer(ContextEventObserver& context)
    : postprocess{context}, effect_renderer{context} {
    initializeOffscreenBuffer(context);
}

void Renderer::draw(Context& context, Scene& scene, Camera& camera) {
    scene.update();

    scene_data.update(context, scene, camera);

    context.setViewPort(context.getSize());

    offscreen.bind();
    offscreen.clear();

    //TODO: performing frustum culling; gets vector<instance_ptr>
    auto instances = performFrustumCulling(scene, camera);

    effect_renderer.update(instances);

    // rendering front to back
    std::sort(instances.begin(), instances.end(), FrontToBackSorter{camera});
    dispatchInstances(instances, context, MaterialShader::Default, Blending::Opaque);
    effect_renderer.draw(Blending::Opaque);

    // draws skybox if it exists
    if (scene.getSkybox()) {
        scene.getSkybox()->draw(context);
    }

    // draws lights influence radius
    if (render_settings.light_radius) {
        renderLightsVolume(context, scene);
    }

    // rendering back to front to follow the translucent order
    std::sort(instances.begin(), instances.end(), BackToFrontSorter{camera});

    dispatchInstances(instances, context, MaterialShader::Default, Blending::Additive);
    effect_renderer.draw(Blending::Additive);

    dispatchInstances(instances, context, MaterialShader::Default, Blending::Modulate);
    effect_renderer.draw(Blending::Modulate);

    dispatchInstances(instances, context, MaterialShader::Default, Blending::Translucent);
    effect_renderer.draw(Blending::Translucent);

    offscreen.unbind();

    postprocess.process(context, offscreen);
}

std::vector<AbstractInstance*> Renderer::performFrustumCulling(Scene &scene, [[maybe_unused]] Camera& camera) const noexcept {
    //TODO: culling
    std::vector<AbstractInstance*> culled;
    culled.reserve(scene.size());

    for (const auto& [id, instance] : scene) {
        culled.emplace_back(instance.get());
    }

    return culled;
}
