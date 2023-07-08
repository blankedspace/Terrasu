#include "Scripts.h"
namespace Terrasu {
	void CameraScript::OnCreate() {
	 
		
	}
	static float lerp(float x, float y, float t) {
		return x + (y - x) * t;
	}
	void CameraScript::OnUpdate(float dt)
	{
		auto& camera = GetComponent<CameraComponent>();
		auto& transform = GetComponent<TransformComponent>();

		player = &FindObjectOfType<PlayerScript>();
		if (player == nullptr) {
			return;
		}
		auto& target = player->GetComponent<TransformComponent>();

		camera.eye.x = lerp(camera.eye.x, target.Translation.x, 2*dt );
		camera.eye.y = lerp(camera.eye.y, target.Translation.y, 2*dt );

		if (camera.eye.y > 20) {
			camera.eye.y = 20;
		}
		if (camera.eye.y < -30){
			camera.eye.y = -30;
		}
		if (camera.eye.x < -50){
			camera.eye.x = -50;
		}
		if (camera.eye.x > 50){
			camera.eye.x = 50;
		}
		transform.Translation = camera.eye;
		transform.Translation.z = 0;
	}

	glm::vec3 CameraScript::ScreenToWorld(glm::vec3 vec)
	{
		auto screen = GetScreenSize();
		glm::vec3 point = {0,0,0};

		point.y = 0.5f - vec.y/screen.y ;
		point.x = vec.x/screen.x - 0.5f;

		point.x *= GetComponent<CameraComponent>().width * 2;
		point.y *= GetComponent<CameraComponent>().height * 2;
		point.x += GetComponent<TransformComponent>().Translation.x;
		point.y += GetComponent<TransformComponent>().Translation.y;
	
		return point;
	}

	
}
