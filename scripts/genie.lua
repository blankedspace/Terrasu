dofile ("../3rdParty/bx/scripts/toolchain.lua")


solution "Terrasu"
	debugdir ("..")
	language "C++"
	startproject "Terrasu"
		configuration { "x32", "winstore*" }
		targetdir (path.join(BGFX_BUILD_DIR, "win32_" .. _ACTION, "bin", _name))
		objdir (path.join(BGFX_BUILD_DIR, "win32_" .. _ACTION, "obj", _name))

	configuration { "x64", "winstore*" }
		targetdir (path.join(BGFX_BUILD_DIR, "win64_" .. _ACTION, "bin", _name))
		objdir (path.join(BGFX_BUILD_DIR, "win64_" .. _ACTION, "obj", _name))

	configuration { "ARM", "winstore*" }
		targetdir (path.join(BGFX_BUILD_DIR, "arm_" .. _ACTION, "bin", _name))
		objdir (path.join(BGFX_BUILD_DIR, "arm_" .. _ACTION, "obj", _name))
	DIR   = path.getabsolute("..")
	local BUILD_DIR = path.join(DIR, ".build")
	local THIRD_PARTY_DIR = path.join(DIR, "3rdparty")
	toolchain(BUILD_DIR, THIRD_PARTY_DIR) 

project "Terrasu"


	configurations {
		"Debug",
		"Release",
	}
	
	flags {
		"Cpp17",
		"StaticRuntime",
	}
	if _ACTION ~= nil and _ACTION:match "^xcode" then
		platforms {
			"Native", -- let xcode decide based on the target output
		}
	else
		platforms {
			"x32",
			"x64",
--			"Xbox360",
			"Native", -- for targets where bitness is not specified
		}
	end
	language "C++"

		files
	{
	
		"../src/**.cpp",
		"../3rdParty/imgui/*.cpp",
		"../3rdParty/imgui/backends/imgui_impl_sdl2.cpp",
		"../3rdParty/bgfx-imgui/bgfx-imgui/*.cpp",
		"../3rdParty/yaml-cpp/src/**.h",
		"../3rdParty/yaml-cpp/src/**.cpp",		
		"../3rdParty/yaml-cpp/include/**.h"
	
	}

	includedirs
	{
    "../3rdParty/bgfx/include",
    "../3rdParty/bx/include",
    "../3rdParty/bimg/include",
    "../3rdParty/SDL/include",
    "../3rdParty/glm",
    "../3rdParty/entt/src",
    "../3rdParty/yaml-cpp/include",
    "../3rdParty/imgui",
    "../3rdParty/bgfx-imgui/bgfx-imgui/",
    "../src/include"



	}




	configuration { "vs*", "x32 or x64" }
		files
		{
	
		"../3rdParty/sdl-imgui/**.cpp"
		}
			libdirs
		{
		"../3rdParty/SDL/VisualC/x64/Debug",
		"../3rdParty/bgfx/.build/win64_vs2017/bin"
		}
				links
		{
		"bxDebug",
		"bimgDebug",
		"bimg_decodeDebug",
		"bgfxDebug",
		}
	configuration { "wasm*" }
		kind "ConsoleApp"

		buildoptions { "-stdlib=libc++", "-g", "-pthread" }
	

		linkoptions {
			"../../../3rdParty/bgfx/.build/wasm/bin/bxDebug.bc", 
			"../../../3rdParty/bgfx/.build/wasm/bin/bxDebug.bc",
			"../../../3rdParty/bgfx/.build/wasm/bin/bimgDebug.bc",
			"../../../3rdParty/bgfx/.build/wasm/bin/bimg_decodeDebug.bc",
			"../../../3rdParty/bgfx/.build/wasm/bin/bgfxDebug.bc",
			"-s TOTAL_MEMORY=32MB",
			"-s ALLOW_MEMORY_GROWTH=1",
			"-s NO_DISABLE_EXCEPTION_CATCHING",
			"-s EMULATE_FUNCTION_POINTER_CASTS",
			"-s USE_SDL=2",
			"-s NO_EXIT_RUNTIME=0",
			"--preload-file ../../../Assets",
			"-g",
			"-gsource-map"
		}

		removeflags {
			"OptimizeSpeed",
		}

		flags {
			"Optimize"
		}
	configuration "Debug"
		defines 
		{
		"BX_CONFIG_DEBUG=1",
		"YAML_CPP_STATIC_DEFINE"
		
		}
		flags
		{
		"Symbols"
		}

	configuration { "android*" }
		kind "ConsoleApp"
		targetextension ".so"
		linkoptions {
			"-shared",
		}
			files
		{
		"../src/**.c"
		}

		links {
			"EGL",
			"GLESv2",
		}
	configuration {"vs*","Debug"}

		links
		{
		"bxDebug",
		"bimgDebug",
		"bimg_decodeDebug",
		"bgfxDebug",
		"SDL2",
		"SDL2main"

		}

		kind "ConsoleApp"
	configuration {"vs*","Release"}
		defines 
		{
		"BX_CONFIG_DEBUG=0",
		"YAML_CPP_STATIC_DEFINE"
		
		}

		links
		{
		"bxRelease",
		"bimgRelease",
		"bimg_decodeRelease",
		"bgfxRelease",
		"SDL2",
		"SDL2main"

		}

		kind "WindowedApp"