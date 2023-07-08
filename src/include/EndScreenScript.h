#pragma once
#include "NativeScriptComponent.h"
namespace Terrasu {


	class EndScreenScript : public NativeScript {

	protected:
		void OnCreate() override;
		virtual void OnDestroy() {}
		void OnUpdate(float dt) override;
		virtual void OnColide(Entity other) {}
	private:
		float waitTime = 0;
		TextUIComponent* GameOverText = nullptr;
		Entity m_entity;
		friend Scene;
	};
}