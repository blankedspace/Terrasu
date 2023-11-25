#pragma once
#include <string>
#include "bgfx/bgfx.h"
namespace Terrasu {

	class Shader {
	public:
		Shader(): handle(BGFX_INVALID_HANDLE){}
		std::string shaderName;
		bgfx::ProgramHandle handle; 

	};
}