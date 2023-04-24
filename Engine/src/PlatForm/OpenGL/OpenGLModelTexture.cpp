#include "Precompile.h"
#include "OpenGLModelTexture.h"
#include "OpenGLTexture.h"

namespace Engine
{
	OpenGLModelTexture::OpenGLModelTexture()
	{
	}

	OpenGLModelTexture::OpenGLModelTexture(const std::initializer_list<MaterialTexture>& textures) : m_Texuters(textures)
	{
	}

	OpenGLModelTexture::~OpenGLModelTexture()
	{
	}

	void OpenGLModelTexture::AddMaterialTexture(const std::string& path, const std::string& name, bool gammaCorrection) 
	{
		MaterialTexture modelTexture;

		if (path.size() == 0) {
			modelTexture.Id = 0;
			modelTexture.Type = name;
			modelTexture.Path = path;
			modelTexture.Texture = nullptr;
		}
		else {
			auto texture = Texture2D::Create(path, gammaCorrection);

			modelTexture.Id = std::dynamic_pointer_cast<OpenGLTexture2D>(texture)->GetRendererID();
			modelTexture.Type = name;
			modelTexture.Path = path;
			modelTexture.Texture = texture;
		}

		m_Texuters.push_back(modelTexture);
	}

	void OpenGLModelTexture::AddMaterialTexture(const Ref<TextureDepthMap> texture)
	{
		MaterialTexture modelTexture;
		
		modelTexture.Id = std::dynamic_pointer_cast<OpenGLTextureDepthMap>(texture)->GetRendererID();
		modelTexture.Type = "depthMap";
		modelTexture.Path = "";
		modelTexture.Texture = texture;

		m_Texuters.push_back(modelTexture);
	}

	void OpenGLModelTexture::AddMaterialTexture(const Ref<TextureDepthCubeMap> texture)
	{
		MaterialTexture modelTexture;
		
		modelTexture.Id = std::dynamic_pointer_cast<OpenGLTextureDepthCubMap>(texture)->GetRendererID();
		modelTexture.Type = "depthMap";
		modelTexture.Path = "";
		modelTexture.Texture = texture;

		m_Texuters.push_back(modelTexture);
	}

	void OpenGLModelTexture::AddMaterialTextureHDR(const std::string& path, const std::string& name)
	{
		MaterialTexture modelTexture;

		auto texture = Engine::TextureHDR::Create(path);

		modelTexture.Id = std::dynamic_pointer_cast<OpenGLTextureHDR>(texture)->GetRendererID();
		modelTexture.Type = name;
		modelTexture.Path = path;
		modelTexture.Texture = texture;

		m_Texuters.push_back(modelTexture);
	}

}
