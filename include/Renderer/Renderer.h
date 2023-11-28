#pragma once

#include "ECS/Components.h"

#include "Vertex.h"
#include "Material.h"
#include "AssetManager.h"
#include "nanovg/nanovg.h"

namespace Terrasu {
	class SimpleQuad;
	class Renderer {
	public:
		Renderer();
		~Renderer();

		void DrawQuad(const TransformComponent& transform,const Material& material);
		void DrawText(const TransformComponent& transform, const TextUIComponent& text);
		void DrawVerts(std::vector<PosUvTileVertex>* vertexArray, const glm::mat4& transform, Material* material);
		void DrawSvg(const TransformComponent& transform, const SpriteSVGComponent& svg, float w, float h, const CameraComponent& camera);
		std::shared_ptr<AssetManager> m_assetManager;
		bgfx::DynamicVertexBufferHandle m_vbh;
		NVGcontext* m_nvg;
	private:
		SimpleQuad* quad;

	};
}