#include "Precompile.h"
#include "Model.h"
#include "PlatForm/OpenGL/OpenGLModel.h"

namespace Engine
{
	Ref<Model> Model::Create(const std::string& path, bool gamma)
	{
		return CreateRef<OpenGLModel>(path, gamma);
	}
}
