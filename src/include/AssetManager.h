#pragma once
#include "bgfx/bgfx.h"
#include "bx/file.h"
#include "Shader.h"
#include "Material.h"
#include <memory>
#include <string>
#include "bimg/bimg.h"
#include <unordered_map>
//Loading Shader, Sprites, pure binary, pure text.
namespace Terrasu {
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