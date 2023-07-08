#include "Scripts.h"
namespace Terrasu {

	void BulletScript::OnUpdate(float dt)
	{
		GetComponent<TransformComponent>().Rotation.z += 10.0f * dt;
		if (!targetIsPlayer)
		GetComponent<TransformComponent>().Translation += Direction * 20.0f * dt;
		else
		GetComponent<TransformComponent>().Translation += Direction * 8.0f * dt;
		m_timeAlive += dt;
		if (m_timeAlive > FiredFrom.TimeToLive)
		{
			Destroy();
		}

	}
	void BulletScript::OnColide(Entity other)
	{
		if (other.HasComponent<NativeScriptComponent>())
		{
			auto& script = other.GetComponent<NativeScriptComponent>();
			if (targetIsPlayer)
			{
				auto& instance = script.GetInstance<PlayerScript>();
				if (&instance != nullptr)
				{
					instance.GetHit(FiredFrom.Dmg);
					if (!FiredFrom.Piercing)
					{
						Destroy();
					}
					
				}
				return;
			}
		
			auto& instance = script.GetInstance<EnemyScript>();
			if (&instance != nullptr)
			{
				instance.GetHit(FiredFrom.Dmg);
				if (!FiredFrom.Piercing)
				{
					Destroy();
				}
			}

		}
	}
}