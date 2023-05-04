#pragma once

#include "Engine/Renderer/Texture.h"

namespace Engine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path, bool gammaCorrection);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;

		uint32_t const GetRendererID() const { return m_RendererID; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		std::string m_Path;
	};

	class OpenGLTextureHDR : public TextureHDR
	{
	public:
		OpenGLTextureHDR(const std::string& path);
		virtual ~OpenGLTextureHDR();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;

		uint32_t const GetRendererID() const { return m_RendererID; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		std::string m_Path;
	};

	class OpenGLTextureDepthMap : public TextureDepthMap
	{
	public:
		OpenGLTextureDepthMap(uint32_t width, uint32_t height);
		virtual ~OpenGLTextureDepthMap();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;

		uint32_t const GetRendererID() const { return m_RendererID; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
	};

	class OpenGLTextureCubeMap : public TextureCubeMap
	{
	public:
		OpenGLTextureCubeMap(const std::vector<std::string>& faces);
		OpenGLTextureCubeMap(uint32_t width, uint32_t height, bool mipmapFilter);
		virtual ~OpenGLTextureCubeMap();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot) const override;
		virtual void Unbind() const override;

		uint32_t GetRendererID() { return m_RendererId; }

	private:
		uint32_t m_RendererId;
		uint32_t m_Width, m_Height;
		std::vector<std::string> m_Faces;
	};

	class OpenGLTextureDepthCubMap : public TextureDepthCubeMap
	{
	public:
		OpenGLTextureDepthCubMap(uint32_t width, uint32_t height);
		virtual ~OpenGLTextureDepthCubMap();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot) const override;
		virtual void Unbind() const override;

		uint32_t GetRendererID() { return m_RendererID; }

	private:
		uint32_t  m_RendererID;
		uint32_t m_Width, m_Height;
	};

	class OpenGLTextureColorBuffer : public  TextureColorBuffer
	{
	public:
		OpenGLTextureColorBuffer(uint32_t width, uint32_t height, uint32_t format, uint32_t type);
		virtual ~OpenGLTextureColorBuffer();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot) const override;
		virtual void Unbind() const override;

		uint32_t GetRendererID() { return m_RendererID; }
	private:
		uint32_t  m_RendererID;
		uint32_t m_Width, m_Height;
	};
}
