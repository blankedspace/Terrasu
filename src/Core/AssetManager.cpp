#include "AssetManager.h"
#include "bx/bx.h"
#include "bimg/bimg.h"
#include "bimg/decode.h"
#include <fstream>
#include <iostream>
namespace Terrasu {


	AssetManager::AssetManager(){
		m_reader = std::make_unique<bx::FileReader>();
		std::cout << "Created";

	}
	AssetManager::~AssetManager(){
		std::cout << "Destroyed";
		for (const auto& tx : m_textures) {
			bgfx::destroy(tx.second->handle);
			bgfx::destroy(tx.second->texColor);
		}
		for (const auto& sh : m_shaders) {
			bgfx::destroy(sh.second->handle);
		}
	}
	const bgfx::Memory* AssetManager::loadMem(const char* _filePath){
		if (bx::open(m_reader.get(), _filePath)) {
			uint32_t size = (uint32_t)bx::getSize(m_reader.get());
			const bgfx::Memory* mem = bgfx::alloc(size + 1);
			bx::read(m_reader.get(), mem->data, size, bx::ErrorAssert{});
			bx::close(m_reader.get());
			mem->data[mem->size - 1] = '\0';
			return mem;
		}
		return NULL;
	}
	bgfx::ShaderHandle AssetManager::loadShaderHandle(const char* _name){

		char filePath[512];
		const char* shaderPath = "???";

		switch (bgfx::getRendererType())
		{
		case bgfx::RendererType::Noop:
		case bgfx::RendererType::Direct3D9:  shaderPath = "3rdParty/bgfx/examples/runtime/shaders/dx9/";   break;
		case bgfx::RendererType::Direct3D11:
		case bgfx::RendererType::Direct3D12: shaderPath = "Assets/shaders/dx11/";  break;
		case bgfx::RendererType::Agc:
		case bgfx::RendererType::Gnm:        shaderPath = "3rdParty/bgfx/examples/runtime/shaders/pssl/";  break;
		case bgfx::RendererType::Metal:      shaderPath = "3rdParty/bgfx/examples/runtime/shaders/metal/"; break;
		case bgfx::RendererType::Nvn:        shaderPath = "3rdParty/bgfx/examples/runtime/shaders/nvn/";   break;
		case bgfx::RendererType::OpenGL:     shaderPath = "Assets/shaders/opengl/";  break;
		case bgfx::RendererType::OpenGLES:   shaderPath = "Assets/shaders/opengles/";  break;
		case bgfx::RendererType::Vulkan:     shaderPath = "3rdParty/bgfx/examples/runtime/shaders/spirv/"; break;
		case bgfx::RendererType::WebGPU:     shaderPath = "3rdParty/bgfx/examples/runtime/shaders/spirv/"; break;

		case bgfx::RendererType::Count:
			BX_ASSERT(false, "You should not be here!");
			break;
		}

		bx::strCopy(filePath, BX_COUNTOF(filePath), shaderPath);
		bx::strCat(filePath, BX_COUNTOF(filePath), _name);
		bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");
		
		auto memory = loadMem(filePath);
		if(NULL == memory)
			return BGFX_INVALID_HANDLE;
		bgfx::ShaderHandle handle = bgfx::createShader(memory);
		bgfx::setName(handle, _name);

		return handle;
	}
	Shader* AssetManager::LoadShader(std::string name){
		if (m_shaders.find(name) != m_shaders.end()) {
			return m_shaders[name].get();
		}
		std::string  _vsName = "vs_" + name;
		std::string  _fsName = "fs_" + name;
		bgfx::ShaderHandle vsh = loadShaderHandle( _vsName.c_str());
		bgfx::ShaderHandle fsh = loadShaderHandle(_fsName.c_str());

		if (vsh.idx == bgfx::kInvalidHandle)
		{
			return LoadShader("texturedQuad");
		}

		Shader shader;
		shader.handle =  bgfx::createProgram(vsh, fsh, true);
		shader.shaderName = name;
		m_shaders[name] = std::make_unique<Shader>(shader);
		return m_shaders[name].get();
	}
	static void imageReleaseCb(void* _ptr, void* _userData)
	{
		BX_UNUSED(_ptr);
		bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
		bimg::imageFree(imageContainer);
	}
	bgfx::TextureHandle AssetManager::loadTextureHandle(const char* _filePath, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation){

		BX_UNUSED(_skip);
		bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

		uint32_t size;
		const bgfx::Memory* mem = loadMem(_filePath);
		if (mem == NULL)
		{
			return BGFX_INVALID_HANDLE;
		}
		void* data = mem->data;
		size = mem->size;

		bx::AllocatorI* allocator = new bx::DefaultAllocator();
		if (NULL != data)
		{
			bimg::ImageContainer* imageContainer = bimg::imageParse(allocator, data, size);
			if (NULL != imageContainer)
			{
				if (NULL != _orientation)
				{
					*_orientation = imageContainer->m_orientation;
				}

				const bgfx::Memory* mem = bgfx::makeRef(
					imageContainer->m_data
					, imageContainer->m_size
					, imageReleaseCb
					, imageContainer
				);

				if (imageContainer->m_cubeMap)
				{
					handle = bgfx::createTextureCube(
						uint16_t(imageContainer->m_width)
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
						, _flags
						, mem
					);
				}
				else if (1 < imageContainer->m_depth)
				{
					handle = bgfx::createTexture3D(
						uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, uint16_t(imageContainer->m_depth)
						, 1 < imageContainer->m_numMips
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
						, _flags
						, mem
					);
				}
				else if (bgfx::isTextureValid(0, false, imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format), _flags))
				{
					handle = bgfx::createTexture2D(
						uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
						, _flags
						, mem
					);
				}

				if (bgfx::isValid(handle))
				{
					bgfx::setName(handle, _filePath);
				}

				if (NULL != _info)
				{
					bgfx::calcTextureSize(
						*_info
						, uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, uint16_t(imageContainer->m_depth)
						, imageContainer->m_cubeMap
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
					);
				}
			}
		}

		//bgfx::destroy(mem);
		delete mem;
		return handle;
	}

	Texture* AssetManager::LoadTexture(std::string name){
		if(m_textures.find(name) != m_textures.end()){ 
			return m_textures[name].get();
		}
		Texture texture;
		texture.textureName = name;
		texture.handle = loadTextureHandle(name.c_str(), BGFX_SAMPLER_POINT, 0, nullptr, nullptr);

		if (texture.handle.idx == bgfx::kInvalidHandle)
		{
			return LoadTexture("Assets/Error.png");
		}

		texture.texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
		m_textures[name] = std::make_unique<Texture>(texture);
		return m_textures[name].get();
	}
	Material AssetManager::CreateMaterial(std::string shaderName)
	{

		Material mt;
		mt.shader = *LoadShader(shaderName);
		return mt;
	}
	bool AssetManager::WriteFile(std::string Filepath, bgfx::Memory data)
	{
		return false;
	}
	bool AssetManager::WriteFileStr(std::string Filepath, std::string data)
	{
		//TODO:
		std::ofstream fout(Filepath);
		fout << data.c_str();
		return true;
	}

	std::string AssetManager::ReadFileStr(std::string Filepath)
	{
		auto mem = loadMem(Filepath.c_str());
	
		std::string s((char*)mem->data, mem->size - 1);
		delete mem;
		return s;
	}
	void AssetManager::HotReload()
	{
	
	
	}
}
