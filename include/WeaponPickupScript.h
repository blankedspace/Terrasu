#pragma once
#include "Entity.h"
namespace Terrasu {

	struct Weapon {

		float ShootEvery = 0.1f;
		float Angle = 15;
		int Ammount = 1;
		int Ammo = 100;
		int MaxAmmo = 100;
		int Dmg = 1;
		bool Piercing = false;
		float TimeToLive = 0.6f;

		glm::vec4 tiledata = { 49 ,22 ,33 ,2 };

	};


	class WeaponPickupScript : public NativeScript {
	protected:
		void OnCreate() override;
		void OnUpdate(float dt) override;
		void OnColide(Entity other) override;

		glm::vec3 StartPos = {0,0,0};
		float TimePassed;
		Weapon WeaponToPickUp;
		friend Scene;

	};
}