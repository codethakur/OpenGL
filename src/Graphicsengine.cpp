#include "Graphicsengine.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vendor/glm/gtc/matrix_transform.hpp"

Graphicsengine::Graphicsengine(GLFWwindow* window)
{
    shader = new Shader("res/shaders/Basic.shader");
    renderer = new Renderer();

    int width = 1, height = 1;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (width > 0 && height > 0) ? (float)width / (float)height : 1.0f;
    float orthoHeight = 6.0f / aspect;

    proj = glm::ortho(-3.0f, 3.0f,
                      -orthoHeight / 2.0f, orthoHeight / 2.0f,
                      -1.0f, 1.0f);

    shader->Bind();
    shader->setUniform1i("u_Texture", 0);
}

Graphicsengine::~Graphicsengine()
{
    for (auto &kv :buffersMap) {
        auto &b = kv.second;
        delete b.vb;
        delete b.ib;
        delete b.vao;
        delete b.texture;
    }
    buffersMap.clear();

    delete shader;
    delete renderer;
}

Graphicsengine::ObjectId Graphicsengine::createQuad(const std::string& texturePath)
{
    float positions[] = {
        -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 1.0f
    };

    unsigned indices[] = { 0, 1, 2, 2, 3, 0 };

    auto* vao = new VertexArray();
    auto* vb = new VertexBuffer(positions, sizeof(positions));
    auto* ib = new IndexBuffer(indices, 6);

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    vao->addBuffer(*vb, layout);

    auto* tex = new Texture(texturePath);

    ObjectId id = nextId++;
    buffersMap[id] = { vao, vb, ib, tex };
    return id;
}

void Graphicsengine::draw(ObjectId id, const glm::mat4& model, const glm::vec4& color)
{
    auto it = buffersMap.find(id);
    if (it == buffersMap.end()) return;
    auto &m = it->second;

    glm::mat4 mvp = proj * model;

    shader->Bind();
    shader->setUniform1i("u_Texture", 0);
    shader->setUniformMat4f("u_MVP", mvp);
    shader->setUniform4f("u_Color", color.r, color.g, color.b, color.a);

    m.texture->Bind(0);
    renderer->Draw(*m.vao, *m.ib, *shader);
}

void Graphicsengine::clear(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    renderer->Clear();
}
