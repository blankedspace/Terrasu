#pragma once
#include "imgui.h"
#include "glm/glm.hpp"
#include <string>
#include "ECS/Entity.h"
#include "AssetManager.h"
#include "ECS/SceneSerializer.h"
namespace Terrasu {
	class ComponentPanel {
	public:
		void DrawComponents(Entity& entity);
		void DrawEntityNode(Entity& entity, bool& isPopupOpen);
		void Draw(const entt::registry& m_registry);
		template<typename T>
		bool DisplayAddComponentEntry(const std::string& entryName);
	private:
		AssetManager* m_assetManager;
		SceneSerializer* m_sceneSerializer;
		Entity* m_SelectionContext;
		Entity* m_clipboard;
		Scene* m_scene;
		bool loadpref;
		std::string m_buffer = "aaa";
		friend Scene;
	};
	


}