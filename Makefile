VCpath := C:\Program^ Files^ ^(x86)\Microsoft^ Visual^ Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat
EMSDKpath := ..\emsdk\emsdk_env.bat
DIR := C:\Projects\Terrasu
GRADLE := C:\Users\Andrew\.gradle\wrapper\dists\gradle-7.3-bin\ddwl0k7mt9g6ak16i1m905vyv\gradle-7.3\bin\gradle

android:
	cd ${DIR}\3rdParty\SDL\android-project && ${GRADLE} assembleDebug
	C:\Users\Andrew\AppData\Local\Android\Sdk\emulator\emulator -avd Pixel_2_API_31
dep:
	IF exist ${DIR}\3rdParty\SDL\android-project\app\jni\SDL2 (echo exist) ELSE  (cd 3rdParty\SDL\android-project\app\jni && mklink /D SDL2 ${DIR}\3rdParty\SDL)
	IF exist ${DIR}\src (echo exist) ELSE  (cd 3rdParty\SDL\android-project\app\jni\src && mklink /D src ${DIR}\src)
	IF exist ${DIR}\3rdParty\SDL\android-project\app\jni\src\3rdParty (echo exist) ELSE (cd 3rdParty\SDL\android-project\app\jni\src && mklink /D 3rdParty ${DIR}\3rdParty)
	IF exist ${DIR}\3rdParty\SDL\android-project\app\src\main\assets\Assets (echo exist) ELSE (cd 3rdParty\SDL\android-project\app\src\main\assets && mklink /D Assets ${DIR}\Assets)
	XCOPY /E ${DIR}\android-project ${DIR}\3rdParty\SDL\android-project
dependencies:
	cd 3rdParty\bgfx && ..\bx\tools\bin\windows\genie --with-examples vs2019
	cmd /c """${VCpath}"" && msbuild 3rdParty\bgfx\.build\projects\vs2019\bgfx.sln"
	cmd /c """${VCpath}"" && msbuild 3rdParty\SDL\VisualC\SDL.sln"
	IF exist 3rdParty\yaml-cpp\build ( echo build exists ) ELSE ( mkdir  3rdParty\yaml-cpp\build && echo build created)
	cd 3rdParty\yaml-cpp\build && cmake ..
	cmd /c """${VCpath}"" && msbuild 3rdParty\yaml-cpp\build\YAML_CPP.sln"
	cd 3rdParty\bgfx && ..\bx\tools\bin\windows\genie --gcc=wasm gmake

	IF exist ${DIR}\3rdParty\SDL\build (echo exist) ELSE (cd 3rdParty\SDL && mkdir build)
	cmd /c """${EMSDKpath}"" && cd 3rdParty\SDL\build && emcmake cmake .. && emmake make -j4"

	cmd /c """${EMSDKpath}"" && cd 3rdParty\bgfx\.build\projects\gmake-wasm && emmake make CFLAGS=-pthread config=debug32 all"
	cd 3rdParty\bgfx && ..\bx\tools\bin\windows\genie --gcc=android-x86_64 gmake
	cd 3rdParty\bgfx\.build\projects\gmake-android-x86_64 && make
	cd 3rdParty\bgfx && ..\bx\tools\bin\windows\genie --gcc=android-arm64 gmake
	cd 3rdParty\bgfx\.build\projects\gmake-android-arm64 && make config=debug64
emscripten:
	3rdParty\bx\tools\bin\windows\genie --gcc=wasm gmake
	cmd /c """${EMSDKpath}"" && cd .build\projects\gmake-wasm && emmake make"
	cmd /c """${EMSDKpath}"" && cd .build\wasm\bin && emrun TerrasuDebug.html"
windows:
	3rdParty\bx\tools\bin\windows\genie vs2019
	cmd /c """${VCpath}"" && msbuild .build\projects\vs2019\Terrasu.sln"
	IF exist ${DIR}\.build\win64_vs2019\bin\SDL2.dll (echo exist) ELSE (XCOPY ${DIR}\3rdParty\SDL\VisualC\x64\Debug\SDL2.dll ${DIR}\.build\win64_vs2019\bin)
	C:\Projects\Terrasu\.build\win64_vs2019\bin\TerrasuDebug.exe
all:
	cmd /c start "" make android
	cmd /c start "" make emscripten
	cmd /c start "" make windows
