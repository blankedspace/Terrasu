#pragma once
#include "entt/entt.hpp"
#include "glm/glm.hpp"
#include "ECS/Scene.h"
#include <iostream>
#include <typeinfo>
namespace Terrasu {
	class NativeScript;
	class NativeScriptComponent;
	class Scene;
	class AssetManager;
	class Entity {
	public:
		Entity() = default;	
		Entity(entt::entity entity, Scene* scene) :
			m_entity(entity), m_scene(scene)
		{
		
		}
		void RestartScene();
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
		T* FindObjectOfType();

		Entity FindObject(std::string tag); 
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
		AssetManager* GetAssetManager(); 
		void Destroy();
		bool IsAlive();
	
		glm::vec2 GetScreenSize();
	private:
		Scene* m_scene = nullptr;
		entt::entity m_entity = entt::null;
		friend Scene;
		friend NativeScript;

	};




		class NativeScript {
		protected:
			virtual void OnCreate() {}
			virtual void OnDestroy() {}
			virtual void OnUpdate(float dt) {}
			virtual void OnColide(Entity other) {}
		public:
			template<typename T>
			inline T* GetComponent() {
				return &entity.GetComponent<T>();
			}
			template<typename T>
			inline T* AddComponent() {
				return &entity.AddComponent<T>();
			}
			template<typename T>
			inline T* FindObjectOfType() {
				return entity.FindObjectOfType<T>();
			}

			inline Entity FindObject(std::string tag) {
				Entity ent = entity.FindObject(tag);
				return ent;
			}
			template<typename T>
			T* BindScript(Entity& ent);
			template<typename T>
			inline Entity Instantiate(std::string tag) {
				auto ent = entity.m_scene->AddEntity(tag);
				ent.AddComponent<T>();
				return ent;
			}
			inline void Destroy() {
				entity.Destroy();
			}
			inline glm::vec2 GetScreenSize() {
				return entity.GetScreenSize();
			}
			Entity entity;
			friend Scene;
		};




		class NativeScriptComponent {
		public:
			std::string ScriptName;
			NativeScript* (*InstantiateScript)();

			inline const std::type_info& GetTypeInfo() {
				if (Instance == nullptr) {
					if (InstantiateScript == nullptr)
					{
						return typeid(NativeScript);
					}
					else
					{
						NativeScript* temp = InstantiateScript();
						const std::type_info& a = typeid(*temp);
						delete temp;
						return a;
					}
				}
				return typeid(*Instance);
			}
			~NativeScriptComponent() {

				Instance = nullptr;
			}
			template<typename T>
			inline void Bind() {

				InstantiateScript = []() {return static_cast<NativeScript*>(new T()); };

			}
			inline void DestroyScript(NativeScriptComponent& nsc) {
				delete nsc.Instance;
				nsc.Instance = nullptr;
			}
			template<typename T>
			inline T* GetInstance()
			{
				if (Instance != nullptr && typeid(T) == typeid(*Instance))
				{
					return (T*)Instance;
				}
				return nullptr;
			}

		private:
			NativeScript* Instance = nullptr;


			friend Scene;
			friend Entity;
			friend NativeScript;
		};

		template<typename T>
		inline T* NativeScript::BindScript(Entity& ent)
		{
			{
				auto& script = ent.AddComponent<NativeScriptComponent>();
				script.Bind<T>();
				script.Instance = script.InstantiateScript();
				script.Instance->entity = ent;
				script.Instance->OnCreate();
				return (T*)script.Instance;
			}
		}
		template<typename T>
		inline T* Entity::FindObjectOfType()
		{

			//TODO: Hashmap
			for (auto [ent, script] : m_scene->m_registry.view<NativeScriptComponent>().each()) {


				if (typeid(*script.Instance) == typeid(T))
					return (T*)script.Instance;
			}
			return nullptr;
		
		}
}
