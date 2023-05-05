#pragma once
#include "Engine.h"

class PBR_SpecularTexture : public Engine::Layer
{
public:
	PBR_SpecularTexture();

	void OnUpdate(Engine::Timestep ts) override;
	void OnEvent(Engine::Event& e) override;
	void OnImGuiRender() override;

private:
	Engine::PerspectiveCameraController m_CameraController;

	Engine::Ref<Engine::Geometry> m_SphereGeometry;
	std::vector<Engine::Ref<Engine::Mesh>> m_SphereMeshs;
	std::vector<Engine::Ref<Engine::ModelTexture>> m_SphereTextures;
	Engine::Ref<Engine::Shader> m_Shader;

	Engine::Ref<Engine::Texture2D> m_AlbedoTexture;
	Engine::Ref<Engine::Texture2D> m_NormalTexture;
	Engine::Ref<Engine::Texture2D> m_MetallicTexture;
	Engine::Ref<Engine::Texture2D> m_RoughnessTexture;
	Engine::Ref<Engine::Texture2D> m_AoTexture;

	Engine::Ref<Engine::Geometry> m_EnvGeometry;
	Engine::Ref<Engine::Mesh> m_EnvMesh;
	Engine::Ref<Engine::ModelTexture> m_ModelTexture;
	Engine::Ref<Engine::Shader> m_EquirectangularToCubeMapShader;

	Engine::Ref<Engine::FrameBuffer> m_FrameBuffer;
	Engine::Ref<Engine::RenderBuffer> m_RenderBuffer;
	Engine::Ref<Engine::TextureCubeMap> m_TextureEnvCubMap;

	Engine::Ref<Engine::TextureCubeMap> m_IrradianceTextureEnvCubMap;

	Engine::Ref<Engine::Shader> m_IrradianceShader;

	Engine::Ref<Engine::TextureCubeMap> m_PrefilterTextureEnvCubMap;

	Engine::Ref<Engine::Shader> m_PrefilterShader;

	Engine::Ref<Engine::TextureColorBuffer> m_BRDFColorBuffer;

	Engine::Ref<Engine::Shader> m_BRDFShader;

	Engine::Ref<Engine::VertexArray> m_VertexArray;

	Engine::Ref<Engine::Shader> m_BackgroundShader;

	glm::vec3 m_LightPositions[4] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};

	//glm::vec3 m_LightPositions[1] = {
	//	glm::vec3(0.0f,  0.0f, 10.0f),
	//};


	glm::vec3 m_LightColors[4] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};


	//glm::vec3 m_LightColors[1] = {
	//	glm::vec3(150.0f, 150.0f, 150.0f)
	//};

	int nrRows = 1;
	int nrColumns = 5;
	float spacing = 2.5;

	std::string m_PBRTextureTypes[5] = {
		"rusted_iron",
		"gold",
		"grass",
		"plastic",
		"wall"
	};
};
