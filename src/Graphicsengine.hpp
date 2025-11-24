#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>
#include <cstdint>

#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"


class Graphicsengine {
public:
    Graphicsengine(GLFWwindow* window);
    ~Graphicsengine();

    using ObjectId = uint32_t;

    ObjectId createQuad(const std::string& texturePath);
    void draw(ObjectId id, const glm::mat4& model, const glm::vec4& color = glm::vec4(1.0f));
    void clear(const glm::vec4& color);

private:
    struct Buffer {
        VertexArray* vao;
        VertexBuffer* vb;
        IndexBuffer* ib;
        Texture* texture;
    };

    std::unordered_map<ObjectId, Buffer> buffersMap;
    ObjectId nextId = 1;

    Shader* shader = nullptr;
    Renderer* renderer = nullptr;
    glm::mat4 proj;
};
