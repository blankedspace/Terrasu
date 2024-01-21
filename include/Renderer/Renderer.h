#pragma once

#include "ECS/Components.h"

#include "Vertex.h"
#include "Material.h"
#include "AssetManager.h"
#include "nanovg/nanovg.h"
#include <map>

namespace Terrasu {
	class SimpleQuad;

	class Renderer {
	public:
		Renderer();
		~Renderer();

		void DrawQuad(TransformComponent comp, const glm::mat4 transform,const Material& material);
		void DrawText(TransformComponent comp, const glm::mat4 transform, const TextUIComponent& text);
		void DrawVerts(std::vector<PosUvTileVertex>* vertexArray, const glm::mat4& transform, Material* material);
		void DrawSvg(TransformComponent comp, const glm::mat4 transform, const SvgComponent& svg, float w, float h, const CameraComponent& camera);
		std::shared_ptr<AssetManager> m_assetManager;
		bgfx::DynamicVertexBufferHandle m_vbh;
		NVGcontext* m_nvg;
		std::map<int, std::vector<std::pair<uint32_t,Drawable*>>> drawables;
	private:

		SimpleQuad* quad;

	};
}