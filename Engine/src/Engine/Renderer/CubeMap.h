#pragma once

namespace Engine
{

	class CubeMap
	{
	public:
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;

	public:
		static Ref<CubeMap> Create(const std::vector<std::string>& faces);
	};
}


