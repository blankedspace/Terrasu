#include "Scripts.h"
namespace Terrasu {
	void EnemySpawnerScript::SpawnEnemy()
	{

		auto ent = Instantiate<SpriteComponent>("Enemy");
		auto& sprite = ent.GetComponent<SpriteComponent>();
		auto& colider = ent.AddComponent<SimplePhysicsComponent>();
		colider.Speed = glm::vec3(0);
		auto& mat = sprite.material = entity.GetAssetManager()->CreateMaterial("tiledQuad");
		mat.textures.push_back(*entity.GetAssetManager()->LoadTexture("Assets/8bitPack.png"));

		Uniform uniform;
		uniform.m_name = "TileData";
		uniform.data = { 49 ,22 ,25 + rand() % 5  ,6 + rand() % 5 };


		mat.uniforms.push_back(uniform);


		uniform.m_name = "u_Color";
		uniform.data = { 1 ,1 ,1 ,1 };


		mat.uniforms.push_back(uniform);

		ent.GetComponent<TransformComponent>().Translation = GetComponent<TransformComponent>()->Translation;
		auto enemy = BindScript<EnemyScript>(ent);
		if ((rand() % 100 ) > 95) {
			enemy->BecomeGiant();
		}

	}
	void EnemySpawnerScript::OnUpdate(float dt)
	{
		auto player = FindObjectOfType<PlayerScript>();
		if (player == nullptr) {
			return;
		}
		TimePassed += dt +(rand() % 2) * dt;
		if (TimePassed > SpawnEvery) {
			TimePassed = (rand() % 100) * 0.01f;
			SpawnEnemy();
			if (rand() % 10 == 9) {
				SpawnEnemy();
			}
		}
	}

}