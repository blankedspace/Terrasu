#include "Scripts.h"
namespace Terrasu {
	void EnemyScript::GetHit(int dmg)
	{
		Health--;
		GetComponent<SpriteComponent>()->material.uniforms[1].data = { 1,0.2,0.2,1 };
		if (Health <= 0)
		{
			Destroy();
		}
	}
	void EnemyScript::BecomeGiant()
	{
		Health = 10;
		weapon.Ammount = 26;
		weapon.Angle = 360/20.0f;
		weapon.ShootEvery = 5.0f;
		weapon.TimeToLive = 5.0f;
		GetComponent<TransformComponent>()->Scale *= 6;
		GetComponent<SimplePhysicsComponent>()->Rect = {0,0,6,6};
	}
	void EnemyScript::OnCreate()
	{
		Speed = 0.5f + (rand() % 100)/50.0f;

		weapon.Ammount = 1  + rand() % 4;
		weapon.Angle = 5.0f + rand() % 50;
		weapon.ShootEvery = 1.95f + ((rand() % 100) / 130.0f);

		weapon.Dmg = 3 + 5 * int(weapon.ShootEvery) + int(5 / (float)weapon.Ammount);
		weapon.tiledata = {49,22,34 + rand() % 4,6 + rand()%4};

		weapon.TimeToLive = 0.9f + (rand() % 100) / 25;
		weapon.MaxAmmo = weapon.Ammo = 1000;

		player = FindObjectOfType<PlayerScript>();
	}
	static float lerp(float x, float y, float t) {
		return x + (y - x) * t;
	}
	void EnemyScript::OnUpdate(float dt)
	{
		GetComponent<SpriteComponent>()->material.uniforms[1].data.y = lerp(GetComponent<SpriteComponent>()->material.uniforms[1].data.y, 1, 3 * dt);
		GetComponent<SpriteComponent>()->material.uniforms[1].data.z = lerp(GetComponent<SpriteComponent>()->material.uniforms[1].data.z, 1, 3 * dt);
		auto transform = GetComponent<TransformComponent>();
		player = FindObjectOfType<PlayerScript>();
		if (player == nullptr) {
			return;
		}
		Speed = 2.5f + 0.25f * player->Difficulty;
		glm::vec3 direction = player->GetComponent<TransformComponent>()->Translation - transform->Translation;
		GetComponent<SimplePhysicsComponent>()->Speed = glm::normalize(direction) * Speed;
		if(glm::length2(direction) < 100.0f)
			GetComponent<SimplePhysicsComponent>()->Speed += glm::normalize(glm::vec3{direction.y,direction.x,0});
		
		m_shootEvery += dt;

		if (m_shootEvery > weapon.ShootEvery) {
			m_shootEvery = 0;
			int diff = (weapon.Ammount - 1) / 2;
			for (int i = -diff; i < weapon.Ammount - diff; i++) {

				auto ent = Instantiate<SpriteComponent>("BulletEnemy");
				auto& sprite = ent.GetComponent<SpriteComponent>();
				auto& phys = ent.AddComponent<SimplePhysicsComponent>();
				phys.state = phys.PassThrough;

				auto& mat = sprite.material = entity.GetAssetManager()->CreateMaterial("tiledQuad");
				mat.textures.push_back(*entity.GetAssetManager()->LoadTexture("Assets/8bitPack.png"));

				Uniform uniform;
				uniform.m_name = "TileData";
				uniform.data = weapon.tiledata;

				mat.uniforms.push_back(uniform);


				uniform.m_name = "u_Color";
				uniform.data = { 1 ,1 ,1 ,1 };

				mat.uniforms.push_back(uniform);

				auto bullet = BindScript<BulletScript>(ent);
				bullet->Direction = glm::normalize(player->GetComponent<TransformComponent>()->Translation - transform->Translation);
				bullet->targetIsPlayer = true;
				bullet->FiredFrom = weapon;

				ent.GetComponent<TransformComponent>().Translation = transform->Translation;




				//ent.AddComponent<NativeScriptComponent>().Bind<BulletController>();
				bullet->FiredFrom = weapon;

				glm::vec3 v3(bullet->Direction.x, bullet->Direction.y, 0.0f);

				// create the rotation matrix
				glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(weapon.Angle * i), glm::vec3(0.0f, 0.0f, 1.0f));

				// rotate the vector around the z-axis
				glm::vec3 rotated_v3 = rotation * glm::vec4(v3, 1.0f);

				// convert the rotated vec3 back to a vec2
				bullet->Direction = { rotated_v3.x, rotated_v3.y,0 };

			}


		}
		
	}
}