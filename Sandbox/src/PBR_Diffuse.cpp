#include "PBR_Diffuse.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

PBR_Diffuse::PBR_Diffuse() : Layer("PBR Diffuse"), m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 1000.0f)
{
    glDepthFunc(GL_LEQUAL);

	m_SphereGeometry = Engine::Geometry::CreateSphere(glm::mat4(1.0f));
	m_SphereMesh = Engine::Mesh::Create(m_SphereGeometry, {});
	
	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/PBR.vert", "assets/shaders/PBR.frag");
	m_Shader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("ao", 1.0f);

	m_EnvGeometry = Engine::Geometry::CreateBox(glm::mat4(1.0f));
	m_ModelTexture = Engine::ModelTexture::Create();
	m_ModelTexture->AddMaterialTextureHDR("assets/textures/hdr/newport_loft.hdr", "equirectangularMap");
	m_EnvMesh = Engine::Mesh::Create(m_EnvGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);

	m_EquirectangularToCubeMapShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/CubeMap.vert", "assets/shaders/EquirectangularCubeMap.frag");
	
	m_FrameBuffer = Engine::FrameBuffer::Create();
	m_RenderBuffer = Engine::RenderBuffer::Create(2048, 2048, GL_DEPTH_COMPONENT24);
	m_TextureEnvCubMap = Engine::TextureCubeMap::CreateEnvMap(2048, 2048);
	m_FrameBuffer->SetRenderBuffer(std::dynamic_pointer_cast<Engine::OpenGLRenderBuffer>(m_RenderBuffer)->GetRendererID());
	m_FrameBuffer->Unbind();

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	m_EquirectangularToCubeMapShader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_EquirectangularToCubeMapShader)->SetMat4("u_Projection", captureProjection);

	Engine::RenderCommand::SetViewport(0, 0, 2048, 2048);
	m_FrameBuffer->Bind();

	for (uint32_t i = 0; i < 6; ++i)
	{
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_EquirectangularToCubeMapShader)->SetMat4("u_View", captureViews[i]);
		m_FrameBuffer->SetTexture(
			std::dynamic_pointer_cast<Engine::OpenGLTextureCubeMap>(m_TextureEnvCubMap)->GetRendererID(), 
			GL_COLOR_ATTACHMENT0, 
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
		);
		Engine::RenderCommand::Clear();
		m_EnvMesh->Submit(m_EquirectangularToCubeMapShader, false);
	}

	m_FrameBuffer->Unbind();

	m_ModelTexture = nullptr;
	m_ModelTexture = Engine::ModelTexture::Create();
	m_ModelTexture->AddMaterialTexture(m_TextureEnvCubMap);
	m_EnvMesh = nullptr;
	m_EnvMesh = Engine::Mesh::Create(m_EnvGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);

	m_IrradianceShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/CubeMap.vert", "assets/shaders/IrradianceConvolution.frag");
	m_IrradianceShader->Bind();

	m_FrameBuffer->Bind();

	m_IrradianceRenderBuffer = Engine::RenderBuffer::Create(32, 32, GL_DEPTH_COMPONENT24);
	m_IrradianceTextureEnvCubMap = Engine::TextureCubeMap::CreateEnvMap(32, 32);
	m_FrameBuffer->SetRenderBuffer(std::dynamic_pointer_cast<Engine::OpenGLRenderBuffer>(m_IrradianceRenderBuffer)->GetRendererID());

	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_IrradianceShader)->SetMat4("u_Projection", captureProjection);

	Engine::RenderCommand::SetViewport(0, 0, 32, 32);


	for (uint32_t i = 0; i < 6; ++i)
	{
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_IrradianceShader)->SetMat4("u_View", captureViews[i]);
		m_FrameBuffer->SetTexture(
			std::dynamic_pointer_cast<Engine::OpenGLTextureCubeMap>(m_IrradianceTextureEnvCubMap)->GetRendererID(), 
			GL_COLOR_ATTACHMENT0, 
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
		);
		Engine::RenderCommand::Clear();
		m_EnvMesh->Submit(m_IrradianceShader, false);
	}

	m_FrameBuffer->Unbind();

	Engine::RenderCommand::SetViewport(0, 0, 1600, 900);

	m_BackgroundShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Background.vert", "assets/shaders/Background.frag");

	m_ModelTexture = nullptr;
	m_ModelTexture = Engine::ModelTexture::Create();
	m_ModelTexture->AddMaterialTexture(m_TextureEnvCubMap, "environmentMap");
	m_EnvMesh = nullptr;
	m_EnvMesh = Engine::Mesh::Create(m_EnvGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);

}

void PBR_Diffuse::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::SetClearColor({ 0, 0, 0, 1 });
	Engine::RenderCommand::Clear();

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	m_IrradianceTextureEnvCubMap->Bind();

	for (int row = 0; row < nrRows; ++row)
	{
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("metallic", (float)row / (float)nrRows);
		for (int col = 0; col < nrColumns; ++col)
		{
			
			std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("roughness",
					glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f)
				);

			glm::mat4 transfom = glm::translate(glm::mat4(1.0f), glm::vec3(
				(col - (nrColumns/ 2)) * spacing,
				(row - (nrRows/ 2)) * spacing,
				0.0
			));

			m_SphereGeometry->SetTransform(transfom);

			std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetMat3("normalMatrix", 
					glm::transpose(glm::inverse(glm::mat3(transfom)))
				);

			m_SphereMesh->SubmitStrip(m_Shader);
		}
	}

	for (int i = 0; i < sizeof(m_LightPositions) / sizeof(m_LightPositions[0]); ++i)
	{
		glm::vec3 newPos = m_LightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = m_LightPositions[i];
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("lightPositions[" + std::to_string(i) + "]", newPos);
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("lightColors[" + std::to_string(i) + "]", m_LightColors[0]);

		glm::mat4 transfom = glm::scale(glm::translate(glm::mat4(1.0f), newPos), glm::vec3(0.5f));

		m_SphereGeometry->SetTransform(transfom);

		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetMat3("normalMatrix",
			glm::transpose(glm::inverse(glm::mat3(transfom)))
		);

		m_SphereMesh->SubmitStrip(m_Shader);
	}

	m_TextureEnvCubMap->Bind();
	m_EnvMesh->Submit(m_BackgroundShader);
	m_Shader->Bind();

	Engine::Renderer::EndScene();

}

void PBR_Diffuse::OnEvent(Engine::Event& e)
{
}

void PBR_Diffuse::OnImGuiRender()
{
}
