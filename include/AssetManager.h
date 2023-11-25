#pragma once


#include <memory>
#include <string>
#include "bx/file.h"
#include "bimg/bimg.h"
#include <unordered_map>
//Loading Shader, Sprites, pure binary, pure text.
namespace bgfx {
	struct Memory;
	struct UniformHandle;
	struct ShaderHandle;
	struct TextureHandle;
	struct TextureInfo;
}
namespace Terrasu {
	class Shader;
	class Texture;
	class Material;
	
	class AssetManager {
	public:
		AssetManager();
		~AssetManager();
		Shader* LoadShader(std::string name);
		Texture* LoadTexture(std::string name);
		Material CreateMaterial(std::string shaderName);
		bool WriteFile(std::string Filepath,bgfx::Memory data);
		bool WriteFileStr(std::string Filepath, std::string data);
		std::string ReadFileStr(std::string Filepath);
		bgfx::UniformHandle CreateUniformHandle(std::string name);

		void HotReload();
	private:
		const bgfx::Memory* loadMem(const char* _filePath);
		bgfx::ShaderHandle loadShaderHandle(const char* _name);
		bgfx::TextureHandle loadTextureHandle(const char* _filePath, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation);
		std::unique_ptr<bx::FileReaderI> m_reader;

		std::unordered_map<std::string,std::unique_ptr<Texture>> m_textures;
		std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
		std::unordered_map<std::string, bgfx::UniformHandle> m_uniforms;


};
}