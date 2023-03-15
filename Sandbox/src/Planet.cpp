#include "Planet.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

Planet::Planet() : m_CameraController(75.0f, 1.6f / 0.9f, 0.1f, 1000.f)
{
	m_Model = Engine::Renderer::GetModelLibrary()->Load("assets/objects/planet/planet.obj");
	m_Shader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/ModelLoad.vert", "assets/shaders/ModelLoad.frag");

	m_AsteroidShader = Engine::Renderer::GetShaderLibrary()->Load("assets/shaders/Asteroid.vert", "assets/shaders/Asteroid.frag");
	m_AsteroidShader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_AsteroidShader)->SetInt("texture_diffuse1", 0);

	srand(static_cast<uint32_t>(glfwGetTime()));
	float radius = 150.0f;
	float offset = 25.0f;

	for (uint32_t i = 0; i < amount; i++) 
	{
		glm::mat4 model = glm::mat4(1.0f);

		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
		model = glm::scale(model, glm::vec3(scale));

		float rotAngle = static_cast<float>((rand() % 360));
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		m_ModelMatrixs[i] = model;
	}

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &m_ModelMatrixs[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < std::dynamic_pointer_cast<Engine::OpenGLModel>(m_Model)->m_Meshes.size(); i++)
	{
		auto mesh = std::dynamic_pointer_cast<Engine::OpenGLModel>(m_Model)->m_Meshes[i];
		auto vertexArray = std::dynamic_pointer_cast<Engine::OpenGLMesh>(mesh)->GetVertexArray();
		
		vertexArray->Bind();
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		vertexArray->Unbind();
	}
}

void Planet::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

    Engine::RenderCommand::SetClearColor({ 0.0, 0.0, 0.0, 1.0 });
	Engine::RenderCommand::Clear();

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	m_Shader->Bind();
	m_Model->Submit(m_Shader, glm::scale(glm::mat4(1.0f), glm::vec3(4.0f)));

	m_AsteroidShader->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(m_AsteroidShader)->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
	glBindTexture(GL_TEXTURE_2D, std::dynamic_pointer_cast<Engine::OpenGLModel>(m_Model)->m_Textures[0].Id);

	for (unsigned int i = 0; i < std::dynamic_pointer_cast<Engine::OpenGLModel>(m_Model)->m_Meshes.size(); i++)
	{
		auto mesh = std::dynamic_pointer_cast<Engine::OpenGLModel>(m_Model)->m_Meshes[i];
		auto vertexArray = std::dynamic_pointer_cast<Engine::OpenGLMesh>(mesh)->GetVertexArray();
		vertexArray->Bind();
		uint32_t size = std::dynamic_pointer_cast<Engine::OpenGLMesh>(mesh)->m_Indices.size();
		glDrawElementsInstanced(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0, amount);
		vertexArray->Unbind();
	}

	Engine::Renderer::EndScene();
}

void Planet::OnEvent(Engine::Event& e)
{
}

void Planet::OnImGuiRender()
{
}

