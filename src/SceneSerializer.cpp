#include "SceneSerializer.h"
#include "NativeScriptComponent.h"
#include "Scripts.h"
#include <fstream>
#include "Entity.h"

#include <iostream>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};


}

namespace Terrasu {
	SceneSerializer::SceneSerializer(AssetManager* assetManager, Scene* scene) :
		m_assetManager(assetManager), m_scene(scene)
	{

	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		for (const auto entityID : m_scene->m_registry.view<TagComponent>()) {
			Entity entity{ entityID,m_scene };
			SerializeEntity(out, entity);
		};
		out << YAML::EndSeq;
		out << YAML::EndMap;

		m_assetManager->WriteFileStr(filepath, out.c_str());


	}
	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity)
	{

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << 1234214;

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>().tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& tc = entity.GetComponent<CameraComponent>();
			out << YAML::Key << "eye" << YAML::Value << tc.eye;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap; // SpriteComponent
			out << YAML::Key << "Order" << YAML::Value << entity.GetComponent<SpriteComponent>().order;
			auto& material = entity.GetComponent<SpriteComponent>().material;
			out << YAML::Key << "Shader" << YAML::Value << material.shader.shaderName;
			out << YAML::Key << "Texture" << YAML::Value << material.textures[0].textureName;
			out << YAML::Key << "Uniforms" << YAML::Value << YAML::BeginSeq;
			for (auto& uni : material.uniforms) {

				out << YAML::BeginMap << YAML::Key << "Name" << YAML::Value << uni.m_name;
				out << YAML::Key << "UniformData" << YAML::Value << uni.data;
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // SpriteComponent
		}
		if (entity.HasComponent<TextUIComponent>())
		{
			out << YAML::Key << "TextUIComponent";
			out << YAML::BeginMap; // TextUIComponent
			out << YAML::Key << "Text" << YAML::Value << entity.GetComponent<TextUIComponent>().text;
			auto& material = entity.GetComponent<TextUIComponent>().material;
			out << YAML::Key << "Shader" << YAML::Value << material.shader.shaderName;
			out << YAML::Key << "Texture" << YAML::Value << material.textures[0].textureName;
			out << YAML::Key << "Uniforms" << YAML::Value << YAML::BeginSeq;
			for (auto& uni : material.uniforms) {

				out << YAML::BeginMap << YAML::Key << "Name" << YAML::Value << uni.m_name;
				out << YAML::Key << "UniformData" << YAML::Value << uni.data;
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // TextUIComponent
		}
		if (entity.HasComponent<NativeScriptComponent>())
		{
			out << YAML::Key << "NativeScriptComponent";
			out << YAML::BeginMap; // TagComponent

			auto& type = entity.GetComponent<NativeScriptComponent>().GetTypeInfo();
			out << YAML::Key << "type" << YAML::Value << type.name();

			out << YAML::EndMap; // TagComponent
		}
		if (entity.HasComponent<SimplePhysicsComponent>())
		{
			out << YAML::Key << "ColiderComponent";
			out << YAML::BeginMap; // ColiderComponent

			auto& Rect = entity.GetComponent<SimplePhysicsComponent>().Rect;
			out << YAML::Key << "Rect" << YAML::Value << Rect;

			out << YAML::EndMap; // ColiderComponent
		}
		out << YAML::EndMap; // Entity
	}
	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data;
		std::string s = m_assetManager->ReadFileStr(filepath);
		data = YAML::Load(s);

		std::string sceneName = data["Scene"].as<std::string>();
		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{

				DeserializeEntity(entity);

			}
		}

		return true;

	}
	Entity SceneSerializer::DeserializeEntity(YAML::Node entity)
	{
		uint64_t uuid = entity["Entity"].as<uint64_t>();

		std::string name;
		auto tagComponent = entity["TagComponent"];
		if (tagComponent)
			name = tagComponent["Tag"].as<std::string>();


		Entity deserializedEntity = m_scene->AddEntity(name);

		auto transformComponent = entity["TransformComponent"];
		if (transformComponent)
		{
			// Entities always have transforms
			auto& tc = deserializedEntity.GetComponent<TransformComponent>();
			tc.Translation = transformComponent["Translation"].as<glm::vec3>();
			tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
			tc.Scale = transformComponent["Scale"].as<glm::vec3>();
		}


		auto spriteComponent = entity["SpriteComponent"];
		if (spriteComponent)
		{
			auto& src = deserializedEntity.AddComponent<SpriteComponent>();

			src.order = spriteComponent["Order"].as<int>();
			src.material = m_assetManager->CreateMaterial(spriteComponent["Shader"].as<std::string>());
			src.material.textures.push_back(*m_assetManager->LoadTexture(spriteComponent["Texture"].as<std::string>()));
			auto uniforms = spriteComponent["Uniforms"];
			for (auto uni : uniforms) {
				Uniform uniform;
				uniform.m_name = uni["Name"].as<std::string>();
				uniform.data = uni["UniformData"].as<glm::vec4>();
				uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
				src.material.uniforms.push_back(uniform);
			}


		}
		auto textUIComponent = entity["TextUIComponent"];
		if (textUIComponent)
		{
			auto& src = deserializedEntity.AddComponent<TextUIComponent>();

			src.text = textUIComponent["Text"].as<std::string>();
			src.material = m_assetManager->CreateMaterial(textUIComponent["Shader"].as<std::string>());
			src.material.textures.push_back(*m_assetManager->LoadTexture(textUIComponent["Texture"].as<std::string>()));
			auto uniforms = textUIComponent["Uniforms"];
			for (auto uni : uniforms) {
				Uniform uniform;
				uniform.m_name = uni["Name"].as<std::string>();
				uniform.data = uni["UniformData"].as<glm::vec4>();
				uniform.m_handle = bgfx::createUniform(uniform.m_name.c_str(), uniform.m_type);
				src.material.uniforms.push_back(uniform);
			}
			src.UpdateText(src.text);

		}

		auto cameraComponent = entity["CameraComponent"];
		if (cameraComponent)
		{
			auto& src = deserializedEntity.AddComponent<CameraComponent>();
			src.eye = cameraComponent["eye"].as<glm::vec3>();
		}

		auto nativeScriptComponent = entity["NativeScriptComponent"];
		if (nativeScriptComponent) {
			auto& src = deserializedEntity.AddComponent<NativeScriptComponent>();
			src.InstantiateScript = ScriptFactory[nativeScriptComponent["type"].as<std::string>()];
		}

		auto coliderComponent = entity["ColiderComponent"];
		if (coliderComponent) {
			auto& src = deserializedEntity.AddComponent<SimplePhysicsComponent>();
			src.Rect = coliderComponent["Rect"].as<glm::vec4>();
		}

		return deserializedEntity;
	}
}