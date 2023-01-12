#include "Precompile.h"
#include "Model.h"
#include "PlatForm/OpenGL/OpenGLModel.h"

namespace Engine
{
	Ref<Model> Model::Create(const std::string& path, bool gamma)
	{
		return CreateRef<OpenGLModel>(path, gamma);
	}

	void ModelLibrary::Add(const Ref<Model>& model)
	{
		auto name = model->GetName();
		ENGINE_CORE_ASSERT(!Exists(name), "Model has already Exits!");
		m_Models[name] = model;
	}

	Ref<Model> ModelLibrary::Load(const std::string& path)
	{
		auto model = Model::Create(path);
		Add(model);
		return model;
	}

	Ref<Model> ModelLibrary::Get(const std::string& name)
	{
		ENGINE_CORE_ASSERT(Exists(name), "Model is not exist!");
		return m_Models[name];
	}

	bool ModelLibrary::Exists(const std::string& name) const
	{
		return m_Models.find(name) != m_Models.end();
	}
}
