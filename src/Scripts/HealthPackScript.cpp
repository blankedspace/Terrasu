#include "HealthPackScript.h"
#include "PlayerScript.h"
namespace Terrasu {
	void HealthPackScript::OnCreate(){
		auto transform = GetComponent<TransformComponent>();
		GetComponent<SimplePhysicsComponent>()->state = SimplePhysicsComponent::PassThrough;
		StartPos = transform->Translation;
	}

	void HealthPackScript::OnUpdate(float dt){
		TimePassed += dt * 3.0f;
		auto transform = GetComponent<TransformComponent>();
		transform->Translation.y = StartPos.y + glm::sin(TimePassed) / 3.0f;
	}

	void HealthPackScript::OnColide(Entity other){
		if (other.HasComponent<NativeScriptComponent>())
		{
			auto script = other.GetComponent<NativeScriptComponent>();
			auto instance = script.GetInstance<PlayerScript>();
			if (instance != nullptr)
			{

				instance->GetHit(-25);
				Destroy();
			}
		}
	}
}
