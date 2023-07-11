#include "EndScreenScript.h"
#include "Input.h"
namespace Terrasu {
	void EndScreenScript::OnCreate(){
		GetComponent<TransformComponent>()->Scale = { 1 ,1 ,1 };
		auto ent = Instantiate<TextUIComponent>("EndGame");
		GameOverText = &ent.GetComponent<TextUIComponent>();
		GameOverText->text = "game over";
		auto& mat = GameOverText->material = entity.GetAssetManager()->CreateMaterial("text");
		mat.textures.push_back(*entity.GetAssetManager()->LoadTexture("Assets/8bitPack.png"));

		Uniform uniform;
		uniform.m_name = "TileData";
		uniform.data = { 49 ,22 ,0 ,0 };


		mat.uniforms.push_back(uniform);

		uniform.m_name = "Fill";
		uniform.data = { 49 ,0 ,0 ,0 };

		mat.uniforms.push_back(uniform);

	
		GameOverText->UpdateText(GameOverText->text); 
		ent.GetComponent<TransformComponent>().Scale = {0.04 ,0.04 ,0.04 };
		ent.GetComponent<TransformComponent>().Translation.x -= 0.15f;
		GetComponent<SpriteComponent>()->material.uniforms[2].data = { 0,0,0,0 };
	}

	void EndScreenScript::OnUpdate(float dt){

		waitTime += dt;
		auto& material = GetComponent<SpriteComponent>()->material;
        material.uniforms[2].data.w += dt * 0.5f;
		if (waitTime > 3)
		{
			waitTime -= 99999;


			auto ent = Instantiate<TextUIComponent>("EndGame");
			ent.GetComponent<TransformComponent>().Scale = { 0.04, 0.04 ,0.04 };
			ent.GetComponent<TransformComponent>().Translation.x -= 0.275f;
			ent.GetComponent<TransformComponent>().Translation.y -= 0.1f;
			GameOverText = &ent.GetComponent<TextUIComponent>();
			GameOverText->text = "click to restart";
			auto& mat = GameOverText->material = entity.GetAssetManager()->CreateMaterial("text");
			mat.textures.push_back(*entity.GetAssetManager()->LoadTexture("Assets/8bitPack.png"));

			Uniform uniform;
			uniform.m_name = "TileData";
			uniform.data = { 49 ,22 ,0 ,0 };


			mat.uniforms.push_back(uniform);
			uniform.m_name = "Fill";
			uniform.data = { 49 ,0 ,0 ,0 };

			mat.uniforms.push_back(uniform);

			GameOverText->UpdateText(GameOverText->text);

		}
		if (waitTime < 0)
		{
			if (Input::Mouse.state == Input::Mouse.leftPressed)
			{
				entity.RestartScene();
			}
		}
	}
}
