#include  "Scripts.h"
#include "Input.h"
#include "AudioManager.h"
namespace Terrasu {
	void PlayerScript::OnColide(Entity other) {
	}
	void PlayerScript::GetHit(int Dmg) {
		Health -= Dmg;
		Hearts->material.uniforms[1].data.x -= Dmg/100.0f;
		GetComponent<SpriteComponent>()->material.uniforms[1].data = {1,0.2,0.2,1};
		if (Health <= 0) {
			auto ent = Instantiate<SpriteComponent>("EndGame");
			auto sprite = &ent.GetComponent<SpriteComponent>();
			sprite->order = 999;
			sprite->material = entity.GetAssetManager()->CreateMaterial("uq");
			auto& mat = sprite->material;
			mat.textures.push_back(*entity.GetAssetManager()->LoadTexture("Assets/Error.png"));

			Uniform uniform;
			uniform.m_name = "TileData";
			uniform.data = { 1,1,0,0 };

			mat.uniforms.push_back(uniform);


			uniform.m_name = "Fill";
			uniform.data = { 1,0,0,0 };

		
			mat.uniforms.push_back(uniform);

			uniform.m_name = "u_Color";
			uniform.data = { 255,100,100,1 };

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

		auto& mat = sprite.material = entity.GetAssetManager()->CreateMaterial("uq");
		mat.textures.push_back(*entity.GetAssetManager()->LoadTexture("Assets/8bitPack.png"));

		Uniform uniform;
		uniform.m_name = "TileData";
		uniform.data = weapon.tiledata;

		mat.uniforms.push_back(uniform);


		uniform.m_name = "Fill";
		uniform.data = {1,0,0,0};

		mat.uniforms.push_back(uniform);

		uniform.m_name = "u_Color";
		uniform.data = { 1,1,1,1 };


		mat.uniforms.push_back(uniform);



		UiWeapons.push_back(ent);

		UiWeapons[CurrentWeapon].GetComponent<SpriteComponent>().material.uniforms[2].data = { 0,0,0,1 };
	}
	void PlayerScript::OnCreate(){
		Hearts = &FindObject("Hearts").GetComponent<SpriteComponent>();
		Entity timer = FindObject("Timer");
		Entity fpss = FindObject("FPS");
		fps = &fpss.GetComponent<TextUIComponent>();
		TimerText = &timer.GetComponent<TextUIComponent>();
		fps->text = "000";
		fps->UpdateText(fps->text);
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

		TimerText->text = std::to_string((int)currenttime / 60) + ":" + std::to_string((int)currenttime % 60);
		TimerText->UpdateText(TimerText->text);
		
		fps->text = std::to_string(1/dt);
		fps->UpdateText(fps->text);
		GetComponent<SpriteComponent>()->material.uniforms[1].data.y = lerp(GetComponent<SpriteComponent>()->material.uniforms[1].data.y, 1, 3 * dt);
		GetComponent<SpriteComponent>()->material.uniforms[1].data.z = lerp(GetComponent<SpriteComponent>()->material.uniforms[1].data.z, 1, 3 * dt);

		auto physics = GetComponent<SimplePhysicsComponent>();
		auto transform = GetComponent<TransformComponent>();
		physics->Speed = { 0,0,0 };
		if (Input::IsPressed(Keys::SDLK_w)) {
			physics->Speed.y = 5.0f;
		}
		if (Input::IsPressed(Keys::SDLK_a)) {
			physics->Speed.x = -5.0f;
		}
		if (Input::IsPressed(Keys::SDLK_s)) {
			physics->Speed.y = -5.0f;
		}
		if (Input::IsPressed(Keys::SDLK_d)) {
			physics->Speed.x = 5.0f;
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
		if (Input::Mouse.state == Input::Mouse.leftPressed) {
			playSound("Assets/highlands.wav", SDL_MIX_MAXVOLUME);
		}
		if (Input::Mouse.state == Input::Mouse.rightPressed ) {
			if (AllWeapons.size() == 0){
				return;
			}
			if (m_shootEvery > AllWeapons[CurrentWeapon].ShootEvery) {
				//PauseSoundTr();

				data = entity.GetAssetManager()->LoadAudioFile("Assets/test1.mp3");
				//PlaySoundTr(data);
				m_shootEvery = 0;
				int diff = (AllWeapons[CurrentWeapon].Ammount - 1) / 2;
			}
		}
	}
}

