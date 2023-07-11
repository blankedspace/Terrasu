#include<iostream>
#include "Application.h"
#include "imgui_impl_bgfx.h"
#include "bgfx/platform.h"
#if BX_PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#endif

static Terrasu::Application* s_app;
void updateApp()
{
	s_app->MainLoop();
}


int main(int argc, char* argv[])
{
	
		s_app = new Terrasu::Application(600, 600); 
		s_app->Init();
#if BX_PLATFORM_EMSCRIPTEN
		emscripten_set_main_loop(updateApp, 0, 0);
#else
	while (!s_app->MainLoop()) { }
	ImGui_Implbgfx_Shutdown();
	bgfx::shutdown();
#endif

	return 0;
}