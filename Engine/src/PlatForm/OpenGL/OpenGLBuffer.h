#pragma once
#include "Engine/Renderer/Buffer.h"

namespace Engine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void *vertices, uint32_t size);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(BufferLayout& layout) override { m_Layout = layout; };

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const void *indices, uint32_t count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer();
		~OpenGLFrameBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetTexture(uint32_t textureID) const override;
		virtual void SetTexture(uint32_t textureID, uint32_t attachment) const override;

		virtual void SetDepthCubeMapTexture(uint32_t textureID);
		virtual void SetRenderBuffer(uint32_t bufferID);

	private:
		uint32_t m_RendererID;
	};

	class OpenGLRenderBuffer : public RenderBuffer
	{
	public:
		OpenGLRenderBuffer(uint32_t width, uint32_t height);
		~OpenGLRenderBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		uint32_t GetRendererID() { return m_RendererID; }

	private:
		uint32_t m_RendererID;
	};
}

