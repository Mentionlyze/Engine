#include "Precompile.h"
#include "RenderCommand.h"
#include "PlatForm/OpenGL/OpenGLRendererAPI.h"

namespace Engine
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}