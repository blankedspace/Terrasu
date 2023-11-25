#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include "AssetManager.h"
#include "ComponentPanel.h"
#include "ECS/Entity.h"
#include "Scripts/Scripts.h"
#include "ECS/Components.h"
#include "ECS/Scene.h"

namespace Terrasu{
	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		//ImGui::Columns(2);
		//		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::TextUnformatted(label.c_str());
		//ImGui::NextColumn();

		//ImGui::Mu(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = 20.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		ImGui::SetNextItemWidth(30);
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		//		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::SetNextItemWidth(30);
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		//		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::SetNextItemWidth(30);
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		//		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		//ImGui::Columns(1);

		ImGui::PopID();
	}

	void DrawVec4Control(const std::string& label, glm::vec4& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		//ImGui::Columns(2);
		//		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::TextUnformatted(label.c_str());
		//ImGui::NextColumn();

		//ImGui::Mu(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = 20.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		ImGui::SetNextItemWidth(30);
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		//		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::SetNextItemWidth(30);
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		//		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::SetNextItemWidth(30);
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		//		ImGui::PopItemWidth();



		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("W", buttonSize))
			values.w = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		ImGui::SetNextItemWidth(30);
		ImGui::DragFloat("##W", &values.w, 0.1f, 0.0f, 0.0f, "%.2f");
		//		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		//ImGui::Columns(1);

		ImGui::PopID();
	}


	template<typename T>
	static bool DrawComponent(const std::string& name, Entity& entity)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>()){
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			//float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			float lineHeight = 20;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight })){
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings")){
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}


			if (removeComponent){
				entity.RemoveComponent<T>();
				ImGui::TreePop();
				return false;
			}

			
			return open;
		}
		return false;
	}
	
	template<typename T>
	bool ComponentPanel::DisplayAddComponentEntry(const std::string& entryName) {
		if (!m_SelectionContext->HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext->AddComponent<T>();
				ImGui::CloseCurrentPopup();
				return true;
			}
		}
		return false;
	}


	void ComponentPanel::DrawComponents(Entity& entity) {
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().tag;

			char* buffer = strdup(tag.c_str());
			if (ImGui::InputText("##Tag", buffer, 99)) {
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent")) {
			if (DisplayAddComponentEntry<SpriteComponent>("Sprite Component")) {
				auto& mat = m_SelectionContext->GetComponent<SpriteComponent>().material = m_assetManager->CreateMaterial("tiledQuad");
				mat.textures.push_back(*m_assetManager->LoadTexture("Assets/8bitPack.png"));

				Uniform uniform;
				uniform.m_name = "TileData";
				uniform.data = { 48.0,22.0,25.5,1.0 };


				mat.uniforms.push_back(uniform);
			}
			if (DisplayAddComponentEntry<NativeScriptComponent>("Empty Script")) {
				m_SelectionContext->GetComponent<NativeScriptComponent>().Bind<NativeScript>();
			}
			if (DisplayAddComponentEntry<SimplePhysicsComponent>("PhysicsBody")) {

			}
			if (DisplayAddComponentEntry<TextUIComponent>("TextUI")) {

			}


			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		if (DrawComponent<TransformComponent>("Transform", entity)) {
			auto& component = entity.GetComponent<TransformComponent>();
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
			ImGui::TreePop();
		}
		if (DrawComponent<CameraComponent>("Camera", entity)) {
		
			auto& component = entity.GetComponent<CameraComponent>();
			DrawVec3Control("Eye", component.eye);
			ImGui::InputFloat("##camwidth", &component.width);
			ImGui::SameLine();
			ImGui::InputFloat("##camheight", &component.height);
			ImGui::TreePop();
		}
		if (DrawComponent<SpriteComponent>("SpriteComponent", entity)) {
			auto& component = entity.GetComponent<SpriteComponent>();
			Material* mat = &component.material;

			ImGui::InputInt("##order", &component.order);

			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity.GetId(), treeNodeFlags, "Material");
			if (opened) {
				ImGui::Text("Shader:");
				char* buffer = strdup(mat->shader.shaderName.c_str());
				if (ImGui::InputText("##shader", buffer, 99)) {
					mat->shader.shaderName = std::string(buffer);
				}
				ImGui::Text("Textures:");
				for (auto& text : mat->textures) {

					buffer = strdup(text.textureName.c_str());
					if (ImGui::InputText("##texture", buffer, 99)) {
						text.textureName = std::string(buffer);
					}

				}
				if (ImGui::Button("Add"))
				{
					Uniform uni;
					uni.m_name = "Empty" ;
					uni.data = { 0,0,0,0 };

					mat->uniforms.push_back(uni);
				}
				ImGui::SameLine();
				ImGui::Text("Uniforms:");
				int counter = 0;
				for (auto& uni : mat->uniforms) {

					buffer = strdup(uni.m_name.c_str());
					if (ImGui::InputText(std::string("##uniform").append(std::to_string(counter)).c_str(), buffer, 99)) {
						uni.m_name = std::string(buffer);

					}

					ImGui::SameLine();
					if (ImGui::Button("Remove")) {
						mat->uniforms.erase(mat->uniforms.begin() + counter);
					}
					DrawVec4Control(uni.m_name, uni.data);
					counter++;
				}
				if (ImGui::Button("Reload material")) {

					m_assetManager->HotReload();
					Material material = m_assetManager->CreateMaterial(mat->shader.shaderName);
					material.textures.push_back(*m_assetManager->LoadTexture(mat->textures[0].textureName));


					for (auto& uni : mat->uniforms) {

			
						material.uniforms.push_back(uni);
					}
					entity.GetComponent<SpriteComponent>().material = material;



				}


				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		if (DrawComponent<NativeScriptComponent>("NativeScriptComponent", entity)) {

			auto& component = entity.GetComponent<NativeScriptComponent>();

			auto& type = component.GetTypeInfo();

			std::string s = type.name();
			if (ImGui::BeginCombo("##combo", s.c_str())) // The second parameter is the label previewed before opening the combo.
			{
				for (auto factory : ScriptFactory) {

					bool is_selected = (s.c_str() == factory.first.c_str()); // You can store your selection however you want, outside or inside your objects
					if (ImGui::Selectable(factory.first.c_str(), is_selected)) {
						entity.RemoveComponent<NativeScriptComponent>();
						auto& script = entity.AddComponent<NativeScriptComponent>();
						script.InstantiateScript = factory.second;

					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				}
				ImGui::EndCombo();
			}


			ImGui::TreePop();

		};
		if (DrawComponent<SimplePhysicsComponent>("Colider", entity)) {
			auto& component = entity.GetComponent<SimplePhysicsComponent>();
			DrawVec4Control("Rect", component.Rect);
			DrawVec3Control("Speed", component.Speed);
			ImGui::TreePop();
		}
		if (DrawComponent<TextUIComponent>("TextUI", entity)) {
			TextUIComponent& component = entity.GetComponent<TextUIComponent>();
			auto buffer = strdup(component.text.c_str());
			if (ImGui::InputText("text", buffer, 99)) {
				component.text = std::string(buffer);
				component.UpdateText(component.text);
			}

			//TODO: mat draw in separate function;
			Material* mat = &component.material;
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity.GetId(), treeNodeFlags, "Material");
			if (opened) {
				ImGui::Text("Shader:");
				char* buffer = strdup(mat->shader.shaderName.c_str());
				if (ImGui::InputText("##shader", buffer, 99)) {
					mat->shader.shaderName = std::string(buffer);
				}
				if (ImGui::Button("Add"))
				{
					auto texture = m_assetManager->LoadTexture("Assets/Error.png");
					mat->textures.push_back(*texture);
				}
				ImGui::SameLine();
				ImGui::Text("Textures:");
				for (auto& text : mat->textures) {

					buffer = strdup(text.textureName.c_str());
					if (ImGui::InputText("##texture", buffer, 99)) {
						text.textureName = std::string(buffer);
					}

				}
				if (ImGui::Button("AddU"))
				{
					Uniform uni;
					uni.m_name = "Empty";
					uni.data = { 0,0,0,0 };
		
					mat->uniforms.push_back(uni);
				}
				ImGui::SameLine();
				ImGui::Text("Uniforms:");
				int counter = 0;
				for (auto& uni : mat->uniforms) {

					buffer = strdup(uni.m_name.c_str());
					if (ImGui::InputText(std::string("##uniform").append(std::to_string(counter)).c_str(), buffer, 99)) {
						uni.m_name = std::string(buffer);

					}

					ImGui::SameLine();
					if (ImGui::Button("Remove")) {
						mat->uniforms.erase(mat->uniforms.begin() + counter);
					}
					DrawVec4Control(uni.m_name, uni.data);
					counter++;
				}
				if (ImGui::Button("Reload material")) {
					m_assetManager->HotReload();
					Material material = m_assetManager->CreateMaterial(mat->shader.shaderName);
					material.textures.push_back(*m_assetManager->LoadTexture(mat->textures[0].textureName));

					for (auto& uni : mat->uniforms) {

					
						material.uniforms.push_back(uni);
					}
					entity.GetComponent<TextUIComponent>().material = material;

				}
				ImGui::TreePop();
			}
			
			ImGui::TreePop();
		}
	}
	void ComponentPanel::DrawEntityNode(Entity& entity, bool& isPopupOpen) {
		auto& tag = entity.GetComponent<TagComponent>().tag;
		ImGuiTreeNodeFlags flags = (m_SelectionContext!=nullptr && (*m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity.GetId(), flags, "%s", tag.c_str());
		if (ImGui::IsItemClicked())
		{
			delete m_SelectionContext;
			m_SelectionContext = new Entity(entity);
		}

		bool entityDeleted = false;
		if ((m_SelectionContext != nullptr && *m_SelectionContext == entity) && !isPopupOpen && ImGui::BeginPopupContextItem("DeletePop", ImGuiPopupFlags_NoOpenOverExistingPopup | ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::MenuItem(("Delete " + tag).c_str()))
				entityDeleted = true;
			if (ImGui::MenuItem(("Copy " + tag).c_str()))
				m_clipboard = m_SelectionContext;
			isPopupOpen = true;
			ImGui::EndPopup();
		}
		ImGui::PushID(tag.c_str());
		if (ImGui::BeginDragDropTarget()) {
			// Some processing...
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource()) {
			// Some processing...
			ImGui::EndDragDropSource();
		}
		ImGui::PopID();
		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, "%s", tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			if (*m_SelectionContext == entity)
				m_SelectionContext = nullptr;
			entity.Destroy();
		}

	}
	void ComponentPanel::Draw(const entt::registry& m_registry) {
		bool dockspaceOpen = true;
		//ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::Begin("Scene Hierarchy");

		bool isPopupOpen = false;
		for (const auto entityID : m_registry.view<TagComponent>()) {
			Entity entity{entityID,m_scene };
			DrawEntityNode(entity, isPopupOpen);
		}

		//if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			//m_SelectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow("CreateEntitiesPop", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)){
			if (ImGui::MenuItem("Create Empty Entity")){
				auto entityexample = m_scene->AddEntity("Entity");
				auto& mat = entityexample.AddComponent<SpriteComponent>().material = m_assetManager->CreateMaterial("tiledQuad");
				mat.textures.push_back(*m_assetManager->LoadTexture("Assets/8bitPack.png"));

				Uniform uniform;
				uniform.m_name = "TileData";
				uniform.data = { 49.0,22.0,25.5,1.0 };


				mat.uniforms.push_back(uniform);
			}
				//AddEntity("Empty Entity")->AddComponent<SpriteRenderComponent>().m_textureColor = Renderer::TryLoadTexture("Assets/Error.png");
			if (ImGui::MenuItem("Paste"))
				m_scene->AddEntity(*m_clipboard);

				ImGui::EndPopup();
		}

		
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext != nullptr && m_SelectionContext->GetId() != entt::null)
		{
			DrawComponents(*m_SelectionContext);
		}

		ImGui::End();

	}


}