#include "DepthStencil.h"
#include "glm/gtx/transform.hpp"
#include "glad/glad.h"
#include <map>

DepthStencil::DepthStencil() : Layer("Depth Stencil"),
	m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 100.f)
{
	m_VertexArray = Engine::VertexArray::Create();
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

    auto vertexBuffer = Engine::VertexBuffer::Create(vertices, sizeof(vertices));
    Engine::BufferLayout layout = {
        { Engine::ShaderDataType::Float3, "a_Pos"},
        { Engine::ShaderDataType::Float2, "a_TexCoord"},
        { Engine::ShaderDataType::Float3, "a_Normal"},
    };

    vertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(vertexBuffer);

    uint32_t indices[6 * 6] = {
        0,  1,  2,  2,  3,  0,
        4,  5,  6,  6,  7,  4,
        8,  9,  10, 10, 11, 8, 
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    auto indexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_VertexArray->SetIndexBuffer(indexBuffer);

    m_PlaneVertexArray = Engine::VertexArray::Create();

    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    
    auto planeVertexBuffer = Engine::VertexBuffer::Create(planeVertices, sizeof(planeVertices));
    Engine::BufferLayout plainLayout = {
        { Engine::ShaderDataType::Float3, "a_Pos"},
        { Engine::ShaderDataType::Float2, "a_TexCoord"},
    };
    planeVertexBuffer->SetLayout(plainLayout);
    m_PlaneVertexArray->AddVertexBuffer(planeVertexBuffer);

    uint32_t planeIndices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    auto planeIndexBuffer = Engine::IndexBuffer::Create(planeIndices, sizeof(planeIndices) / sizeof(uint32_t));
    m_PlaneVertexArray->SetIndexBuffer(planeIndexBuffer);

    m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/DepthStencil.vert", "assets/shaders/DepthStencil.frag");
    m_Shader->Bind();

    m_CubeTexture = Engine::Texture2D::Create("assets/textures/metal.png");
    m_FloorTexture = Engine::Texture2D::Create("assets/textures/marble.jpg");

    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("u_Texture", 0);

    m_BorderShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/SingleColor.vert", "assets/shaders/SingleColor.frag");

    m_TransparentVertexArray = Engine::VertexArray::Create();

    float transparentVertices[5 * 4] = {
        0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    auto transparentVertexBuffer = Engine::VertexBuffer::Create(transparentVertices, sizeof(transparentVertices));
    Engine::BufferLayout transparentLayout = {
        { Engine::ShaderDataType::Float3, "a_Pos"},
        { Engine::ShaderDataType::Float2, "a_TexCoord"},
    };
    transparentVertexBuffer->SetLayout(transparentLayout);
    m_TransparentVertexArray->AddVertexBuffer(transparentVertexBuffer);

    uint32_t transparentIndices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    auto transparentIndexBuffer = Engine::IndexBuffer::Create(transparentIndices, sizeof(transparentIndices) / sizeof(uint32_t));
    m_TransparentVertexArray->SetIndexBuffer(transparentIndexBuffer);
    
    m_TransparentShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Blending.vert", "assets/shaders/Blending.frag");
    m_TransparentTexture = Engine::Texture2D::Create("assets/textures/grass.png");
    //m_TransparentTexture = Engine::Texture2D::Create("assets/textures/blending_transparent_window.png");
}

void DepthStencil::OnUpdate(Engine::Timestep ts)
{
    m_CameraController.OnUpdate(ts);

    Engine::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    Engine::RenderCommand::Clear();

    Engine::Renderer::BeginScene(m_CameraController.GetCamera());

    glStencilMask(0x00);

    glm::mat4 transform = glm::mat4(1.0f);

    //m_TransparentShader->Bind();

    m_Shader->Bind(); 
    /*for (uint32_t i = 0; i < m_Vegetation.size(); i++) 
    {
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, m_Vegetation[i]);
        Engine::Renderer::Submit(m_TransparentVertexArray, m_TransparentShader, transform);
    }*/
   
    m_Shader->Bind();

    m_FloorTexture->Bind();
    Engine::Renderer::Submit(m_PlaneVertexArray, m_Shader, transform);


    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    m_CubeTexture->Bind();
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-1.0f, 0.0f, -1.0f));
    Engine::Renderer::Submit(m_VertexArray, m_Shader, transform);

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(2.0f, 0.0f, 0.0f));
    Engine::Renderer::Submit(m_VertexArray, m_Shader, transform);

    //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    //glStencilMask(0x00);
    //glDisable(GL_DEPTH_TEST);

    //m_BorderShader->Bind();
    //m_CubeTexture->Bind();
    //transform = glm::mat4(1.0f);
    //transform = glm::translate(transform, glm::vec3(-1.0f, 0.0f, -1.0f));
    //transform = glm::scale(transform, glm::vec3(1.1f));
    //Engine::Renderer::Submit(m_VertexArray, m_BorderShader, transform);

    //transform = glm::mat4(1.0f);
    //transform = glm::translate(transform, glm::vec3(2.0f, 0.0f, 0.0f));
    //transform = glm::scale(transform, glm::vec3(1.1f));
    //Engine::Renderer::Submit(m_VertexArray, m_BorderShader, transform);

    //glStencilMask(0xFF);
    //glStencilFunc(GL_ALWAYS, 1, 0xFF);
    //glEnable(GL_DEPTH_TEST);

    m_TransparentTexture->Bind();

    std::map<float, glm::vec3> sorted;
    for (uint32_t i = 0; i < m_Vegetation.size(); i++) 
    {
        float distance = glm::length(m_CameraController.GetCamera().GetPosition() - m_Vegetation[i]);
        sorted[distance] = m_Vegetation[i];
    }

    for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) 
    {
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, it->second);
        Engine::Renderer::Submit(m_TransparentVertexArray, m_Shader, transform);
    }


    Engine::Renderer::EndScene();
}

void DepthStencil::OnImGuiRender()
{
}

void DepthStencil::OnEvent(Engine::Event& event)
{
}
