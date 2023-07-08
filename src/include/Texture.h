#pragma once
#include <string>
#include "bgfx/bgfx.h"
namespace Terrasu {
	class Texture {
	public:
		std::string textureName = "Assets/Error.png";
		bgfx::UniformHandle texColor;
		bgfx::TextureHandle handle;
	};

}