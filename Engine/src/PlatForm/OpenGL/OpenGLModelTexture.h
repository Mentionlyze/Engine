#pragma once
#include "Engine/Renderer/ModelTexture.h"

namespace Engine
{
	class OpenGLModelTexture : public ModelTexture
	{
	public:
		std::vector<MaterialTexture> m_Texuters;

	public:
		OpenGLModelTexture();
		OpenGLModelTexture(const std::initializer_list<MaterialTexture>& textures);
		~OpenGLModelTexture();


		void AddMaterialTexture(const std::string& path, const std::string& name) override;
	};
}

