#include "PBR_SpecularTexture.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

PBR_SpecularTexture::PBR_SpecularTexture() : Layer("PBR Texture Specular"), m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 1000.0f)
{
	glDepthFunc(GL_LEQUAL);
	Engine::RenderCommand::EnableCubeMapSeamless();

	m_SphereGeometry = Engine::Geometry::CreateSphere(glm::mat4(1.0f));

	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/PBR.vert", "assets/shaders/PBRSpecularTexture.frag");
	m_Shader->Bind();

	m_EnvGeometry = Engine::Geometry::CreateBox(glm::mat4(1.0f));
	m_ModelTexture = Engine::ModelTexture::Create();
	m_ModelTexture->AddMaterialTextureHDR("assets/textures/hdr/newport_loft.hdr", "equirectangularMap");
	m_EnvMesh = Engine::Mesh::Create(m_EnvGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);

	m_EquirectangularToCubeMapShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/CubeMap.vert", "assets/shaders/EquirectangularCubeMap.frag");
	
	m_FrameBuffer = Engine::FrameBuffer::Create();
	m_RenderBuffer = Engine::RenderBuffer::Create(2048, 2048, GL_DEPTH_COMPONENT24);
	m_TextureEnvCubMap = Engine::TextureCubeMap::CreateEnvMap(2048, 2048, true);
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

	m_TextureEnvCubMap->Bind();
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	m_ModelTexture = nullptr;
	m_ModelTexture = Engine::ModelTexture::Create();
	m_ModelTexture->AddMaterialTexture(m_TextureEnvCubMap);
	m_EnvMesh = nullptr;
	m_EnvMesh = Engine::Mesh::Create(m_EnvGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);

	m_IrradianceShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/CubeMap.vert", "assets/shaders/IrradianceConvolution.frag");

	//m_IrradianceRenderBuffer = Engine::RenderBuffer::Create(32, 32, GL_DEPTH_COMPONENT24);
	m_IrradianceTextureEnvCubMap = Engine::TextureCubeMap::CreateEnvMap(32, 32);

	m_RenderBuffer->Bind();
	m_FrameBuffer->Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	m_IrradianceShader->Bind();
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

	m_PrefilterShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/CubeMap.vert", "assets/shaders/Prefilter.frag");

	// TODO
	m_PrefilterTextureEnvCubMap = Engine::TextureCubeMap::CreateEnvMap(128, 128, true);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	m_PrefilterShader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_PrefilterShader)->SetMat4("u_Projection", captureProjection);
	glActiveTexture(0);
	m_TextureEnvCubMap->Bind();

	m_FrameBuffer->Bind();
	uint32_t maxMipLevels = 5;
	for (uint32_t mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		uint32_t mipWidth = static_cast<uint32_t>(128 * std::pow(0.5, mip));
		uint32_t mipHeight = static_cast<uint32_t>(128 * std::pow(0.5, mip));
		m_RenderBuffer->Bind();
		//glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		Engine::RenderCommand::SetViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_PrefilterShader)->SetFloat("roughness", roughness);
		for (uint32_t i = 0; i < 6; ++i)
		{
			std::dynamic_pointer_cast<Engine::OpenGLShader>(m_PrefilterShader)->SetMat4("u_View", captureViews[i]);
			m_FrameBuffer->SetTexture(
				std::dynamic_pointer_cast<Engine::OpenGLTextureCubeMap>(m_PrefilterTextureEnvCubMap)->GetRendererID(),
				GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				mip
			);

			Engine::RenderCommand::Clear();
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_EnvMesh->Submit(m_PrefilterShader, false);
		}
	}

	m_FrameBuffer->Unbind();

	m_BRDFShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Blur.vert", "assets/shaders/BRDF.frag");
	m_BRDFShader->Bind();

	m_BRDFColorBuffer = Engine::TextureColorBuffer::Create(512, 512, GL_RG);
	//m_BRDFRenderBuffer = Engine::RenderBuffer::Create(512, 512, GL_DEPTH_COMPONENT24);

	m_FrameBuffer->Bind();
	m_RenderBuffer->Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	//m_FrameBuffer->SetRenderBuffer(std::dynamic_pointer_cast<Engine::OpenGLRenderBuffer>(m_BRDFRenderBuffer)->GetRendererID());
	m_FrameBuffer->SetTexture(std::dynamic_pointer_cast<Engine::OpenGLTextureColorBuffer>(m_BRDFColorBuffer)->GetRendererID(), GL_COLOR_ATTACHMENT0);

	m_VertexArray = Engine::VertexArray::Create();

	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	auto vertexBuffer = Engine::VertexBuffer::Create(quadVertices, sizeof(quadVertices));
	Engine::BufferLayout layout = {
		{ Engine::ShaderDataType::Float3, "a_Pos"},
		{ Engine::ShaderDataType::Float2, "a_TexCoords"},
	};
	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[] = {
		0, 1, 2,
		2, 3, 1
	};

	auto indexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	Engine::RenderCommand::SetViewport(0, 0, 512, 512);
	Engine::RenderCommand::Clear();

	m_BRDFColorBuffer->Bind();
	Engine::Renderer::Submit(m_VertexArray, m_BRDFShader, glm::mat4(1.0));

	m_FrameBuffer->Unbind();

	Engine::RenderCommand::SetViewport(0, 0, 1600, 900);

	m_BackgroundShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Background.vert", "assets/shaders/Background.frag");

	m_ModelTexture = nullptr;
	m_ModelTexture = Engine::ModelTexture::Create();
	m_ModelTexture->AddMaterialTexture(m_TextureEnvCubMap, "environmentMap");
	m_EnvMesh = nullptr;
	m_EnvMesh = Engine::Mesh::Create(m_EnvGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_ModelTexture)->m_Texuters);

	//uniform samplerCube irradianceMap;
	//uniform samplerCube prefilterMap;
	//uniform sampler2D brdfLUT;

	for (uint32_t i = 0; i < nrColumns; ++i) {
		m_SphereTextures.push_back(Engine::ModelTexture::Create());
	}

	for (uint32_t i = 0; i < 5; ++i)
	{
		auto modelTexture = m_SphereTextures[i];
		modelTexture->AddMaterialTexture(m_IrradianceTextureEnvCubMap, "irradianceMap");
		modelTexture->AddMaterialTexture(m_PrefilterTextureEnvCubMap, "prefilterMap");
		modelTexture->AddMaterialTexture(m_BRDFColorBuffer, "brdfLUT");
		modelTexture->AddMaterialTexture("assets/textures/pbr/" + m_PBRTextureTypes[i] + "/albedo.png", "albedoMap");
		modelTexture->AddMaterialTexture("assets/textures/pbr/" + m_PBRTextureTypes[i] + "/normal.png", "normalMap");
		modelTexture->AddMaterialTexture("assets/textures/pbr/" + m_PBRTextureTypes[i] + "/metallic.png", "metallicMap");
		modelTexture->AddMaterialTexture("assets/textures/pbr/" + m_PBRTextureTypes[i] + "/roughness.png", "roughnessMap");
		modelTexture->AddMaterialTexture("assets/textures/pbr/" + m_PBRTextureTypes[i] + "/ao.png", "aoMap");


		m_SphereMeshs.push_back(Engine::Mesh::Create(
			m_SphereGeometry,
			std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(modelTexture)->m_Texuters
		));
	}


	//m_SphereMesh = Engine::Mesh::Create(m_SphereGeometry, std::dynamic_pointer_cast<Engine::OpenGLModelTexture>(m_SphereTextures)->m_Texuters);

}

void PBR_SpecularTexture::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Engine::RenderCommand::SetClearColor({ 0, 0, 0, 1 });
	Engine::RenderCommand::Clear();

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());


	for (int col = 0; col < nrColumns; ++col)
	{
		//std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

		glm::mat4 transform = glm::translate(
			glm::mat4(1.0f),
			glm::vec3(
				(float)(col - (nrColumns / 2)) * spacing,
				0.0f,
				2.0f
			)
		);

		m_SphereGeometry->SetTransform(transform);
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));
		m_SphereMeshs[col]->SubmitStrip(m_Shader);
	}

	for (uint32_t i = 0; i < sizeof(m_LightPositions) / sizeof(m_LightPositions[0]); ++i)
	{
		glm::vec3 newPos = m_LightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = m_LightPositions[i];
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("lightPositions[" + std::to_string(i) + "]", newPos);
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetFloat3("lightColors[" + std::to_string(i) + "]", m_LightColors[i]);

		glm::mat4 transform = glm::scale(
			glm::translate(
				glm::mat4(1.0f),
				newPos
			),
			glm::vec3(0.5f)
		);

		m_SphereGeometry->SetTransform(transform);
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->SetMat3("normalMatrix",
			glm::transpose(glm::inverse(glm::mat3(transform)))
		);

		//m_SphereMesh->SubmitStrip(m_Shader);
	}

	//m_PrefilterTextureEnvCubMap->Bind();
	m_EnvMesh->Submit(m_BackgroundShader);

	//m_BRDFColorBuffer->Bind();
	//Engine::Renderer::Submit(m_VertexArray, m_BRDFShader, glm::mat4(1.0));

	m_Shader->Bind();

	Engine::Renderer::EndScene();

}

void PBR_SpecularTexture::OnEvent(Engine::Event& e)
{
}

void PBR_SpecularTexture::OnImGuiRender()
{
}
