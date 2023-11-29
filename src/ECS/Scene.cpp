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
namespace Terrasu {
	Entity Scene::AddEntity(std::string tag)
	{
		Entity ent{ m_registry.create(), this };

		ent.AddComponent<TagComponent>().tag = tag;
		ent.AddComponent<TransformComponent>();
		return ent;
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

		
		PlaySoundTr(nullptr);
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
		CameraComponent cam = camera.GetComponent<CameraComponent>();
		cam.width = cam.height * m_screenwidth/m_screenheight;

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
				m_registry.destroy(ent);
			}
			m_toDesroy.clear();

		}


		auto comparePositions = [](const SpriteComponent& a, const SpriteComponent& b) {
			return a.order < b.order;
		};


		m_registry.sort<SpriteComponent>(comparePositions);
		//Render
		bgfx::touch(0);
		auto viewTS = m_registry.view<TransformComponent, SpriteComponent>();
		for (auto [entity, transform, sprite] : viewTS.each()) {
			m_renderer->DrawQuad(transform, sprite.material);
		}
		auto viewTT = m_registry.view<TransformComponent, TextUIComponent>();
		for (auto [entity, transform, text] : viewTT.each()) {
			m_renderer->DrawText(transform, text);
		}

		auto viewTSp = m_registry.view<TransformComponent, SpineComponent>();
		for (auto [entity, transform, spine] : viewTSp.each()) {
			spine.image->m_assetManager = m_assetManager.get();
			spine.image->transform = transform.GetTransform();
			if (Runtime) {
				spine.image->Update(dt * 1000);
			}
			spine.image->Render(*m_renderer.get());
			//m_renderer->DrawQuad(transform, sprite.material);

		}


		auto viewTLt = m_registry.view<TransformComponent, SpriteLottieComponent>();
		for (auto [entity, transform, lottie] : viewTLt.each()) {
			lottie.order += 1;
			if (lottie.order > lottie.image->totalFrame())
			{
				lottie.order = 0;
			}
			lottie.image->renderSync(lottie.order,*lottie.surface);

			auto mat = m_assetManager->CreateMaterial("texturedQuad");
			lottie.surface->buffer();
			auto mem = bgfx::makeRef(lottie.surface->buffer(), lottie.surface->height() * lottie.surface->bytesPerLine());
			auto handle = bgfx::createTexture2D(
				uint16_t(lottie.surface->width())
				, uint16_t(lottie.surface->height())
				, 1 < 1
				, 1
				, bgfx::TextureFormat::Enum::BGRA8
				, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE
				, mem
			);
			Texture* tex = new Texture();
			tex->handle = handle;
			tex->texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
			mat.textures.push_back(*tex);
			m_renderer->DrawQuad(transform, mat);

		}
		nvgBeginFrame(m_renderer->m_nvg, float(m_screenwidth), float(m_screenheight), 1.0f);
		auto viewTSV = m_registry.view<TransformComponent, SpriteSVGComponent>();
		for (auto [entity, transform, svg] : viewTSV.each()) {
			m_renderer->DrawSvg(transform, svg, float(m_screenwidth), float(m_screenheight), cam);
		}

		nvgEndFrame(m_renderer->m_nvg);
		bgfx::frame();

		Uint64 end = SDL_GetPerformanceCounter();
		dt = (end - start) / (float)SDL_GetPerformanceFrequency();
		timeAlive += dt;
	}

}