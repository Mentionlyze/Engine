#pragma once
#include <glad/glad.h>

namespace Engine
{
	class Texture
	{
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;

	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path, bool gammaCorrection = false);
	};

	class TextureHDR: public Texture
	{
	public:
		static Ref<TextureHDR> Create(const std::string& path);
	};


	class TextureDepthMap : public Texture
	{
	public:
		static Ref<TextureDepthMap> Create(uint32_t width, uint32_t height);
	};

	class TextureCubeMap : public Texture
	{
	public:
		static Ref<TextureCubeMap> Create(const std::vector<std::string>& faces);
		static Ref<TextureCubeMap> CreateEnvMap(uint32_t width, uint32_t height, bool mipmapFilter = false);
	};

	class TextureDepthCubeMap : public Texture
	{
	public:
		static Ref<TextureDepthCubeMap> Create(uint32_t width, uint32_t height);
	};

	class TextureColorBuffer : public Texture
	{
	public:
		static Ref<TextureColorBuffer> Create(uint32_t width, uint32_t height, uint32_t format = GL_RGBA, uint32_t type = GL_FLOAT);
	};
}
