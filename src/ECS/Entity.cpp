#include "AssetManager.h"
#include "ECS/Components.h"
#include "ECS/Scene.h"
#include "ECS/Entity.h"
#include "ECS/SceneSerializer.h"

namespace Terrasu {
	glm::mat4 Entity::GetTransform() const
	{
		
			TransformComponent transfrom = m_scene->m_registry.get<TransformComponent>(m_entity);

			glm::mat4 rotation = glm::toMat4(glm::quat(transfrom.Rotation));
			auto tr = glm::translate(glm::mat4(1.0f), transfrom.Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), transfrom.Scale);

			if (transfrom.parent != -1) {
				Entity ent = { (entt::entity)transfrom.parent, m_scene };

				return ent.GetTransform() * tr;
			}

			return tr;
		
	}
	void Entity::RestartScene(){
		m_scene->Restart();
	}
	AssetManager* Entity::GetAssetManager()
	{
		return m_scene->m_assetManager.get();
	}
	Entity Entity::LoadPrefab(std::string path) {
		SceneSerializer srz{ GetAssetManager(),m_scene };
		return srz.DeserializePrefab(path);
	}
	void Entity::Destroy()
	{
		m_scene->m_toDesroy.insert(m_entity);
		SceneSerializer srz{ GetAssetManager(),m_scene };
		for (auto child : srz.getAllDescendants((uint32_t)m_entity)) {
			m_scene->m_toDesroy.insert((entt::entity)child);
		}
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
		//prioritize children
		auto children = GetComponent<TransformComponent>().children;
		for (auto child : children) {
			auto tg = m_scene->m_registry.get<TagComponent>((entt::entity)child).tag;
			if (tg == tag) {
				Entity temp{(entt::entity)child,m_scene };
				return temp;
			}

		}
		for (auto [ent, script] : m_scene->m_registry.view<TagComponent>().each()) {
			if (m_scene->m_registry.get<TagComponent>(ent).tag == tag)
			{
				Entity temp{ ent,m_scene };
				return temp;
			}

		}

		return {};
	}

	std::vector<Entity> Terrasu::NativeScript::GetChlidren()
	{
		std::vector<Entity> toRet;
		TransformComponent transform = entity.GetComponent<TransformComponent>();
		for (auto child: transform.children) {
			
			toRet.push_back(Entity{ (entt::entity)child,entity.m_scene });
		}
		return toRet;
	}
	std::vector<Entity> Entity::getAllDescendants() {
		auto vec = getAllDescendants((uint32_t)m_entity, m_scene);
		std::vector<Entity> toRet;
		for (auto v : vec) {
			toRet.push_back(Entity{(entt::entity)v, m_scene});
		}
		return toRet;
	}
	std::vector<uint32_t> Entity::getAllDescendants(uint32_t entity_, Scene* scene) {

		std::vector<uint32_t> descendants;
		Terrasu::Entity entity{ (entt::entity)entity_, scene };
		auto directChildren = entity.GetComponent<Terrasu::TransformComponent>().children;

		for (auto child : directChildren) {
			descendants.push_back(child);
			auto vec = getAllDescendants(child, m_scene);
			descendants.insert(descendants.begin(), vec.begin(), vec.end());  // Recursive call
		}
		return descendants;
	}
}