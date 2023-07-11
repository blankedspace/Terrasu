#pragma once
#include "Entity.h"
namespace Terrasu {


	class EndScreenScript : public NativeScript {

	protected:
		void OnCreate() override;
		void OnUpdate(float dt) override;
	private:
		float waitTime = 0;
		TextUIComponent* GameOverText = nullptr;
		Entity m_entity;
		friend Scene;
	};
}