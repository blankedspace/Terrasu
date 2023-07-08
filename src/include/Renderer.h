#pragma once
#include "bgfx/bgfx.h"
#include "Components.h"
#include "SimpleQuad.h"
#include "Material.h"
namespace Terrasu {

	class Renderer {
	public:
		Renderer();
		~Renderer();

		void DrawQuad(const TransformComponent& transform,const Material& material);
		void DrawText(const TransformComponent& transform, const TextUIComponent& text);
	private:
		SimpleQuad quad;
	};
}