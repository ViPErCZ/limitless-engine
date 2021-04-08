#pragma once

#include <limitless/material_system/material_compiler.hpp>

namespace LimitlessEngine {
    class Emitter;
    class SpriteEmitter;
    class MeshEmitter;
    class EffectInstance;

    class EffectCompiler : public MaterialCompiler {
    private:
        std::string getEmitterDefines(const Emitter& emitter) noexcept;
        void compile(const SpriteEmitter& emitter);
        void compile(const MeshEmitter& emitter);
    public:
        explicit EffectCompiler(Context& context, Assets& assets) : MaterialCompiler{context, assets} {}

        using ShaderCompiler::compile;
        void compile(const EffectInstance& instance);
    };
}