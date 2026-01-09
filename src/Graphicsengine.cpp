#include "Graphicsengine.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vendor/glm/gtc/matrix_transform.hpp"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------
Graphicsengine::Graphicsengine(GLFWwindow* window)
{
    shader   = new Shader("res/shaders/Basic.shader");
    texture  = new Texture("res/textures/codethakur.png");
    renderer = new Renderer();

    // ✅ REQUIRED FOR 3D
    glEnable(GL_DEPTH_TEST);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    float aspect = (float)width / (float)height;
    proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    view = glm::translate(glm::mat4(1.0f),
                          glm::vec3(0.0f, 0.0f, -3.0f));
}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------
Graphicsengine::~Graphicsengine()
{
    delete triangleVAO;
    delete triangleVB;
    delete triangleIB;

    delete texture;
    delete shader;
    delete renderer;
}

// ------------------------------------------------------------
// Init cube (24 vertices, ALL faces textured)
// ------------------------------------------------------------
void Graphicsengine::initTriangle()
{
    if (triangleInitialized)
        return;

    float vertices[] = {
        // ---------- FRONT ----------
        -0.3f,-0.3f, 0.3f,  -0.2f,-0.2f,
        0.3f,-0.3f, 0.3f,   1.2f,-0.2f,
        0.3f, 0.3f, 0.3f,   1.2f, 1.2f,
        -0.3f, 0.3f, 0.3f,  -0.2f, 1.2f,

        // ---------- BACK ----------
         0.3f,-0.3f,-0.3f,  -0.2f,-0.2f,
        -0.3f,-0.3f,-0.3f,  1.2f,-0.2f,
        -0.3f, 0.3f,-0.3f,  1.2f, 1.2f,
         0.3f, 0.3f,-0.3f,  -0.2f, 1.2f,

        // ---------- LEFT ----------
        -0.3f,-0.3f,-0.3f,  -0.2f,-0.2f,
        -0.3f,-0.3f, 0.3f,  1.2f,-0.2f,
        -0.3f, 0.3f, 0.3f,  1.2f, 1.2f,
        -0.3f, 0.3f,-0.3f,  -0.2f, 1.2f,

        // ---------- RIGHT ----------
         0.3f,-0.3f, 0.3f,  -0.2f,-0.2f,
         0.3f,-0.3f,-0.3f,  1.2f,-0.2f,
         0.3f, 0.3f,-0.3f,  1.2f, 1.2f,
         0.3f, 0.3f, 0.3f,  -0.2f, 1.2f,

        // ---------- TOP ----------
        -0.3f, 0.3f, 0.3f,  -0.2f,-0.2f,
         0.3f, 0.3f, 0.3f,  1.2f,-0.2f,
         0.3f, 0.3f,-0.3f,  1.2f, 1.2f,
        -0.3f, 0.3f,-0.3f,  -0.2f, 1.2f,

        // ---------- BOTTOM ----------
        -0.3f,-0.3f,-0.3f,  -0.2f,-0.2f,
         0.3f,-0.3f,-0.3f,  1.2f,-0.2f,
         0.3f,-0.3f, 0.3f,  1.2f, 1.2f,
        -0.3f,-0.3f, 0.3f,  -0.2f, 1.2f,    }
        ;

    unsigned int indices[] = {
         0,  1,  2,  2,  3,  0,
         4,  5,  6,  6,  7,  4,
         8,  9, 10, 10, 11,  8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    triangleVAO = new VertexArray();
    triangleVB  = new VertexBuffer(vertices, sizeof(vertices));
    triangleIB  = new IndexBuffer(indices, 36);

    VertexBufferLayout layout;
    layout.Push<float>(3); // position
    layout.Push<float>(2); // texcoord

    triangleVAO->addBuffer(*triangleVB, layout);

    triangleInitialized = true;
}

// ------------------------------------------------------------
// Draw cube
// ------------------------------------------------------------
void Graphicsengine::drawTriangle(const glm::mat4& model,
                                  const glm::vec4& color)
{
    initTriangle();

    glm::mat4 mvp = proj * view * model;

    shader->Bind();
    shader->setUniformMat4f("u_MVP", mvp);
   shader->setUniform4f("u_Color", color.r, color.g, color.b, color.a);

    shader->setUniform1i("u_Texture", 0);

    texture->Bind(0);

    float borderColor[] = { 0.744f, 0.907f, 0.702f, 1.0f};

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    renderer->Draw(*triangleVAO, *triangleIB, *shader);
}





// ------------------------------------------------------------
// Clear
// ------------------------------------------------------------
void Graphicsengine::clear(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

