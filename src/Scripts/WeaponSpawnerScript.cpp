#include "WeaponSpawnerScript.h"
#include "WeaponPickupScript.h"
#include "HealthPackScript.h"
namespace Terrasu {
	void WeaponSpawnerScript::OnUpdate(float dt)
	{
		TimePassed += dt;
		if (TimePassed > SpawnEvery) {
			TimePassed = 0;
			auto ent = Instantiate<SpriteComponent>("Weapon");
			auto& sprite = ent.GetComponent<SpriteComponent>();
			auto& colider = ent.AddComponent<SimplePhysicsComponent>();
			colider.Speed = glm::vec3(0);
			auto& mat = sprite.material = entity.GetAssetManager()->CreateMaterial("tiledQuad");
			mat.textures.push_back(*entity.GetAssetManager()->LoadTexture("Assets/8bitPack.png"));

			Uniform uniform;
			uniform.m_name = "TileData";
			uniform.data = { 49 ,22 ,34 + rand()%6 , 7 + rand() % 2 };

			uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
			mat.uniforms.push_back(uniform);


			uniform.m_name = "u_Color";
			uniform.data = { 1 ,1 ,1 ,1 };

			uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
			mat.uniforms.push_back(uniform);

			ent.GetComponent<TransformComponent>().Translation = GetComponent<TransformComponent>().Translation + glm::vec3{rand() % 3, rand() % 3, rand() % 3 };
			BindScript<WeaponPickupScript>(ent);

			if (rand() % 2) {
				auto ent = Instantiate<SpriteComponent>("Food");
				auto& sprite = ent.GetComponent<SpriteComponent>();
				auto& colider = ent.AddComponent<SimplePhysicsComponent>();
				colider.Speed = glm::vec3(0);
				auto& mat = sprite.material = entity.GetAssetManager()->CreateMaterial("tiledQuad");
				mat.textures.push_back(*entity.GetAssetManager()->LoadTexture("Assets/8bitPack.png"));

				Uniform uniform;
				uniform.m_name = "TileData";
				uniform.data = { 49 ,22 ,34 + rand() % 2 , 15 + rand() % 4 };

				uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
				mat.uniforms.push_back(uniform);


				uniform.m_name = "u_Color";
				uniform.data = { 1 ,1 ,1 ,1 };

				uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
				mat.uniforms.push_back(uniform);

				ent.GetComponent<TransformComponent>().Translation = GetComponent<TransformComponent>().Translation + glm::vec3{ rand() % 3, rand() % 3, rand() % 3 };
				BindScript<HealthPackScript>(ent);

			}
		}
	}
}