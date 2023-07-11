#pragma once
#include "bgfx/bgfx.h"
#include "Components.h"
#include "SimpleQuad.h"
#include "Material.h"
#include "AssetManager.h"
namespace Terrasu {

	class Renderer {
	public:
		Renderer();
		~Renderer();

		void DrawQuad(const TransformComponent& transform,const Material& material);
		void DrawText(const TransformComponent& transform, const TextUIComponent& text);
		std::shared_ptr<AssetManager> m_assetManager;
	private:
		SimpleQuad quad;
	
	};
}