#pragma once
#include "Entity.h"
#include "WeaponPickupScript.h"
namespace Terrasu {
	class BulletScript : public NativeScript {
	protected:
		void OnUpdate(float dt) override;
		void OnColide(Entity other) override;
	private:
		float m_timeAlive = 0;
	public:
		Weapon FiredFrom;
		glm::vec3 Direction = {0,0,0};
		bool targetIsPlayer = false;
	};
}