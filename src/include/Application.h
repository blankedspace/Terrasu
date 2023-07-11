#pragma once
#include <string>
#include <memory>

#include "SDL.h"
#include "bgfx/bgfx.h"

#include "Renderer.h"
#include "AssetManager.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

#include "SceneSerializer.h"

namespace Terrasu
{

	class Application
	{
	public:
		Application(int width,int height);
		void Init();
		bool MainLoop();
		void ChangeScene(Scene* scene);
	private:
		SDL_Window* m_window = nullptr;
		int m_width, m_height;
		std::shared_ptr<Renderer> m_renderer;
		std::shared_ptr<AssetManager> m_assetManager;
		std::unique_ptr<Scene> m_activeScene;
		std::unique_ptr<SceneSerializer> m_sceneSerializer;

	};
}