#include<iostream>
#include "Application.h"
#include "../3rdParty/bgfx-imgui/imgui_impl_bgfx.h"
int main(int argc, char* argv[])
{
	{
		Terrasu::Application app(600, 600);
		app.Init();
		app.MainLoop();
	}

	ImGui_Implbgfx_Shutdown();
	bgfx::shutdown();
	return 0;
}