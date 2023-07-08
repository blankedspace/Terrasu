#pragma once
#include "NativeScriptComponent.h"
namespace Terrasu {


	class HealthPackScript : public NativeScript {

	protected:
		void OnCreate() override;
		virtual void OnDestroy() {}
		void OnUpdate(float dt) override;
		void OnColide(Entity other) override;
	private:
		glm::vec3 StartPos = { 0,0,0 };
		float TimePassed;
		friend Scene;
	};
}