#pragma once
#include "entt/entt.hpp"

#include "Renderer.h"
#include "AssetManager.h"
#include <thread>
namespace Terrasu {
	class Application;
	class Entity;
	class SceneSerializer;
	class ComponentPanel;

	struct pair_hash {
		inline std::size_t operator()(const std::pair<entt::entity, entt::entity>& v) const {
			return (uint32_t)v.first * 31 + (uint32_t)v.second;
		}
	};

	class Scene {
	public:
		Entity AddEntity(std::string tag);
		Entity AddEntity(Entity& ent);
		Scene();
		~Scene();
		void OnStart();
		void OnUpdate();
		void Restart();
		bool IsEntityAlive(entt::entity entity);
	private:
		std::shared_ptr<Renderer> m_renderer;
		std::shared_ptr<AssetManager> m_assetManager;
		std::unique_ptr<ComponentPanel> m_componentPanel;
		std::unordered_set<entt::entity> m_toDesroy;

		std::unordered_set<std::pair<entt::entity,entt::entity>, pair_hash> m_Colisions;
		std::atomic<bool> unlockFromColision = { true }, unlockFromMain = { true };
		Application* m_app = nullptr;
		entt::registry m_registry;
		int m_screenwidth = 0, m_screenheight = 0;
		float dt = 0;
		float timeAlive = 0;
		bool Runtime = false;
		
		bool Restarted = false;
		
		int collisionframe = 0;
		std::function<void()> m_colisionFunction;
		std::thread m_collision;

		friend Application;
		friend Entity;
		friend SceneSerializer;
	};
}