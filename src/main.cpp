#include<iostream>
#include "Application.h"
#include "imgui_impl_bgfx.h"
int main(int argc, char* argv[])
{
	{
		Terrasu::Application app(600, 600);
		app.Init();
		app.MainLoop();
	}
#if BX_CONFIG_DEBUG
	ImGui_Implbgfx_Shutdown();
#endif
	bgfx::shutdown();
	return 0;
}