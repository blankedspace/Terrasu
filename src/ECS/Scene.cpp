#include "AssetManager.h"
#include "bgfx/bgfx.h"
#include "bx/bx.h"
#include "ECS/Scene.h"
#include "bx/math.h"
#include "ECS/Entity.h"
#include <iostream>
#include "SDL.h"
#include "bgfx/platform.h"
#include "imgui_impl_bgfx.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "../ComponentPanel.h"
#include "ECS/SceneSerializer.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "AssetManager.h"
#include "AABB.h"
#include <mutex>

#include <chrono>
#include <ctime>
#include <Sound/SoundManager.h>

#include "UUID.h"
namespace Terrasu {
	Entity Scene::AddEntity(std::string tag)
	{
		return AddEntity(tag, generate_uuid_v4());
	}
	Entity Scene::AddEntity(std::string tag, std::string uuid)
	{
		Entity* ent = new Entity{ m_registry.create(), this };

		ent->AddComponent<TagComponent>().tag = tag;
		ent->GetComponent<TagComponent>().UUID = uuid;
		ent->AddComponent<TransformComponent>();
		return *ent;
	}

	Entity Scene::AddEntity(Entity& ent)
	{
	

		SceneSerializer srz{ m_assetManager.get(),this };
		YAML::Emitter out;
		srz.SerializeEntity(out, ent);

		return srz.DeserializeEntity(YAML::Load(out.c_str()));
	}

	Scene::Scene() {

	}
	Scene::~Scene() {
		if (Runtime) {
			Runtime = false;
		}

		for (const auto entity : m_registry.view<TagComponent>()) {
			m_registry.destroy(entity);
		}
	}
	bool Scene::IsEntityAlive(entt::entity entity) {
		if (m_toDesroy.find(entity) == m_toDesroy.end()) {
			return true;
		}
		return false;
	}

	void Scene::Restart()
	{
		auto m_scene = new Scene();
		SceneSerializer srz{ m_assetManager.get(), m_scene };
		srz.Deserialize("Assets/Scene.scn");
		bgfx::reset(m_screenwidth, m_screenheight, BGFX_RESET_NONE);

		m_scene->m_screenwidth = m_screenwidth;
		m_scene->m_screenheight = m_screenheight;

		m_app->ChangeScene(m_scene);
		Restarted = true;
	}


	void Scene::OnStart()
	{
		srand(int(time(0)));
		bgfx::ViewId m_viewId = 0;
		bgfx::setViewMode(m_viewId, bgfx::ViewMode::Sequential);

		m_componentPanel = std::make_unique<ComponentPanel>();
		m_componentPanel->m_scene = this;
		m_componentPanel->m_assetManager = m_assetManager.get();
		m_componentPanel->m_sceneSerializer = new SceneSerializer(m_assetManager.get(),this );
		m_colisionFunction = [this]()
		{
			
				//around 60 fps
				float updaterate = 16.6f;

				if ((int)(timeAlive * 1000) / updaterate >= collisionframe) {
					collisionframe++;
					auto viewCOL = m_registry.group<TransformComponent, SimplePhysicsComponent>();
					for (auto [ent, transform, col] : viewCOL.each()) {
						if (collisionframe < 0) {
							return;
						}
						if (!IsEntityAlive(ent)) {
							continue;
						}
						transform.Translation += col.Speed * updaterate / 1000.0f;
						for (auto [ent2, transform2, col2] : viewCOL.each()) {

							if (ent == ent2) {
								continue;
							}

							auto aabb = AABB(transform.Translation, transform2.Translation, col.Rect.z, col.Rect.w, col2.Rect.z, col2.Rect.w);
							if (aabb) {

								Entity entity{ ent,this };
								if (entity.HasComponent<NativeScriptComponent>())
								{
									auto& script = entity.GetComponent<NativeScriptComponent>();
									if (script.Instance != nullptr && entity.IsAlive())
									{

										m_Colisions.insert({ ent,ent2 });
									}
								}

								if (col.state == col.PassThrough || col2.state == col2.PassThrough)
								{
									continue;
								}
								auto normal = AABBnormal(transform.Translation,
									transform2.Translation,
									col.Rect.z, col.Rect.w,
									col2.Rect.z, col2.Rect.w,
									glm::length(col.Speed) * updaterate / 1000.0f);

								if (!((col.Speed.y > 0 && normal.y > 0) || (normal.y < 0 && col.Speed.y < 0)) && normal.y != 0) {
									transform.Translation.y -= (col.Speed.y * updaterate / 1000.0f) - normal.y * dt;
								}

								if (!((col.Speed.x > 0 && normal.x > 0) || (normal.x < 0 && col.Speed.x < 0)) && normal.x != 0) {
									transform.Translation.x -= (col.Speed.x * updaterate / 1000.0f) - normal.x * dt;;
								}

								if (glm::length2(normal) == 0 && col.state != col.Static) {
									transform.Translation -= col.Speed * updaterate / 1000.0f;
									auto dist = (transform2.Translation - transform.Translation);
									//Make sure dist!=0
									dist.x += 0.01f;

									transform.Translation -= glm::normalize(dist) * updaterate / 1000.0f;
								}

							}


						}
					}
				}

			
		};

		

		auto viewNS = m_registry.view<NativeScriptComponent>();
		for (auto [ent, script] : viewNS.each()) {

			if (!script.Instance) {
			
				script.Instance = script.InstantiateScript();
				script.Instance->entity.m_entity = ent;
				script.Instance->entity.m_scene = this;
			}
		}

		for (auto [ent, script] : viewNS.each()) {
			script.Instance->OnCreate();
		}
		Runtime = true;
	}

	void Scene::OnUpdate() {

		Uint64 start = SDL_GetPerformanceCounter();
		m_colisionFunction();
		m_toDesroy.clear();
#if BX_CONFIG_DEBUG && BX_PLATFORM_WINDOWS
		ImGui_Implbgfx_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();
		if (ImGui::BeginMainMenuBar()) {

			if (!Runtime && ImGui::MenuItem("Play")) {
				Runtime = true;
				timeAlive = 0;
			}
			else if (Runtime && ImGui::MenuItem("Stop")) {
				Runtime = false;
			}
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Save")) {
					SceneSerializer srz{ m_assetManager.get(),this };
					srz.Serialize("Assets/Scene.scn");
				}
				if (ImGui::MenuItem("Load")) {
					auto m_scene = new Scene();
					SceneSerializer srz{ m_assetManager.get(), m_scene };
					srz.Deserialize("Assets/Scene.scn");
					bgfx::reset(m_screenwidth, m_screenheight, BGFX_RESET_NONE);

					m_scene->m_screenwidth = m_screenwidth;
					m_scene->m_screenheight = m_screenheight;

					m_app->ChangeScene(m_scene);
					ImGui::EndMenu();
					ImGui::EndMainMenuBar();
					ImGui::Render();
					return;
				}
				ImGui::EndMenu();
			}


			ImGui::EndMainMenuBar();
		}
		m_componentPanel->Draw(m_registry);

		ImGui::Render();
		ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
#endif // BX_CONFIG_DEBUG

		//camera



		Entity camera{ m_registry.view<CameraComponent>()[0],this };
		CameraComponent& cam = camera.GetComponent<CameraComponent>();
		cam.width = cam.height * m_screenwidth/m_screenheight;
		cam.screen_h = m_screenheight;
		cam.scren_w = m_screenwidth;

		TransformComponent camtr = camera.GetComponent<TransformComponent>();
		{
			float view[16];
			bgfx::setViewClear(0
				, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
				, cam.backgroundcolor
				, 1.0f
				, 0
			);

			bx::mtxLookAt(view, { cam.eye.x,cam.eye.y,cam.eye.z }, { camtr.Translation.x, camtr.Translation.y, camtr.Translation.z });
			
			float proj[16];
			//bx::mtxOrtho(ortho, centering, m_width + centering, m_height + centering, centering, 0.0f, 100.0f, 0.0f, caps->homogeneousDepth);
			bx::mtxOrtho(proj, -cam.width, cam.width, -cam.height, cam.height, 0.1f, 100.0f, 0, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(0, view, proj);
			bgfx::setViewRect(0, 0, 0, uint16_t(m_screenwidth), uint16_t(m_screenheight));
		}


		//Collision 
		if (m_Colisions.size() > 0 && unlockFromColision.load(std::memory_order_seq_cst)) {

			for (auto& ents : m_Colisions) {
				Entity entity{ ents.first,this };
				if (entity.HasComponent<NativeScriptComponent>())
				{
					auto& script = entity.GetComponent<NativeScriptComponent>();
					if (script.Instance != nullptr && entity.IsAlive())
					{
						script.Instance->OnColide({ ents.second,this });

					}
				}
			}
	
			m_Colisions.clear();
		}
		//Scripts
		auto viewNS = m_registry.view<NativeScriptComponent>();
		if (Runtime)
			for (auto [ent, script] : viewNS.each()) {

				if (!script.Instance) {
					script.Instance = script.InstantiateScript();
					script.Instance->entity.m_entity = ent;
					script.Instance->entity.m_scene = this;
					script.Instance->OnCreate();
				}

				if (IsEntityAlive(ent)) {
					script.Instance->OnUpdate(dt);
					if(Restarted) return;
				}
			}



		if (m_toDesroy.size() > 0) {

			//Destroy entities
			for (auto ent : m_toDesroy) {
				Entity temp{ent,this};
				if (temp.HasComponent<NativeScriptComponent>()) {
					
					temp.GetComponent<NativeScriptComponent>().DestroyScript(temp.GetComponent<NativeScriptComponent>());
				}
				std::cout << "ent" << (uint32_t)ent << std::endl;
				m_registry.destroy(ent);
			}
			m_toDesroy.clear();

		}




		//Render
		bgfx::touch(0);
		m_renderer->drawables.clear();
		auto viewTS = m_registry.view<TransformComponent, SpriteComponent>();
		for (auto [entity, transform, sprite] : viewTS.each()) {
			Entity temp{ entity,this };
			m_renderer->drawables[sprite.order].push_back(std::make_pair((uint32_t)entity, &sprite));
			//sprite.Draw(temp.GetTransform(), m_renderer.get());
			
		}
		auto viewTT = m_registry.view<TransformComponent, TextUIComponent>();
		for (auto [entity, transform, text] : viewTT.each()) {
			Entity temp{ entity,this };
			//m_renderer->drawables[text.order].push_back(&text);
		}


		auto viewTSp = m_registry.view<TransformComponent, SpineComponent>();
		for (auto [entity, transform, spine] : viewTSp.each()) {
			spine.image->m_assetManager = m_assetManager.get();
			Entity temp{ entity,this };
			spine.image->transform = temp.GetTransform();
			Entity ent{ (entt::entity)entity,this };
	
			if (Runtime) {
				spine.image->Update(dt * 1000);
			}
			spine.image->Render(*m_renderer.get());
			//m_renderer->DrawQuad(transform, sprite.material);
			Drawable* a = &spine;
			//m_renderer->drawables[spine.order].push_back(std::make_pair((uint32_t)entity, &spine));

		}


		auto viewTLt = m_registry.view<TransformComponent, SpriteLottieComponent>();
		for (auto [entity, transform, lottie] : viewTLt.each()) {
		
			m_renderer->drawables[lottie.order].push_back(std::make_pair((uint32_t)entity, &lottie));

		}
	
		auto viewTSV = m_registry.view<TransformComponent, SvgComponent>();
		for (auto [entity, transform, svg] : viewTSV.each()) {

			if (!transform.Scale.x || !transform.Scale.y || !transform.Scale.z)
				continue;
			m_renderer->drawables[svg.order].push_back(std::make_pair((uint32_t)entity, &svg));
	
		
		}



		auto viewTTT = m_registry.view<TransformComponent, TextComponent>();
		for (auto [entity, transform, text] : viewTTT.each()) {
			m_renderer->drawables[text.order].push_back(std::make_pair((uint32_t)entity, &text));
			transform.Scale /= 72;
			transform.Scale.y *= -1;
		}
		
		for (auto d : m_renderer->drawables) {
			for (auto b : d.second) {
				
				Entity ent{(entt::entity)b.first,this };
				auto t = ent.GetTransform();
				auto tt = ent.GetComponent<TagComponent>().tag;
				try
				{
					b.second->Draw(
						ent.GetComponent<TransformComponent>(),
						t,
						cam,
						m_renderer.get());
				}
				catch (const std::exception& e)
				{
						std::cout<< e.what();
				}

			}
		}
		for (auto [entity, transform, text] : viewTTT.each()) {
			transform.Scale *= 72;
			transform.Scale.y *= -1;
		}

	
		bgfx::frame();

		Uint64 end = SDL_GetPerformanceCounter();
		dt = (end - start) / (float)SDL_GetPerformanceFrequency();
		timeAlive += dt;
	}

}