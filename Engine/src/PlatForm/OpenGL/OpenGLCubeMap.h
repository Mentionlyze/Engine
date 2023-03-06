#pragma once
#include "Engine/Renderer/CubeMap.h"

namespace Engine
{

	class OpenGLCubeMap : public CubeMap
	{
	public:
		OpenGLCubeMap(const std::vector<std::string>& faces);
		~OpenGLCubeMap();

		void Bind(uint32_t slot) const;
		void Unbind() const;

		uint32_t GetPath() { return m_RendererId; }

	private:
		uint32_t m_RendererId;
		std::vector<std::string> m_Faces;
	};

}

