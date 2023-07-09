
workspace "Terrasu"
   configurations { "Debug", "Release" }
   platforms {"x64"}
project "Terrasu"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"

   files {
   "src/**.h", 
   "src/**.cpp",
   "3rdParty/imgui/*.cpp",
   "3rdParty/imgui/backends/imgui_impl_sdl2.cpp",
   "3rdParty/bgfx-imgui/bgfx-imgui/*.cpp"
   }
   includedirs 
   {
    "3rdParty/bgfx/include",
    "3rdParty/bx/include",
    "3rdParty/bimg/include",
    "3rdParty/SDL/include",
    "3rdParty/glm",
    "3rdParty/entt/src",
    "3rdParty/yaml-cpp/include",
    "3rdParty/imgui",
    "3rdParty/bgfx-imgui/bgfx-imgui",
    "src/include"
   }
    libdirs
    {
	    "3rdParty/bgfx/.build/win64_vs2017/bin"
    }
    buildoptions 
	{
		"/Zc:__cplusplus",

	}
   filter "action:vs*"
    includedirs 
   {
    "3rdParty/bx/include/compat/msvc"
   }
   filter "configurations:Debug"
      defines { "DEBUG", "BX_CONFIG_DEBUG=1" }
      staticruntime "on"
      runtime "Debug"
      symbols "On"
        libdirs
        {
            "3rdParty/SDL/VisualC/x64/Debug",
            "3rdParty/yaml-cpp/build/Debug"
        }
        links
		{
		"bxDebug",
		"bimgDebug",
		"bimg_decodeDebug",
		"bgfxDebug",
		"SDL2",
        "SDL2main",
        "yaml-cppd"
		}


   filter "configurations:Release"
      defines { "NDEBUG","BX_CONFIG_DEBUG=0" }
      staticruntime "on"
      optimize "On"
        libdirs
        {
            "3rdParty/SDL/VisualC/x64/Release",
            "3rdParty/yaml-cpp/build/Release"
        }
        links
        {
        "bxRelease",
        "bimgRelease",
        "bimg_decodeRelease",
        "bgfxRelease",
        "SDL2",
        "SDL2main",
        "yaml-cpp"
        }