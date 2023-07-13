#include "Application.h"
#include <SDL_syswm.h>
#include <iostream>
#include "Input.h"

#if BX_PLATFORM_WINDOWS
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "imgui_impl_bgfx.h"
#endif
#if BX_PLATFORM_ANDROID
namespace bx {
	Location Location::current(const char* _filePath, uint32_t _line)
	{
		return Location(_filePath, _line);
	}
}
#endif
namespace Terrasu
{

	Application::Application(int width,int height):m_width(width),m_height(height)
	{
	
	}
	void Application::Init()
	{
		//initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		}
		SDL_SetHint(SDL_HINT_VIDEO_EXTERNAL_CONTEXT, "1");
		m_window = SDL_CreateWindow("Terrasu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, SDL_WINDOW_RESIZABLE);
		SDL_SysWMinfo wmi;
		SDL_VERSION(&wmi.version);
		if (!SDL_GetWindowWMInfo(m_window, &wmi))
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't Get native window SDL: %s", SDL_GetError());
		}
		//initialize BGFX
		bgfx::Init init;
#if BX_PLATFORM_WINDOWS
		
		init.type = bgfx::RendererType::Direct3D11;
		init.platformData.nwh = wmi.info.win.window;
#elif BX_PLATFORM_EMSCRIPTEN 
		init.type = bgfx::RendererType::OpenGLES;
		init.platformData.nwh = (void*)"#canvas";
#elif BX_PLATFORM_ANDROID

		init.type = bgfx::RendererType::OpenGL;
		init.platformData.nwh =  wmi.info.android.window;

#endif
		init.resolution.width = m_width;
		init.resolution.height = m_height;
		init.resolution.reset = BGFX_RESET_VSYNC;
		bgfx::init(init);
		bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
		);
		bgfx::setViewRect(0, 0, 0, m_width, m_height);
	
		//initialize Game
		m_renderer = std::make_shared<Renderer>();
		m_assetManager = std::make_shared<AssetManager>();
		m_activeScene = std::make_unique<Scene>();
		m_renderer->m_assetManager = m_assetManager;

		m_activeScene->m_screenheight = m_height;
		m_activeScene->m_screenwidth = m_width;
		m_activeScene->m_renderer = m_renderer;
		m_activeScene->m_assetManager = m_assetManager;
		m_activeScene->m_app = this;

		m_sceneSerializer = std::make_unique<SceneSerializer>( m_assetManager.get(), m_activeScene.get());
		//m_sceneSerializer->Serialize("Assets/scene.scn");

		m_sceneSerializer->Deserialize("Assets/scene.scn");
		m_activeScene->OnStart();

#if BX_PLATFORM_WINDOWS
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui_Implbgfx_Init(255);

		ImGui_ImplSDL2_InitForD3D(m_window);
#endif

	}

	bool Application::MainLoop()
	{
		bool exit = false;
		SDL_Event event;


			//Input

			SDL_GetMouseState(&Input::Mouse.x, &Input::Mouse.y);
			while (SDL_PollEvent(&event) > 0) {
#if BX_PLATFORM_WINDOWS
				ImGui_ImplSDL2_ProcessEvent(&event);
#endif
				switch (event.type)
				{
				case SDL_MOUSEBUTTONDOWN:
				{
					const SDL_MouseButtonEvent& mev = event.button;
					if (mev.button == SDL_BUTTON_LEFT)
						Input::Mouse.state = Input::Mouse.leftPressed;
					if (mev.button == SDL_BUTTON_RIGHT)
						Input::Mouse.state = Input::Mouse.rightPressed;
					break;
				}
				case SDL_MOUSEBUTTONUP:
					Input::Mouse.state = Input::Mouse.none;
					break;
				case SDL_KEYDOWN:
					Input::SetKeyState((Keys)event.key.keysym.sym, true);
					break;
				case SDL_KEYUP:
					Input::SetKeyState((Keys)event.key.keysym.sym, false);
					break;

				case SDL_WINDOWEVENT:
				{
					const SDL_WindowEvent& wev = event.window;
					switch (wev.event)
					{
				
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					{
						m_width = wev.data1;
						m_height = wev.data2;

						m_activeScene->m_screenheight = m_height;
						m_activeScene->m_screenwidth = m_width;

						bgfx::reset(m_width, m_height, BGFX_RESET_VSYNC);
						//bgfx::setViewRect(0, 0, 0, m_width, m_height);
						break;
					}
					case SDL_WINDOWEVENT_CLOSE:
					{
						exit = true;
					}
					break;
					}
				}

				}
			}
			//Render
			m_activeScene->OnUpdate();
		


		return exit;

	}

	void Application::ChangeScene(Scene* scene)
	{
		m_activeScene = std::unique_ptr<Scene>(scene);
		m_activeScene->m_renderer = m_renderer;
		m_activeScene->m_assetManager = m_assetManager;
		m_activeScene->m_app = this;
		m_activeScene->OnStart();
	}

}	