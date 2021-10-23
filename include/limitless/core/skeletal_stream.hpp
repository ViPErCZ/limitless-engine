#pragma once

#include <limitless/core/indexed_stream.hpp>

namespace Limitless {
    template <typename Vertex>
    class SkinnedVertexStream : public IndexedVertexStream<Vertex> {
    private:
        std::vector<VertexBoneWeight> bone_weights;
        std::shared_ptr<Buffer> bone_buffer;

        void initialize() {
            BufferBuilder builder;
            bone_buffer = builder.setTarget(Buffer::Type::Array)
                    .setUsage(Buffer::Storage::Static)
                    .setAccess(Buffer::ImmutableAccess::None)
                    .setData(bone_weights.data())
                    .setDataSize(bone_weights.size() * sizeof(VertexBoneWeight))
                    .build();

            this->vertex_array.template setAttribute<glm::ivec4>(4, false, sizeof(VertexBoneWeight), (GLvoid*)offsetof(VertexBoneWeight, bone_index), bone_buffer);
            this->vertex_array.template setAttribute<glm::vec4>(5, false, sizeof(VertexBoneWeight), (GLvoid*)offsetof(VertexBoneWeight, weight), bone_buffer);
        };
    public:
        SkinnedVertexStream(std::vector<Vertex>&& vertices, std::vector<typename IndexedVertexStream<Vertex>::index_type>&& indices, std::vector<VertexBoneWeight>&& bones, VertexStreamUsage usage, VertexStreamDraw draw)
            : IndexedVertexStream<Vertex>{std::move(vertices), std::move(indices), usage, draw}
            , bone_weights {std::move(bones)} {
            initialize();
        }

        auto& getBoneWeights() noexcept { return bone_weights; }
        const auto& getBoneWeights() const noexcept { return bone_weights; }
    };
}
