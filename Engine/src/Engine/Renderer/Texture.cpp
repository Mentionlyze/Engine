#include "Precompile.h"
#include "Texture.h"
#include "PlatForm/OpenGL/OpenGLTexture.h"

namespace Engine
{
	Ref<Texture2D> Texture2D::Create(const std::string& path, bool gammaCorrection)
	{
		return CreateRef<OpenGLTexture2D>(path, gammaCorrection);
	}

	Ref<TextureDepthMap> TextureDepthMap::Create(uint32_t width, uint32_t height)
	{
		return CreateRef<OpenGLTextureDepthMap>(width, height);
	}

	Ref<TextureCubeMap> TextureCubeMap::Create(const std::vector<std::string>& faces)
	{
		return CreateRef<OpenGLTextureCubeMap>(faces);
	}

	Ref<TextureDepthCubeMap> TextureDepthCubeMap::Create(uint32_t width, uint32_t height)
	{
		return CreateRef<OpenGLTextureDepthCubMap>(width, height);
	}

	Ref<TextureColorBuffer> TextureColorBuffer::Create(uint32_t width, uint32_t height)
	{
		return CreateRef<OpenGLTextureColorBuffer>(width, height);
	}
	Ref<TextureHDR> TextureHDR::Create(const std::string& path)
	{
		return CreateRef<OpenGLTextureHDR>(path);
	}
}