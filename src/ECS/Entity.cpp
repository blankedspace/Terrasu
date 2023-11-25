#include "AssetManager.h"
#include "ECS/Components.h"
#include "ECS/Scene.h"
#include "ECS/Entity.h"

namespace Terrasu {

	void Entity::RestartScene(){
		m_scene->Restart();
	}
	AssetManager* Entity::GetAssetManager()
	{
		return m_scene->m_assetManager.get();
	}
	void Entity::Destroy()
	{
		m_scene->m_toDesroy.insert(m_entity);
	}
	bool Entity::IsAlive()
	{
		return m_scene->IsEntityAlive(m_entity);
	}
	glm::vec2  Entity::GetScreenSize()
	{
		return { m_scene->m_screenwidth,m_scene->m_screenheight };
	}
	Entity Entity::FindObject(std::string tag)
	{
		for (auto [ent, script] : m_scene->m_registry.view<TagComponent>().each()) {

			auto tg = m_scene->m_registry.get<TagComponent>(ent).tag;
			if (m_scene->m_registry.get<TagComponent>(ent).tag == tag)
			{
				Entity temp{ ent,m_scene };
				return temp;
			}

		}

		return {};
	}
}