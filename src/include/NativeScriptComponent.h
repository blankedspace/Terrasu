#pragma once
#include "Entity.h"
#include "Input.h"
namespace Terrasu {
	class NativeScriptComponent;
	class NativeScript {
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(float dt) {}
		virtual void OnColide(Entity other) {}
	public:
		template<typename T>
		inline T& GetComponent(){
			return entity.GetComponent<T>();
		}
		template<typename T>
		inline T& AddComponent(){
			return entity.AddComponent<T>();
		}
		template<typename T>
		inline T& FindObjectOfType(){
			return entity.FindObjectOfType<T>();
		}

		inline Entity FindObject(std::string tag) {
			Entity ent = entity.FindObject(tag);
			return ent;
		}
		template<typename T>
		inline T& BindScript(Entity& ent){
			auto& script = ent.AddComponent<NativeScriptComponent>();
			script.Bind<T>();
			script.Instance = script.InstantiateScript();
			script.Instance->entity = ent;
			script.Instance->OnCreate();
			return *(T*)script.Instance;
		}
		template<typename T>
		inline Entity Instantiate(std::string tag) {
			auto ent = entity.m_scene->AddEntity(tag);
			ent.AddComponent<T>();
			return ent;
		}
		inline void Destroy(){
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
		NativeScript* (*InstantiateScript)();

		inline const std::type_info& GetTypeInfo() {
			if(Instance == nullptr)
				if(InstantiateScript == nullptr)
					return typeid(NativeScript);
				else
					return typeid(*InstantiateScript());
			return typeid(*Instance);
		}
		~NativeScriptComponent() {
	
			Instance = nullptr;
		}
		template<typename T>
		inline void Bind() {

			InstantiateScript = []() {return static_cast<NativeScript*>(new T()); };
	
		}
		inline void DestroyScript(NativeScriptComponent& nsc){
			delete nsc.Instance; 
			nsc.Instance = nullptr;
		}
		template<typename T>
		inline T& GetInstance()
		{
			if (Instance != nullptr && typeid(T) == typeid(*Instance))
			{
				return *(T*)Instance;
			}
			return *(T*)nullptr;
		}

	private:
		NativeScript* Instance  = nullptr;


		friend Scene;
		friend Entity;
		friend NativeScript;
	};
}