#pragma once
#include "glm/glm.hpp"
#include "bgfx/bgfx.h"
#include "bx/file.h"
#include "Renderer/Material.h"
#include <memory>
#include <string>
#include "bimg/bimg.h"
#include <unordered_map>
#include "../3rdParty/bgfx/examples/common/cube_atlas.h"
#include "../3rdParty/bgfx/examples/common/font/font_manager.h"
#include "../3rdParty/bgfx/examples/common/font/text_buffer_manager.h"

#include <Renderer/Text.h>

struct NSVGimage;
namespace Terrasu {

	typedef struct {
		const uint8_t* data;
		size_t dataSize;
		size_t currentPosition;
		std::string filename;
	} AudioData;


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
			AudioData* LoadAudioFile(std::string Filepath);
			void HotReload();
			NSVGimage* LoadSvg(std::string name);
			const bgfx::Memory* loadMem(const char* _filePath);
			FontHandle LoadFont(std::string name);
			Text* CreateText(std::string fontname = "Assets/font/Pretendard-Regular.otf");
			TrueTypeHandle loadTtf(const char* _filePath);
		private:

			bgfx::ShaderHandle loadShaderHandle(const char* _name);
			bgfx::TextureHandle loadTextureHandle(const char* _filePath, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation);
			std::unique_ptr<bx::FileReaderI> m_reader;

			std::unordered_map<std::string,std::unique_ptr<Texture>> m_textures;
			std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
			std::unordered_map<std::string, bgfx::UniformHandle> m_uniforms;
			std::unordered_map<std::string, FontHandle> m_fonts;

			std::unique_ptr<FontManager> m_fontManager = std::make_unique<FontManager>(512);
			std::unique_ptr<TextBufferManager> m_tbman = std::make_unique<TextBufferManager>(m_fontManager.get());


	};
}