#include "Skybox.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Skybox::Skybox() : Layer("Skybox"),
    m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 1000.f)
{
    m_SkyboxVertexArray = Engine::VertexArray::Create();

    float skyboxVertices[8 * 24] = {
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,

        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,

        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
    };

    auto skyboxVertexBuffer = Engine::VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));
    Engine::BufferLayout layout = {
        {Engine::ShaderDataType::Float3, "a_Pos"}
    };
    skyboxVertexBuffer->SetLayout(layout);
    m_SkyboxVertexArray->AddVertexBuffer(skyboxVertexBuffer);
    
    uint32_t indices[6 * 6] = {
         0,  1,  2,  2,  3,  0,
         4,  5,  6,  6,  7,  4,
         8,  9,  10, 10, 11, 8,
         12, 13, 14, 14, 15, 12,
         16, 17, 18, 18, 19, 16,
         20, 21, 22, 22, 23, 20
    };

    auto skyboxIndexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_SkyboxVertexArray->SetIndexBuffer(skyboxIndexBuffer);

    m_SkyboxShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Skybox.vert", "assets/shaders/Skybox.frag");
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_SkyboxShader)->SetInt("skybox", 0);

    std::vector<std::string> faces = {
        "assets/textures/skybox/right.jpg",
        "assets/textures/skybox/left.jpg",
        "assets/textures/skybox/top.jpg",
        "assets/textures/skybox/bottom.jpg",
        "assets/textures/skybox/front.jpg",
        "assets/textures/skybox/back.jpg",
    };
    m_SkyboxMap = Engine::TextureCubeMap::Create(faces);

    m_CubeVertexArray = Engine::VertexArray::Create();
    float vertices[8 * 24] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, -1.0f,


        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    };

    auto cubeVertexBuffer = Engine::VertexBuffer::Create(vertices, sizeof(vertices));
    Engine::BufferLayout cubeLayout = {
        { Engine::ShaderDataType::Float3, "a_Pos"},
        { Engine::ShaderDataType::Float2, "a_TexCoord"},
        { Engine::ShaderDataType::Float3, "a_Normal"},
    };

    cubeVertexBuffer->SetLayout(cubeLayout);
    m_CubeVertexArray->AddVertexBuffer(cubeVertexBuffer);

    auto cubeIndexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_CubeVertexArray->SetIndexBuffer(cubeIndexBuffer);

    m_CubeShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/SkyCube.vert", "assets/shaders/SkyCube.frag");
    m_CubeTexture = Engine::Texture2D::Create("assets/textures/container2.png");
}

void Skybox::OnUpdate(Engine::Timestep ts)
{
    m_CameraController.OnUpdate(ts);

    Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });
    Engine::RenderCommand::Clear();

    Engine::Renderer::BeginScene(m_CameraController.GetCamera());

    glm::mat4 transform = glm::mat4(1.0f);

    m_CubeShader->Bind();
    m_SkyboxMap->Bind();
    Engine::Renderer::Submit(m_CubeVertexArray, m_CubeShader, transform);

    glDepthFunc(GL_LEQUAL);
    m_SkyboxShader->Bind();
    m_SkyboxMap->Bind();
    Engine::Renderer::Submit(m_SkyboxVertexArray, m_SkyboxShader, transform, true);

    glDepthFunc(GL_LESS);

    Engine::Renderer::EndScene();
}

void Skybox::OnEvent(Engine::Event& e)
{
}

void Skybox::OnImGuiRender()
{
}
