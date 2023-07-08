#pragma once
#include "NativeScriptComponent.h"
#include "WeaponPickupScript.h" 
namespace Terrasu {
	class EnemyScript : public NativeScript {
	public:
		void GetHit(int dmg);
		void BecomeGiant();
	protected:
		void OnCreate() override;
		virtual void OnDestroy() {}
		void OnUpdate(float dt) override;
		virtual void OnColide(Entity other) {}
		PlayerScript* player = nullptr;
		Weapon weapon;
		float m_shootEvery = 0;
		int Health = 3;
		float Speed = 2.5f;
		friend Scene;

	};
}