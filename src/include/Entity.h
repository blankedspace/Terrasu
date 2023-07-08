#pragma once
#include "entt/entt.hpp"
#include "Components.h"
#include "Scene.h"

#include <iostream>
#include <typeinfo>
namespace Terrasu {
	class NativeScript;
	class Entity {
	public:
		Entity() = default;	
		Entity(entt::entity entity, Scene* scene) :
			m_entity(entity), m_scene(scene)
		{
		
		}
		inline void RestartScene() {
			m_scene->Restart();
		}
		template<typename T, typename... Args>
		inline T& AddComponent(Args&&... args)
		{
			return m_scene->m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);;
		}
		template<typename T>
		inline bool HasComponent()
		{
			return m_scene->m_registry.any_of<T>(m_entity);
		}
		template<typename T>
		inline T& GetComponent()
		{
			assert(HasComponent<T>());
			return m_scene->m_registry.get<T>(m_entity);
		}

		template<typename T>
		inline T& FindObjectOfType()
		{
			//TODO: Hashmap
			for (auto [ent, script] : m_scene->m_registry.view<NativeScriptComponent>().each()) {
			
				if(typeid(*script.Instance) == typeid(T))
					return *(T*)script.Instance;
			}
			return *(T*)nullptr;
		}
		inline Entity FindObject(std::string tag) {
			for (auto [ent, script] : m_scene->m_registry.view<TagComponent>().each()) {

				auto tg = m_scene->m_registry.get<TagComponent>(ent).tag;
				if (m_scene->m_registry.get<TagComponent>(ent).tag == tag)
				{
					Entity temp{ ent,m_scene };
					return temp;
				}
					
			}
			return *(Entity*)nullptr;
		}
		template<typename T>
		inline bool RemoveComponent()
		{
			return m_scene->m_registry.remove<T>(m_entity);
		}
		bool operator == (const Entity& c)
		{
			if (m_entity == c.m_entity && m_scene == c.m_scene)
				return true;
			return false;
		}
		inline uint32_t GetId() {
			return (uint32_t)m_entity;
		}
		inline AssetManager* GetAssetManager() {
			return m_scene->m_assetManager.get();
		}
		inline void Destroy() {
			m_scene->m_toDesroy.insert(m_entity);
		}
		inline bool IsAlive()
		{
			return m_scene->IsEntityAlive(m_entity);
		}
		inline glm::vec2 GetScreenSize() {
			return {m_scene->m_screenwidth,m_scene->m_screenheight};
		}
	private:
		Scene* m_scene = nullptr;
		entt::entity m_entity = entt::null;
		friend Scene;
		friend NativeScript;

	};
}