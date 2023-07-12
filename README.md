# Terrasu
While making a lot of small games with Unity game engine i wondered how 
cross-platform games work? I choose c++ for my small "game engine". Because it's everywhere, in all operating systems and all big game engines.
But i couldnt find simple tutorial on how to make game for win,linux,browsers and mobile with C++. Maybe because C++ is not easy, or 
because some big tech companies want to push their technologies.

This is not a professional git,just a small guide from newbie to newbies.

At first you need to understand how compiler works. There are 
couple of choices:
1. Microsoft Visual C++ compiler
2. The GNU compiler collection
3. Clang/LLVM
e.t.c

To compile single file into a program you usually execute simple command
g++ main.cpp -o main.exe
And i recommend to read about compiler steps:
1. Pre-processing the source code.
  This step involves handling preprocessor directives and macros, which are instructions to modify the source code before compilation.
Many errors can occur at this step, sometimes you need to pass some defines to a compiler for example:
g++ main.cpp -DBX_CONFIG_DEBUG -o main.exe
  And i recommend to read about compiler flags (like -i -l -L -O3)
  And at this step you include headers, btw compiler just copy pastes headers to your file then you #include "Header.h"
3. Compiling the source code
4. Assembling the compiled file
5. Linking the object code file to create an executable file
   At this step a lot of errors can occur too, C++ is big and old language. So its a pretty hard problem to use others code.
Usually you compile them with steps i'll explain later, and link to your project, but what if your code and lib code uses different version of C++ and so on.
We can link staticly with mylib.lib or dinamically with mylib.dll.


To build a program from many files we need to execute a lot of compiler commands
g++ one.cpp -o one.o
g++ two.cpp -o two.o
....
g++ onehundred.cpp -o onehundred.o
g++ *.o -o app.exe
So we need a way to optimize this procces. To our help comes: gnu make, Or full blown IDE like visual studio.
Read how gmake works here - [https://www.gnu.org/software/make/manual/html_node/Simple-Makefile.html].
As you can understand thats not ideal way too. If we want to make cross-platfrom app(and we want), we cant use Visual studio to make webassembly proj and android is wierd too.
And for make we`ll need to write a lot of commands for all platfroms. Ofcourse some one thought about this problem before.I think programming is about abstraction!

There are tools to generate project files for other tools which use compilers.
[https://cmake.org/] Cmake is a tool to generate project files, like makefiles or visual studio project files.
[https://premake.github.io/] But in our project we will use Premake.

Now you have basic understanding of C++ building pipeline, i recommend downloading some open source programs and building it.
Maybe something like Godot(it uses Scons, which i didnt mention) [https://docs.godotengine.org/en/stable/contributing/development/compiling/index.html].
But if you continue reading this guide, we will build a lot of dependencies.
# Building for windows (Visual Studio)
To compile Terrasu for windows run:
3rdParty\Premake\premak5.exe vs2019
All instructions for premake tool is located in premake5.lua file, you can read it to understand dependencies and defines and other compiler flags.
Now we have Terrasu.sln and can just open it in visual studio, but at first lets build our dependencies.
- SDL2[https://wiki.libsdl.org/SDL2/Installation] "Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware."
In simpler words - SDL creates window and gets mouse and keyboard input(or touches if we use mobile)
Simplest way to build is to use vs project files in 3rdParty\SDL\VisualC\SDL.sln
But ofcourse you can try use Cmake.
- bgfx[https://bkaradzic.github.io/bgfx/build.html] "Cross-platform, graphics API agnostic, "Bring Your Own Engine/Framework" style rendering library." In simpler words there are a lot of ways to access your GPU
OpenGl, DirectX, Vulkan... and what to choose depends on build target(DirectX is Windows exclusive for example). So we can just call bgfx functions to build with different renderer API.
'''
  Bgfx uses self-made tool GENiE - but actually its a fork of premake4 with some changes.
  cd 3rdParty\bgfx
  and run
  ..\bx\tools\bin\windows\genie --with-examples vs2017 (check out examples a lot of interesting info)
'''
  and again build it with Visual studio
- yaml-cpp [https://github.com/jbeder/yaml-cpp] "yaml-cpp is a YAML parser and emitter in C++ matching the YAML 1.2 spec." I use it to save Scene files in my game. You can check Assets\Scene.scn to see how YAML looks
	yaml-cpp uses cmake to generate vs project files

After building all dependencies check that everything is in right path(Debug and Release) in premake5.lua.
And now we are ready to build Terrasu executable!

But to run the programm we need Assets folder and SLD2.dll(it was build with static lib) in the same directory as our
executable, if we debug with visual studio its directory where .sln file is located.

Oh and we need to compile shaders, this is just bgfx thing, because of support for different graphics API, which use different shader languages
Again i recommend reading about shaders... and honestly [https://learnopengl.com/] is a better place to read about games from scracth. You can easily follow the tutorial with bgfx instead of opengl.
Building shaders:
cd Shaders
and call:
make
You can read makefiles to understand what is going on, but basically it uses bgfx shader compilation tool [https://bkaradzic.github.io/bgfx/tools.html#shader-compiler-shaderc].

Now you are ready to run Terrasu.exe)
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
'''
  3rdParty\bx\tools\bin\windows\genie --gcc=wasm gmake
  cd .build\projects\gmake-wasm
  emmake mak
'''
You now have .js .data .html .wasm files. to check if they work run emrun TerrasuDebug.html (Only debug version is tested, you can edit script genie.lua for release vesrion)
btw if src folder is in same directory you can debug c++ code in chrome with F12 [https://developer.chrome.com/blog/wasm-debugging-2020/]
# Building for Android (Android-studio)
I cant make better than this tutorial [https://lazyfoo.net/tutorials/SDL/52_hello_mobile/android_windows/index.php]
though its outdated all concepts are still there.
