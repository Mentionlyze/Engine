#include "MultiLight.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

MuitLightLayer::MuitLightLayer() : Layer("Sandbox3D"),
    m_CameraController(45.0f, 1.6f / 0.9f, 0.1f, 100.0f),
    m_Material(Engine::CreateRef<Material>()),
    m_DirectLight(Engine::CreateRef<DirectLight>()),
    m_SpotLight(Engine::CreateRef<SpotLight>()),
    m_PointLight(Engine::CreateRef<PointLight>()),
    m_PointLightColor(glm::vec3(1.0f, 1.0f, 1.0f))
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

    ENGINE_INFO("1, {0}", m_PointLightPositions[0].x);
    ENGINE_INFO("2, {0}", m_PointLightPositions[1].x);
    ENGINE_INFO("3, {0}", m_PointLightPositions[2].x);
    ENGINE_INFO("4, {0}", m_PointLightPositions[3].x);

    auto indexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_VertexArray->SetIndexBuffer(indexBuffer);

    m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/MultiLight.vert", "assets/shaders/MultiLight.frag");
    m_Texture = Engine::Texture2D::Create("assets/textures/container2.png");
    m_SpecTexture = Engine::Texture2D::Create("assets/textures/container2_specular.png");

    m_Shader->Bind();
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("u_Material.diffuse", 0);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetInt("u_Material.specular", 1);

    auto lightShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/LightCube.vert", "assets/shaders/LightCube.frag");
    lightShader->Bind();
}

void MuitLightLayer::OnUpdate(Engine::Timestep ts)
{
    m_CameraController.OnUpdate(ts);

    Engine::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
    Engine::RenderCommand::Clear(true);

    glm::mat4 viewMatrix = m_CameraController.GetCamera().GetViewMatrix();
    glm::vec3 viewPosition = m_CameraController.GetCamera().GetPosition();
    glm::vec3 cameraFoawardDirection = m_CameraController.GetCamera().GetForwardDirection();

    Engine::Renderer::BeginScene(m_CameraController.GetCamera());
    auto lightShader = Engine::Renderer::GetShaderLibrary()->Get("LightCube");
    lightShader->Bind();
    std::dynamic_pointer_cast<Engine::OpenGLShader>(lightShader)->SetFloat3("u_LightColor", m_PointLight->specular);

    std::string pointLightPrefix = "u_PointLights[";

    for (uint32_t i = 0; i < 4; i++)
    {
        
        m_Shader->Bind();
        std::string positionKey = pointLightPrefix + std::to_string(i) + "].position";
        std::string constantKey = pointLightPrefix + std::to_string(i) + "].constant";
        std::string linearKey = pointLightPrefix + std::to_string(i) + "].linear";
        std::string quadraticKey = pointLightPrefix + std::to_string(i) + "].quadratic";
        std::string ambientKey = pointLightPrefix + std::to_string(i) + "].ambient";
        std::string diffuseKey = pointLightPrefix + std::to_string(i) + "].diffuse";
        std::string specularKey = pointLightPrefix + std::to_string(i) + "].specular";

        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3(positionKey, m_PointLightPositions[i]);
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat(constantKey, m_PointLight->constant);
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat(linearKey, m_PointLight->linear);
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat(quadraticKey, m_PointLight->quadratic);
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3(ambientKey, m_PointLight->ambient);
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3(diffuseKey, m_PointLight->diffuse);
        std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3(specularKey, m_PointLight->specular);

        lightShader->Bind();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_PointLightPositions[i]);
        transform = glm::scale(transform, glm::vec3(0.2f));

        Engine::Renderer::Submit(m_VertexArray, lightShader, transform);
    }

    m_Texture->Bind();
    m_SpecTexture->Bind(1);
    m_Shader->Bind();


    m_DirectLight->ambient = m_DirectLight->specular * 0.1f;
    m_DirectLight->diffuse= m_DirectLight->specular * 0.8f;

    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetMat4("u_View", viewMatrix);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_ViewPosition", viewPosition);

    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_Material.ambient", m_Material->ambient);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("u_Material.shininess", m_Material->shininess);

    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_DirectLight.direction", m_DirectLight->direction);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_DirectLight.ambient", m_DirectLight->ambient);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_DirectLight.diffuse", m_DirectLight->diffuse);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_DirectLight.diffuse", m_DirectLight->specular);

    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_SpotLight.position", viewPosition);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_SpotLight.direction", cameraFoawardDirection);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_SpotLight.ambient", m_SpotLight->ambient);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_SpotLight.diffuse", m_SpotLight->diffuse);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("u_SpotLight.specular", m_SpotLight->specular);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("u_SpotLight.constant", m_SpotLight->constant);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("u_SpotLight.linear", m_SpotLight->linear);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("u_SpotLight.specular", m_SpotLight->quadratic);
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("u_SpotLight.cutoff", glm::cos(glm::radians(m_SpotLight->cutoff)));
    std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("u_SpotLight.outerCutoff", glm::cos(glm::radians(m_SpotLight->outerCutoff)));

   
    for (uint32_t i = 0; i < 10; i++)
    {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, m_CubePositions[i]);
        float angle = i * 20.0f;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        Engine::Renderer::Submit(m_VertexArray, m_Shader, transform);
    }

   
    Engine::Renderer::EndScene();
}

void MuitLightLayer::OnImGuiRender()
{
    ImGui::Begin("Light");
    ImGui::SliderFloat3("light color", glm::value_ptr(m_DirectLight->specular), 0.0f, 1.0f);
    ImGui::SliderFloat3("light position", glm::value_ptr(m_DirectLight->direction), -5.0f, 5.0f);
    ImGui::SliderFloat3("pointlight color", glm::value_ptr(m_PointLight->specular), 0.0f, 1.0f);
    ImGui::End();
}

void MuitLightLayer::OnEvent(Engine::Event& e)
{
    m_CameraController.OnEvent(e);
}
