#include <limitless/models/cube.hpp>

#include <limitless/util/tangent_space.hpp>
#include <limitless/models/mesh.hpp>

using namespace Limitless;

Cube::Cube() : ElementaryModel("cube") {
    //TODO: fix face order
    std::vector<VertexNormalTangent> vertices = {
            // back face
            {{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f, -1.0f}, glm::vec3{0.0f}, {0.0f, 0.0f}}, // bottom-left
            {{0.5f, -0.5f, -0.5f},   {0.0f, 0.0f, -1.0f}, glm::vec3{0.0f}, {1.0f, 0.0f}}, // bottom-right
            {{0.5f,  0.5f, -0.5f},   {0.0f, 0.0f, -1.0f}, glm::vec3{0.0f}, {1.0f, 1.0f}}, // top-right
            {{0.5f,  0.5f, -0.5f},   {0.0f, 0.0f, -1.0f}, glm::vec3{0.0f}, {1.0f, 1.0f}}, // top-right
            {{ -0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, glm::vec3{0.0f}, {0.0f, 1.0f}}, // top-left
            {{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f, -1.0f}, glm::vec3{0.0f}, {0.0f, 0.0f}}, // bottom-left
            // front face
            {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {0.0f, 0.0f}}, // bottom-left
            {{ 0.5f,  0.5f,  0.5f},  {0.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {1.0f, 1.0f}}, // top-right
            {{0.5f, -0.5f,  0.5f},   {0.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {1.0f, 0.0f}}, // bottom-right
            {{0.5f,  0.5f,  0.5f},   {0.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {1.0f, 1.0f}}, // top-right
            {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {0.0f, 0.0f}}, // bottom-left
            {{-0.5f,  0.5f,  0.5f},  {0.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {0.0f, 1.0f}}, // top-left
            // left face
            {{-0.5f,  0.5f,  0.5f},  {-1.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {1.0f, 0.0f}}, // top-right
            {{-0.5f, -0.5f, -0.5f},  {-1.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {0.0f, 1.0f}}, // bottom-left
            {{-0.5f,  0.5f, -0.5f},  {-1.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {1.0f, 1.0f}}, // top-left
            {{ -0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {0.0f, 1.0f}}, // bottom-left
            {{-0.5f,  0.5f,  0.5f},  {-1.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {1.0f, 0.0f}}, // top-right
            {{-0.5f, -0.5f,  0.5f},  {-1.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {0.0f, 0.0f}}, // bottom-right
            // right face
            {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {1.0f, 0.0f}}, // top-left
            {{ 0.5f,  0.5f, -0.5f},  {1.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {1.0f, 1.0f}}, // top-right
            {{ 0.5f, -0.5f, -0.5f},  {1.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {0.0f, 1.0f}}, // bottom-right
            {{ 0.5f, -0.5f, -0.5f},  {1.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, {0.0f, 1.0f}}, // bottom-right
            {{  0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, { 0.0f, 0.0f}}, // bottom-left
            {{  0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, { 1.0f, 0.0f}}, // top-left
            // bottom face
            {{ -0.5f, -0.5f, -0.5f},  {0.0f, -1.0f, 1.0f}, glm::vec3{0.0f}, {0.0f, 1.0f}}, // top-right
            {{  0.5f, -0.5f,  0.5f},  {0.0f, -1.0f, 1.0f}, glm::vec3{0.0f}, {1.0f, 0.0f}}, // bottom-left
            {{  0.5f, -0.5f, -0.5f},  {0.0f, -1.0f, 1.0f}, glm::vec3{0.0f}, {1.0f, 1.0f}}, // top-left
            {{ 0.5f, -0.5f,  0.5f},   {0.0f, -1.0f, 1.0f}, glm::vec3{0.0f}, { 1.0f, 0.0f}}, // bottom-left
            {{ -0.5f, -0.5f, -0.5f},  {0.0f, -1.0f, 1.0f}, glm::vec3{0.0f}, { 0.0f, 1.0f}}, // top-right
            {{ -0.5f, -0.5f,  0.5f},  {0.0f, -1.0f, 1.0f}, glm::vec3{0.0f}, {0.0f, 0.0f}}, // bottom-right
            // top face
            {{ -0.5f,  0.5f, -0.5f},  {0.0f, 1.0f, 1.0f}, glm::vec3{0.0f}, {0.0f, 1.0f}}, // top-left
            {{ 0.5f,  0.5f, -0.5f},   {0.0f, 1.0f, 1.0f}, glm::vec3{0.0f}, { 1.0f, 1.0f}}, // top-right
            {{ 0.5f,  0.5f,  0.5f},   {0.0f, 1.0f, 1.0f}, glm::vec3{0.0f}, { 1.0f, 0.0f}}, // bottom-right
            {{ 0.5f,  0.5f,  0.5f},   {0.0f, 1.0f, 1.0f}, glm::vec3{0.0f}, { 1.0f, 0.0f}}, // bottom-right
            {{ -0.5f,  0.5f,  0.5f},  {0.0f, 1.0f, 1.0f}, glm::vec3{0.0f}, { 0.0f, 0.0f}}, // bottom-left
            {{ -0.5f,  0.5f, -0.5f},  {0.0f, 1.0f, 1.0f}, glm::vec3{0.0f}, { 0.0f, 1.0f}}  // top-left
    };

    calculateTangentSpaceTriangle(vertices);

    meshes.emplace_back(
        std::make_unique<Mesh>(
                std::make_unique<VertexStream<VertexNormalTangent>>(std::move(vertices), VertexStreamUsage::Static, VertexStreamDraw::Triangles),
                "cube_mesh")
        );

    calculateBoundingBox();
}