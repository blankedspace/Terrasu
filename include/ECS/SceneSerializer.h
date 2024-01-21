#pragma once
#include "yaml-cpp/yaml.h"
#include "AssetManager.h"
#include "Scene.h"

namespace Terrasu {
	class SceneSerializer {
	public:
		SceneSerializer(AssetManager* assetManager,Scene* scene);

		void Serialize(const std::string& filepath);
		void SerializeEntity(YAML::Emitter& out,Entity entity);
		std::vector<uint32_t> getAllDescendants(uint32_t entityID);
		void SerializePrefab(Entity& entity);
		Entity DeserializePrefab(const std::string& filepath);
		bool Deserialize(const std::string& filepath);
		Entity DeserializeEntity(YAML::Node ent);

	private:
		AssetManager* m_assetManager = nullptr;
		Scene* m_scene = nullptr;
	};
}