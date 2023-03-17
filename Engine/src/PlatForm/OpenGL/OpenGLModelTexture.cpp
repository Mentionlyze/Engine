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
		auto texture = Texture2D::Create(path, gammaCorrection);

		modelTexture.Id = std::dynamic_pointer_cast<OpenGLTexture2D>(texture)->GetRendererId();
		modelTexture.Type = name;
		modelTexture.Path = path;
		modelTexture.Texture = texture;

		m_Texuters.push_back(modelTexture);
	}
}
