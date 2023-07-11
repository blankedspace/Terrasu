#pragma once
#include "Entity.h"
namespace Terrasu {
	class WeaponSpawnerScript : public NativeScript {
	protected:
		void OnUpdate(float dt) override;
	private:
		float SpawnEvery = 10.0f + rand()%15;
		float TimePassed = 0;
	};
}