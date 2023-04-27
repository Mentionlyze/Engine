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


		void AddMaterialTexture(const std::string& path, const std::string& name, bool gammaCorrection = false) override;
		void AddMaterialTexture(const Ref<TextureColorBuffer> texture, const std::string& type) override;
		void AddMaterialTexture(const Ref<TextureDepthMap> texture) override;
		void AddMaterialTexture(const Ref<TextureCubeMap> texture, const std::string& type) override;
		void AddMaterialTexture(const Ref<TextureDepthCubeMap> texture) override;
		void AddMaterialTextureHDR(const std::string& path, const std::string& name) override;
	};
}

