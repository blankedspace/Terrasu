#pragma once
#include "Entity.h"
namespace Terrasu {

	class PlayerScript;
	class CameraScript : public NativeScript {
	public:
		glm::vec3 ScreenToWorld(glm::vec3 vec);
	protected:
		void OnCreate() override;
		void OnUpdate(float dt) override;
	private:
		PlayerScript* player = nullptr;
		Entity m_entity;
		friend Scene;
	};
}