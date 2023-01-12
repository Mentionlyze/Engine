#pragma once
#include "Shader.h"
#include <glm/glm.hpp>

namespace Engine
{
	class Model
	{
	public:
		virtual ~Model() = default;

		virtual const std::string GetName() const = 0;
		
		virtual void Submit(const Ref<Shader>& shader, const glm::mat4& transform) const = 0;

		static Ref<Model> Create(const std::string& path, bool gamma = false);
	};

	class ModelLibrary
	{
	public:
		void Add(const Ref<Model>& model);
		Ref<Model> Load(const std::string& path);
		Ref<Model> Get(const std::string& name);

		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Ref<Model>> m_Models;
	};
}


