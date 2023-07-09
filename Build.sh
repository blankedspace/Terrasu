GREEN='\033[1;32m'
cmake -B 3rdParty/yaml-cpp/build -S 3rdParty/yaml-cpp
cd 3rdParty/bgfx
../bx/tools/bin/windows/genie --with-examples vs2017
cd ../..
3rdParty/Premake/premake5.exe vs2019
cd Shaders
make

echo -e "\n${GREEN}Run bgfx.sln in 3rdParty/bgfx/.build/projects to build bgfx.lib"
echo -e "\n${GREEN}Run YAML_CPP.sln in 3rdParty/yaml-cpp/build to build yaml_cpp.lib"
echo -e "\n${GREEN}Run SDL.sln in 3rdParty/SDL/VisualC to build SDL.lib"