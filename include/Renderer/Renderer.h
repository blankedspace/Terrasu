#pragma once

#include "ECS/Components.h"

#include "Vertex.h"
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
		void DrawVerts(std::vector<PosUvTileVertex>* vertexArray, const glm::mat4& transform, Material* material);
		std::shared_ptr<AssetManager> m_assetManager;
		bgfx::DynamicVertexBufferHandle m_vbh;
	private:
		SimpleQuad* quad;
	
	};
}