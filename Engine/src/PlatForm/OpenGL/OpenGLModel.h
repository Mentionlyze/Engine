#pragma once
#include "Engine/Renderer/Model.h"
#include "Engine/Renderer/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{
	class OpenGLModel : public Model
	{
	public:
		std::vector<Ref<Mesh>> m_Meshes;
		std::vector<ModelTexture> m_Textures;
		std::string m_Directory;
		bool m_GammaCorrection;
	public:
		OpenGLModel(const std::string& path, bool gamma);
		~OpenGLModel();

		void Submit(const Ref<Shader>& shader, const glm::mat4& transform) const override;

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);

		Ref<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<ModelTexture> LoadMateriaTextures(aiMaterial* material, aiTextureType type, std::string typeName);
	};
}

