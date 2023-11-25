#pragma once
#include "Entity.h"
#include "WeaponPickupScript.h"
namespace Terrasu {


	class PlayerScript : public NativeScript {
	public:
		void GetHit(int Dmg);
		void EquipWeapon(Weapon weapon);
		int Difficulty = 0;
	protected:
		void OnCreate() override;
		void OnUpdate(float dt) override;
		void OnColide(Entity other) override;
		

		std::vector<Weapon> AllWeapons;
		std::vector<Entity> UiWeapons;

		TextUIComponent* TimerText = nullptr;
		float currenttime = 0;
		int Health = 100;

		SpriteComponent* Hearts = nullptr;
		int CurrentWeapon = 0;
		float m_shootEvery = 0;
		friend Scene;
	};
}