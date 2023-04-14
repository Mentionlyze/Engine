#include "NormalMapping.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

NormalMapping::NormalMapping() : Layer("Normal Mapping"),
	m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 1000.0f)
{
    m_CameraController.SetPosition({ 0.0, 0.0, 6.0 });

	m_VertexArray = Engine::VertexArray::Create();

    glm::vec3 pos1(-1.0, 1.0, 0.0);
    glm::vec3 pos2(-1.0, -1.0, 0.0);
    glm::vec3 pos3(1.0, -1.0, 0.0);
    glm::vec3 pos4(1.0, 1.0, 0.0);
    // texture coordinates
    glm::vec2 uv1(0.0, 1.0);
    glm::vec2 uv2(0.0, 0.0);
    glm::vec2 uv3(1.0, 0.0);
    glm::vec2 uv4(1.0, 1.0);
    // normal vector
    glm::vec3 nm(0.0, 0.0, 1.0);

    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;

    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent1 = glm::normalize(tangent1);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent1 = glm::normalize(bitangent1);

    // - triangle 2
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent2 = glm::normalize(tangent2);


    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent2 = glm::normalize(bitangent2);

    float vertices[] = {
        // Positions            // normal         // TexCoords  // Tangent                          // Bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };

	auto vertexBuffer = Engine::VertexBuffer::Create(vertices, sizeof(vertices));

    Engine::BufferLayout layout = {
        {Engine::ShaderDataType::Float3, "a_Pos"},
        {Engine::ShaderDataType::Float3, "a_Normal"},
        {Engine::ShaderDataType::Float2, "a_TexCoords"},
        {Engine::ShaderDataType::Float3, "a_Tangent"},
        {Engine::ShaderDataType::Float3, "a_BigTangent"},
    };

    vertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(vertexBuffer);

    uint32_t indices[] = {
        0, 1, 2,
        3, 4, 5
    };
	auto indexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_VertexArray->SetIndexBuffer(indexBuffer);

    m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/NormalMapping.vert", "assets/shaders/NormalMapping.frag");

	m_Texutre = Engine::Texture2D::Create("assets/textures/brickwall.jpg");
	m_NormalTexture = Engine::Texture2D::Create("assets/textures/brickwall_normal.jpg");

    m_Shader->Bind();
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("texture_diffuse", 0);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("texture_normal", 1);
}

void NormalMapping::OnUpdate(Engine::Timestep ts)
{
    m_CameraController.OnUpdate(ts);

    Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });
    Engine::RenderCommand::Clear();

    Engine::Renderer::BeginScene(m_CameraController.GetCamera());

    m_Shader->Bind();
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_LightPos", m_LightPos);
    m_Texutre->Bind();
    m_NormalTexture->Bind(1);

    Engine::Renderer::Submit(m_VertexArray, m_Shader, glm::mat4(1.0f));

    Engine::Renderer::EndScene();
}

void NormalMapping::OnImGuiRender()
{
	ImGui::Begin("Advanced Light");
    ImGui::SliderFloat3("Light Pos", glm::value_ptr(m_LightPos), -5.0, 5.0);
    ImGui::End();
}

void NormalMapping::OnEvent(Engine::Event& e)
{
}
