#pragma once

#include <limitless/instances/material_instance.hpp>

namespace LimitlessEngine {
    class AbstractMesh;

    class MeshInstance {
    private:
        std::shared_ptr<AbstractMesh> mesh;
        MaterialInstance material;
        bool hidden {};
    public:
        MeshInstance(std::shared_ptr<AbstractMesh> mesh, const std::shared_ptr<Material>& material) noexcept;

        [[nodiscard]] auto& getMaterial() noexcept { return material; }
        [[nodiscard]] bool isHidden() const noexcept { return hidden; }

        void hide() noexcept;
        void reveal() noexcept;

        void draw() const noexcept;
    };
}