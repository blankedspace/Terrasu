#pragma once

#include "Components.h"
#include "SimpleQuad.h"
#include "Material.h"
#include "AssetManager.h"

namespace Terrasu {
	class SimpleQuad;
	class Renderer {
	public:
		Renderer();
		~Renderer();

		void DrawQuad(const TransformComponent& transform,const Material& material);
		void DrawText(const TransformComponent& transform, const TextUIComponent& text);
		std::shared_ptr<AssetManager> m_assetManager;
	private:
		SimpleQuad* quad;
	
	};
}