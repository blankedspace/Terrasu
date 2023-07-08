#pragma once
#include "NativeScriptComponent.h"
namespace Terrasu {
	class EnemySpawnerScript : public NativeScript {
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		void OnUpdate(float dt) override;
		virtual void OnColide(Entity other) {}
		void SpawnEnemy();
	private:
		float SpawnEvery = 15.0f;
		float TimePassed = float(rand()%15);
	};
}