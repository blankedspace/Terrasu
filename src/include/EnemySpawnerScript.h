#pragma once
#include "Entity.h"
namespace Terrasu {
	class EnemySpawnerScript : public NativeScript {
	protected:
		void OnUpdate(float dt) override;
		void SpawnEnemy();
	private:
		float SpawnEvery = 15.0f;
		float TimePassed = float(rand()%15);
	};
}