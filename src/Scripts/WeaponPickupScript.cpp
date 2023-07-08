#include "Scripts.h"
namespace Terrasu {
	void WeaponPickupScript::OnCreate()
	{
		auto& transform = GetComponent<TransformComponent>();
		auto& spr = GetComponent<SpriteComponent>();
		GetComponent<SimplePhysicsComponent>().state = SimplePhysicsComponent::PassThrough;
		StartPos = transform.Translation;
		WeaponToPickUp.Ammount = 1 + rand() % 6;
		WeaponToPickUp.Angle = 5.0f + rand() % 20;
		WeaponToPickUp.ShootEvery = 0.15f + ((rand() % 100) / 330.0f);

		WeaponToPickUp.Dmg = 9 + 5 * int(WeaponToPickUp.ShootEvery) +int( 5 / (float)WeaponToPickUp.Ammount);
		WeaponToPickUp.tiledata = spr.material.uniforms[0].data;

		WeaponToPickUp.TimeToLive = 0.7f + (rand() % 100) / 70;
		WeaponToPickUp.MaxAmmo = WeaponToPickUp.Ammo = 60 + rand() % 20;
		int r = rand() % 10;
		if (r > 7)
			WeaponToPickUp.Piercing = true;
	}
	void WeaponPickupScript::OnUpdate(float dt)
	{
		TimePassed += dt * 3;
		auto& transform = GetComponent<TransformComponent>();
		transform.Translation.y = StartPos.y + glm::sin(TimePassed) / 3;

	}
	void WeaponPickupScript::OnColide(Entity other)
	{
		if (other.HasComponent<NativeScriptComponent>())
		{
			auto& script = other.GetComponent<NativeScriptComponent>();
			auto& instance = script.GetInstance<PlayerScript>();
			if (&instance != nullptr)
			{
			
				instance.EquipWeapon(WeaponToPickUp);
				Destroy();
			}
		}
	}
}