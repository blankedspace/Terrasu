#pragma once
#include "NativeScriptComponent.h"
namespace Terrasu {

	class PlayerScript;
	class CameraScript : public NativeScript {
	public:
		glm::vec3 ScreenToWorld(glm::vec3 vec);
	protected:
		void OnCreate() override;
		virtual void OnDestroy() {}
		void OnUpdate(float dt) override;
		virtual void OnColide(Entity other) {}
	private:
		PlayerScript* player = nullptr;
		Entity m_entity;
		friend Scene;
	};
}