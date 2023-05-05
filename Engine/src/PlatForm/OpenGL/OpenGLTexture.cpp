#include "Precompile.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Engine
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, bool gammaCorrection) : m_Path(path)
	{
		//int width, height, channels;

		//stbi_set_flip_vertically_on_load(1);

		//stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		//ENGINE_CORE_ASSERT(data, "Failed to load image");

		//m_Width = width;
		//m_Height = height;

		//GLenum internalFormat = 0, dataFormat = 0;
		//if (channels == 4)
		//{
		//	internalFormat = gammaCorrection ? GL_SRC1_ALPHA : GL_RGBA8;
		//	dataFormat = GL_RGBA;
		//}
		//else if (channels == 3)
		//{
		//	internalFormat = gammaCorrection ? GL_SRGB8 : GL_RGBA8;
		//	dataFormat = GL_RGB;
		//}
		//else if (channels == 1)
		//{
		//	internalFormat = GL_RED;
		//	dataFormat = GL_RED;
		//}
	
		//ENGINE_CORE_INFO(internalFormat & dataFormat);
		//ENGINE_CORE_ASSERT(internalFormat && dataFormat, "Format not suported!");

		//glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		//glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		//glGenerateMipmap(GL_TEXTURE_2D);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		//stbi_image_free(data);

		
		stbi_set_flip_vertically_on_load(1);
		glGenTextures(1, &m_RendererID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
		ENGINE_CORE_ASSERT(data, "Failed to load image");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, format = 0;
		if (nrComponents == 1)
		{
			internalFormat = GL_RED;
			format = GL_RED;
		}
		else if (nrComponents == 3)
		{

			internalFormat = gammaCorrection ? GL_SRGB8 : GL_RGBA8;
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRC1_ALPHA : GL_RGBA8;
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		//glBindTextureUnit(slot, m_RendererID);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGLTexture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTextureHDR::OpenGLTextureHDR(const std::string& path) : m_Path(path)
	{
		stbi_set_flip_vertically_on_load(1);
		int width, height, channels;
		float* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
		ENGINE_CORE_ASSERT(data, "Failed to load image");

		m_Width = width;
		m_Height = height;

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}

	OpenGLTextureHDR::~OpenGLTextureHDR()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureHDR::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTextureHDR::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	OpenGLTextureDepthMap::OpenGLTextureDepthMap(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTextureStorage2D(m_RendererID, 0, GL_DEPTH_COMPONENT, width, height);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	OpenGLTextureDepthMap::~OpenGLTextureDepthMap()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureDepthMap::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTextureDepthMap::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTextureCubeMap::OpenGLTextureCubeMap(const std::vector<std::string>& faces) : m_Faces(faces)
	{
		int width, height, channels;
		GLenum internalFormat = 0, dataFormat = 0;

		glGenTextures(1, &m_RendererId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId);

		for (uint32_t i = 0; i < faces.size(); i++)
		{
			stbi_uc* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
			ENGINE_CORE_ASSERT(data, "Failed to load image");

			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGB;
			}
			else if (channels == 1)
			{
				internalFormat = GL_RED;
				dataFormat = GL_RED;
			}

			ENGINE_CORE_INFO(internalFormat & dataFormat);
			ENGINE_CORE_ASSERT(internalFormat && dataFormat, "Format not suported!");

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}

		m_Width = width;
		m_Height = height;

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}

	OpenGLTextureCubeMap::OpenGLTextureCubeMap(uint32_t width, uint32_t height, bool mipmapFilter) : m_Width(width), m_Height(height)
	{
		glGenTextures(1, &m_RendererId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId);
		for (uint32_t i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		if (mipmapFilter) 
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	OpenGLTextureCubeMap::~OpenGLTextureCubeMap()
	{
		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGLTextureCubeMap::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId);
	}

	void OpenGLTextureCubeMap::Unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}


	OpenGLTextureDepthCubMap::OpenGLTextureDepthCubMap(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	OpenGLTextureDepthCubMap::~OpenGLTextureDepthCubMap()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureDepthCubMap::Bind(uint32_t slot) const
	{
		//glBindTextureUnit(m_RendererID, slot);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}

	void OpenGLTextureDepthCubMap::Unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	OpenGLTextureColorBuffer::OpenGLTextureColorBuffer(uint32_t width, uint32_t height, uint32_t format, uint32_t type) : m_Width(width), m_Height(height)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, format, type, 0);
		//glTextureStorage2D(m_RendererID, 1, GL_RGB16F, m_Width, m_Height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	OpenGLTextureColorBuffer::~OpenGLTextureColorBuffer()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureColorBuffer::Bind(uint32_t slot) const
	{
		//glBindTextureUnit(slot, m_RendererID);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGLTextureColorBuffer::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}


}