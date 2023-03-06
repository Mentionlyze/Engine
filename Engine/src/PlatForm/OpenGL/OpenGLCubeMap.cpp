#include "Precompile.h"
#include "OpenGLCubeMap.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Engine
{
	OpenGLCubeMap::OpenGLCubeMap(const std::vector<std::string>& faces) : m_Faces(faces)
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

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}

	OpenGLCubeMap::~OpenGLCubeMap()
	{
		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGLCubeMap::Bind(uint32_t slot) const
	{
		glBindTextureUnit(m_RendererId, slot);
	}

	void OpenGLCubeMap::Unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}
