#include  "Scripts.h"
namespace Terrasu {
	void PlayerScript::OnColide(Entity other) {
	}
	void PlayerScript::GetHit(int Dmg) {
		Health -= Dmg;
		Hearts->material.uniforms[1].data.x -= Dmg/(float)100;
		GetComponent<SpriteComponent>().material.uniforms[1].data = {1,0.2,0.2,1};
		if (Health <= 0) {
			auto ent = Instantiate<SpriteComponent>("EndGame");
			auto& sprite = ent.GetComponent<SpriteComponent>();
			sprite.order = 999;
			auto& mat = sprite.material = entity.GetAssetManager()->CreateMaterial("UIfillQuad");
			mat.textures.push_back(*entity.GetAssetManager()->LoadTexture("Assets/Error.png"));

			Uniform uniform;
			uniform.m_name = "TileData";
			uniform.data = { 1,1,0,0 };

			uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
			mat.uniforms.push_back(uniform);


			uniform.m_name = "Fill";
			uniform.data = { 1,0,0,0 };

			uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
			mat.uniforms.push_back(uniform);

			uniform.m_name = "u_Color";
			uniform.data = { 0,0,0,1 };

			uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
			mat.uniforms.push_back(uniform);

			BindScript<EndScreenScript>(ent);
			Destroy();
		}
	}
	void PlayerScript::EquipWeapon(Weapon weapon)
	{
		AllWeapons.push_back(weapon);


		auto ent = Instantiate<SpriteComponent>("UiWeapon");
		auto& sprite = ent.GetComponent<SpriteComponent>();
		ent.GetComponent<TransformComponent>().Translation = { 0.20f + AllWeapons.size() / 16.0f ,0.9f,0 };
		ent.GetComponent<TransformComponent>().Scale = { 0.04f ,0.04f ,0.04f };

		auto& mat = sprite.material = entity.GetAssetManager()->CreateMaterial("UIfillQuad");
		mat.textures.push_back(*entity.GetAssetManager()->LoadTexture("Assets/8bitPack.png"));

		Uniform uniform;
		uniform.m_name = "TileData";
		uniform.data = weapon.tiledata;

		uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
		mat.uniforms.push_back(uniform);


		uniform.m_name = "Fill";
		uniform.data = {1,0,0,0};

		uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
		mat.uniforms.push_back(uniform);

		uniform.m_name = "u_Color";
		uniform.data = { 1,1,1,1 };

		uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
		mat.uniforms.push_back(uniform);



		UiWeapons.push_back(ent);

		UiWeapons[CurrentWeapon].GetComponent<SpriteComponent>().material.uniforms[2].data = { 0,0,0,1 };
	}
	void PlayerScript::OnCreate(){
		Hearts = &FindObject("Hearts").GetComponent<SpriteComponent>();
		auto& timer = FindObject("Timer");
		if (&timer != nullptr) {
			TimerText = &timer.GetComponent<TextUIComponent>();
		}
		Weapon weapon;
		weapon.ShootEvery = 1;
		weapon.Ammo = 999999;
		EquipWeapon(weapon);
		TimerText->text = "0:0";
		TimerText->UpdateText(TimerText->text);

	}
	static float lerp(float x, float y, float t) {
		return x + (y - x) * t;
	}
	void PlayerScript::OnUpdate(float dt) {

		currenttime += dt;
		if (int(currenttime * 10.0f) % 10) {
			TimerText->text = std::to_string((int)currenttime / 60) + ":" + std::to_string((int)currenttime % 60);
			TimerText->UpdateText(TimerText->text);
			Difficulty = int(currenttime/30);
		}

		GetComponent<SpriteComponent>().material.uniforms[1].data.y = lerp(GetComponent<SpriteComponent>().material.uniforms[1].data.y, 1, 3 * dt);
		GetComponent<SpriteComponent>().material.uniforms[1].data.z = lerp(GetComponent<SpriteComponent>().material.uniforms[1].data.z, 1, 3 * dt);

		auto& physics = GetComponent<SimplePhysicsComponent>();
		auto& transform = GetComponent<TransformComponent>();
		physics.Speed = { 0,0,0 };
		if (Input::IsPressed(Keys::SDLK_w)) {
			physics.Speed.y = 5.0f;
		}
		if (Input::IsPressed(Keys::SDLK_a)) {
			physics.Speed.x = -5.0f;
		}
		if (Input::IsPressed(Keys::SDLK_s)) {
			physics.Speed.y = -5.0f;
		}
		if (Input::IsPressed(Keys::SDLK_d)) {
			physics.Speed.x = 5.0f;
		}
		for (int i = 0; i <= AllWeapons.size() && i < 10; i++)
		{
			if (Input::IsPressed((Keys)std::to_string(i).c_str()[0]))
			{
				UiWeapons[CurrentWeapon].GetComponent<SpriteComponent>().material.uniforms[2].data = { 1,1,1,1 };
				CurrentWeapon = i-1;
				UiWeapons[CurrentWeapon].GetComponent<SpriteComponent>().material.uniforms[2].data = {0,0,0,1};
			}
		}
	
		m_shootEvery += dt;
		if (Input::Mouse.state == Input::Mouse.rightPressed ) {
			if (AllWeapons.size() == 0){
				return;
			}
			if (m_shootEvery > AllWeapons[CurrentWeapon].ShootEvery){
				m_shootEvery = 0;
				int diff = (AllWeapons[CurrentWeapon].Ammount - 1) / 2;
				for (int i = -diff; i < AllWeapons[CurrentWeapon].Ammount - diff; i++){

					AllWeapons[CurrentWeapon].Ammo--;
					UiWeapons[CurrentWeapon].GetComponent<SpriteComponent>().material.uniforms[1].data.x = AllWeapons[CurrentWeapon].Ammo/(float)AllWeapons[CurrentWeapon].MaxAmmo;
					if (AllWeapons[CurrentWeapon].Ammo <= 0){
						AllWeapons.erase(AllWeapons.begin() + CurrentWeapon);
						UiWeapons.erase(UiWeapons.begin() + CurrentWeapon);
						for (int i = CurrentWeapon; i < UiWeapons.size(); i++)
						{
							UiWeapons[i].GetComponent<TransformComponent>().Translation.x -= 1.0f/16.0f;
						}

						CurrentWeapon = 0;
						if (AllWeapons.size() == 0)
							return;
						UiWeapons[CurrentWeapon].GetComponent<SpriteComponent>().material.uniforms[2].data = { 0,0,0,1 };
						return;
					}
					auto ent = Instantiate<SpriteComponent>("Bullet");
					auto& sprite = ent.GetComponent<SpriteComponent>();
					auto& phys = ent.AddComponent<SimplePhysicsComponent>();
					phys.state = phys.PassThrough;

					auto& mat = sprite.material = entity.GetAssetManager()->CreateMaterial("tiledQuad");
					mat.textures.push_back(*entity.GetAssetManager()->LoadTexture("Assets/8bitPack.png"));

					Uniform uniform;
					uniform.m_name = "TileData";
					uniform.data = AllWeapons[CurrentWeapon].tiledata;

					uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
					mat.uniforms.push_back(uniform);


					uniform.m_name = "u_Color";
					uniform.data = { 1 ,1 ,1 ,1 };

					uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
					mat.uniforms.push_back(uniform);

					auto& bullet = BindScript<BulletScript>(ent);

					ent.GetComponent<TransformComponent>().Translation = transform.Translation;
					auto point = FindObjectOfType<CameraScript>().ScreenToWorld({ Input::Mouse.x, Input::Mouse.y, 0 });
					bullet.Direction = glm::normalize(point - transform.Translation);

					//ent.AddComponent<NativeScriptComponent>().Bind<BulletController>();
					bullet.FiredFrom = AllWeapons[CurrentWeapon];

					glm::vec3 v3(bullet.Direction.x, bullet.Direction.y, 0.0f);

					// create the rotation matrix
					glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(AllWeapons[CurrentWeapon].Angle * i), glm::vec3(0.0f, 0.0f, 1.0f));

					// rotate the vector around the z-axis
					glm::vec3 rotated_v3 = rotation * glm::vec4(v3, 1.0f);

					// convert the rotated vec3 back to a vec2
					bullet.Direction = { rotated_v3.x, rotated_v3.y,0 };
				}
			}
		}
	}
}

