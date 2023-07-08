#pragma once
#define YAML_CPP_STATIC_DEFINE 1
#include "yaml-cpp/yaml.h"
#include "AssetManager.h"
#include "Scene.h"

namespace Terrasu {
	class SceneSerializer {
	public:
		SceneSerializer(AssetManager* assetManager,Scene* scene);

		void Serialize(const std::string& filepath);
		void SerializeEntity(YAML::Emitter& out,Entity entity);
		bool Deserialize(const std::string& filepath);
		Entity DeserializeEntity(YAML::Node ent);

	private:
		AssetManager* m_assetManager = nullptr;
		Scene* m_scene = nullptr;
	};
}