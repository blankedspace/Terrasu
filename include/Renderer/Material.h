#pragma once
#include <vector>
#include <string>
#include <memory>
#include "glm/glm.hpp"

#include "Shader.h"
#include "Texture.h"

namespace Terrasu {

	struct Uniform
	{

		const bgfx::UniformType::Enum m_type = bgfx::UniformType::Vec4;
		std::string m_name;
		glm::vec4 data;

		Uniform& operator=(const Uniform& right) {
			if (this == &right) {
				return *this;
			}
			m_name = right.m_name;
			data = right.data;
			return *this;
		}
	};
	class Material {
	public:
		Shader shader;
		std::vector<Uniform> uniforms;
		std::vector<Texture> textures;
	};
}