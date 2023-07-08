#pragma once
#include "NativeScriptComponent.h"
namespace Terrasu {
	class WeaponSpawnerScript : public NativeScript {
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		void OnUpdate(float dt) override;
		virtual void OnColide(Entity other) {}
	private:
		float SpawnEvery = 10.0f + rand()%15;
		float TimePassed = 0;
	};
}