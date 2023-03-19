#include "Precompile.h"
#include "OpenGLModel.h"
#include "OpenGLMesh.h"
#include "Engine/Renderer/Texture.h"
#include "OpenGLTexture.h"

namespace Engine
{
	OpenGLModel::OpenGLModel(const std::string& path, bool gamma) : m_GammaCorrection(gamma)
	{
		// read file via Assimp
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace);

		ENGINE_CORE_ASSERT(scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode, importer.GetErrorString());

		// retrieve the directory path of the filepath
		m_Directory = path.substr(0, path.find_last_of("/"));
		
		// process Assimp's root node recursively
		ProcessNode(scene->mRootNode, scene);

		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind(".");
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		m_Name = path.substr(lastSlash, count);
	}

	OpenGLModel::~OpenGLModel()
	{
	}

	void OpenGLModel::Submit(const Ref<Shader>& shader, const glm::mat4& transform) const
	{
		for (Ref<Mesh> mesh : m_Meshes)
		{
			mesh->Submit(shader, transform);
		}
	}

	void OpenGLModel::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// process each mesh located at the current node
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			// the node boject only contains indices to index the actual objects int the scene
			// the scene contains all the data, node is just to keep stuff organized£¨like relations between nodes£©
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Ref<Mesh> OpenGLModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		// data to fill
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<MaterialTexture> modelTextures;

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// positions
			auto vertice = mesh->mVertices[i];
			vertex.Position = glm::vec3(vertice.x, vertice.y, vertice.z);

			if (mesh->HasNormals())
			{
				auto normal = mesh->mNormals[i];
				vertex.Normal = glm::vec3(normal.x, normal.y, normal.z);
			}

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for(uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// process material
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		std::vector<MaterialTexture> diffuseMaps = LoadMateriaTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		modelTextures.insert(modelTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<MaterialTexture> specularMaps = LoadMateriaTextures(material, aiTextureType_SPECULAR, "texture_specular");
		modelTextures.insert(modelTextures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<MaterialTexture> normalMaps = LoadMateriaTextures(material, aiTextureType_NORMALS, "texture_normal");
		modelTextures.insert(modelTextures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<MaterialTexture> heightMaps = LoadMateriaTextures(material, aiTextureType_HEIGHT, "texture_height");
		modelTextures.insert(modelTextures.end(), heightMaps.begin(), heightMaps.end());

		return Mesh::Create(vertices, indices, modelTextures);
	}

	std::vector<MaterialTexture> OpenGLModel::LoadMateriaTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		std::vector<MaterialTexture> modelTextures;

		for (uint32_t i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString str;
			material->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip{};
			for (uint32_t j = 0; j < m_Textures.size(); j++)
			{
				if (std::strcmp(m_Textures[j].Path.data(), str.C_Str()) == 0)
				{
					modelTextures.push_back(m_Textures[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				MaterialTexture materialTexture;
				std::string filename = m_Directory + "/" + std::string(str.C_Str());
				Ref<Texture2D> texture = Texture2D::Create(filename);
				materialTexture.Id = std::dynamic_pointer_cast<OpenGLTexture2D>(texture)->GetRendererID();
				materialTexture.Type = typeName;
				materialTexture.Path = str.C_Str();
				materialTexture.Texture = texture;
				modelTextures.push_back(materialTexture);
				m_Textures.push_back(materialTexture);
			}
		}

		return modelTextures;
	}
}

