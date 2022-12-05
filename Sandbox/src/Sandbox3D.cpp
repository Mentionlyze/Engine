#include "Sandbox3D.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

Sandbox3D::Sandbox3D() : Layer("Sandbox3D"), m_CameraController(45.0f, 1.6f/0.9f, 0.1f, 100.0f), m_LightPos(glm::vec3(1.2f, 2.0f, 2.0f)) , m_LightColor(glm::vec3(1.0f))
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

    m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Geometry.vert", "assets/shaders/Geometry.frag");
	m_Texture = Engine::Texture2D::Create("assets/textures/CheckerBoard.png");

    m_Shader->Bind();
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("u_Texture", 0);

    auto lightShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/LightCube.vert", "assets/shaders/LightCube.frag");
    lightShader->Bind();
}

void Sandbox3D::OnUpdate(Engine::Timestep ts)
{
    m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Engine::RenderCommand::Clear(true);

    Engine::Renderer::BeginScene(m_CameraController.GetCamera());
    auto lightShader = Engine::Renderer::GetShaderLibrary()->Get("LightCube");
    lightShader->Bind();
    glm::mat4 lightTransform = glm::mat4(1.0f);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(lightShader)->setFloat3("u_LightColor", m_LightColor);
    lightTransform = glm::translate(lightTransform, m_LightPos);
    lightTransform = glm::scale(lightTransform, glm::vec3(0.2f));
    Engine::Renderer::Submit(m_VertexArray, lightShader, lightTransform);

    m_Texture->Bind();
    m_Shader->Bind();
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->setFloat3("u_LightColor", m_LightColor);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->setFloat3("u_LightPos", m_LightPos);
    Engine::Renderer::Submit(m_VertexArray, m_Shader, glm::mat4(1.0f));

    Engine::Renderer::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
    ImGui::Begin("Light");
    ImGui::SliderFloat3("light color", glm::value_ptr(m_LightColor), 0.0f, 1.0f);
    ImGui::End();
}

void Sandbox3D::OnEvent(Engine::Event& e)
{
    m_CameraController.OnEvent(e);
}
