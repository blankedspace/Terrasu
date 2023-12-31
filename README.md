 <img
  src="https://github.com/blankedspace/Terrasu/blob/main/Assets/ScreenShot.png"
  width="700" 
  alt="Game screenshot"
  title="Game screenshot">
  
bgfx + SDL cross-platfrom C++ game   (android wasm win).
# Terrasu
to download project run
```
git clone --recursive https://github.com/blankedspace/Terrasu
```
# Building for Windows (Visual Studio)
To compile Terrasu for windows run:
```
3rdParty\Premake\premake5.exe vs2019
```
All instructions for premake tool is located in premake5.lua file, you can read it to understand dependencies and defines and other compiler flags.
Now we have Terrasu.sln and can just open it in visual studio, but at first lets build our dependencies.
- SDL2[https://wiki.libsdl.org/SDL2/Installation] "Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware."
In simpler words - SDL creates window and gets mouse and keyboard input(or touches if we use mobile)
Simplest way to build is to use vs project files in 3rdParty\SDL\VisualC\SDL.sln
But ofcourse you can try use Cmake.
- bgfx[https://bkaradzic.github.io/bgfx/build.html] "Cross-platform, graphics API agnostic, "Bring Your Own Engine/Framework" style rendering library." In simpler words there are a lot of ways to access your GPU
OpenGl, DirectX, Vulkan... and what to choose depends on build target(DirectX is Windows exclusive for example). So we can just call bgfx functions to build with different renderer API.
```
  Bgfx uses self-made tool GENiE - but actually its a fork of premake4 with some changes.
  cd 3rdParty\bgfx
  and run
  ..\bx\tools\bin\windows\genie --with-examples vs2017 (check out examples a lot of interesting info)
  and again build it with Visual studio
```
- yaml-cpp [https://github.com/jbeder/yaml-cpp] "yaml-cpp is a YAML parser and emitter in C++ matching the YAML 1.2 spec." I use it to save Scene files in my game. You can check Assets\Scene.scn to see how YAML looks
	yaml-cpp uses cmake to generate vs project files. But in "yaml-cpp static" project change /MD to /MT in release and /MDd to /MTd in debug [https://stackoverflow.com/questions/757418/should-i-compile-with-md-or-mt]
```
  cd 3rdParty\yaml-cpp
  mkdir build
  cd build
  cmake ...
  and again build it with Visual studio
```

After building all dependencies check that everything is in right path(Debug and Release) in premake5.lua.
And now we are ready to build Terrasu executable!

But to run the programm we need Assets folder and SLD2.dll(it was build with static lib) in the same directory as our
executable, if we debug with visual studio its directory where .sln file is located.

Oh and we need to compile shaders, this is just bgfx thing, because of support for different graphics API, which use different shader languages
Again i recommend reading about shaders... and honestly [https://learnopengl.com/] is a better place to read about games from scracth. You can easily follow the tutorial with bgfx instead of opengl.
```
Building shaders:
cd Shaders
and call:
make
```
You can read makefiles to understand what is going on, but basically it uses bgfx shader compilation tool [https://bkaradzic.github.io/bgfx/tools.html#shader-compiler-shaderc].

Now you are ready to run Terrasu.exe
# Building for browser (Emscripten)
Okay we change compilers. We cant use Visual studio for this job(not completely true). 
[https://emscripten.org/] "Emscripten is a complete compiler toolchain to WebAssembly, using LLVM, with a special focus on speed, size, and the Web platform."
Installing emscripten should be pretty easy now - just follow commands. But we can use compiler em++ only in the terminal, where we activated emsdk_env.
  First lets build dependencies, we cant use .lib files which were compiled with MSVC or g++.
- bgfx
```
  cd 3rdParty\bgfx
  and run
  ..\bx\tools\bin\windows\genie --gcc=wasm gmake
  okay now we have makefiles in .build\projects\gmake-wasm
  cd .build\projects\gmake-wasm
  but remember to have activated emsdk_env
  and execute "emmake make" not simple "make".
```
-SDL we dont need to build it, its ported to emscripten and easily included with "-S USE-SDL=2"
-yaml-cpp i simply included to Terrasu project.

But now to build project i use GENiE too, just cuse it aleady have scripts to make wasm make files. Our project is in scripts/genie.lua
read it to understand compilers flags, includes and links.
```
  3rdParty\bx\tools\bin\windows\genie --gcc=wasm gmake
  cd .build\projects\gmake-wasm
  emmake make
```
You now have .js .data .html .wasm files. to check if they work run emrun TerrasuDebug.html (Only debug version is tested, you can edit script genie.lua for release vesrion)
btw if src folder is in same directory you can debug c++ code in chrome with F12 [https://developer.chrome.com/blog/wasm-debugging-2020/]
# Building for Android (Android-studio)
I cant make better than this tutorial [https://lazyfoo.net/tutorials/SDL/52_hello_mobile/android_windows/index.php]
though its outdated all concepts are still there.
dependencies:
- bgfx
  Here how to build only for x86_64 but everything is same with armeabi-v7a arm64-v8a x86

    but at first you need to download android_ndk
	[https://developer.android.com/ndk/downloads]
  Then you need to set your $(ANDROID_NDK_ROOT) (read make files to see how android compiler is called) folder to your path
	[https://stackoverflow.com/questions/9546324/adding-a-directory-to-the-path-environment-variable-in-windows]
```
  cd 3rdParty\bgfx
  and run
  ..\bx\tools\bin\windows\genie --gcc=anroid_x86_64 gmake
  okay now we have makefiles in .build\projects\gmake-anroid_x86_64
    cd .build\projects\gmake-anroid_x86_64
  and call make
```
- yaml-cpp i included it in Android.mk with my project read further to understand.
- SDL
To build our project for android we use SDL_activity. Then we include SDL.h in main.cpp we actually redefine main function and asctually use SDL_main. Because of this we can use 3rdParty/SDL/android-project as glue
to our C++ programm. 

Open 3rdParty/SDL/android-project in android studio. Firstly open android studio settings: File - Project structure - Choose your mode in suggestions "app" - now in modules select Build tools version, NDK version(Download it with android studio if needed), source and target compability (at least 1.8).

Now id you look in build.gradle script you should see:
```
externalNativeBuild {
    ndkBuild {
	arguments "APP_PLATFORM=android-30"
	abiFilters 'armeabi-v7a', 'arm64-v8a', 'x86', 'x86_64'
    }
```
You can remove 'armeabi-v7a', 'arm64-v8a', 'x86', if you only build 'x86_64' bgfx lib. But it means apk will install only on x86_64 platform

So we use ndkBuild system other option is Cmake. in 3rdParty\SDL\android-project\app\jni you'll find Android.mk, Application.mk and src/Android.mk. These are makefiles used to build our externalNatvieLibrary (our C++ project and SDL lib).

You can copy *.mk files from main folder android-project and read it to understand what is included. How Android.mk works [https://developer.android.com/ndk/guides/android_mk].

And now we need to copy our files to android-project but it is easier to just create link to folder:
```
in 3rdParty\SDL\android-project\app\jni
mklink /D SDL2 (PASTE_YOUR_PATH_HERE)/3rdParty/SDL
in 3rdParty\SDL\android-project\app\jni\src
mklink /D src (PASTE_YOUR_PATH_HERE)/src  (our c++ files)
mklink /D 3rdParty (PASTE_YOUR_PATH_HERE)/3rdParty  (our c++ includes)
```
But we still need to pack our assets with apk. In android studio create Assets folder: right-click on project - new - Folder - Assets folder.
```
now in 3rdParty\SDL\android-project\app\src\main\assets
mklink /D Assets (PASTE_YOUR_PATH_HERE)/Assets
```
You can build your project and test in emulaltor.

