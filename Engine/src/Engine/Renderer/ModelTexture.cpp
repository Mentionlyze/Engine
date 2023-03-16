#include "Precompile.h"
#include "ModelTexture.h"
#include "PlatForm/OpenGL/OpenGLModelTexture.h"

namespace Engine
{
	Ref<ModelTexture> ModelTexture::Create()
	{
		return CreateRef<OpenGLModelTexture>();
	}
	Ref<ModelTexture> ModelTexture::Create(const std::initializer_list<MaterialTexture>& textures)
	{
		return CreateRef<OpenGLModelTexture>(textures);
	}
}
