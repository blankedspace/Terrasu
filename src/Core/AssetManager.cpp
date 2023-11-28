#include "AssetManager.h"
#include "bgfx/bgfx.h"

#include "bx/bx.h"
#include "bimg/bimg.h"
#include "bimg/decode.h"
#include <fstream>
#include <iostream>
#include <SDL.h>
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#if BX_PLATFORM_ANDROID
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#endif

#define NANOSVG_IMPLEMENTATION
#include "nanosvg/nanosvg.h"

namespace Terrasu {

#if BX_PLATFORM_ANDROID
	class FileReaderAndroid : public bx::FileReaderI
	{
	public:
		FileReaderAndroid(AAssetManager* _assetManager, AAsset* _file)
			: m_assetManager(_assetManager)
			, m_file(_file)
			, m_open(false)
		{
		}

		virtual ~FileReaderAndroid()
		{
			close();
		}

		virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
		{
			BX_ASSERT(NULL != _err, "Reader/Writer interface calling functions must handle errors.");

			if (NULL != m_file)
			{
				BX_ERROR_SET(_err, bx::kErrorReaderWriterAlreadyOpen, "FileReader: File is already open.");
				return false;
			}


			m_file = AAssetManager_open(m_assetManager, _filePath.getCPtr(), AASSET_MODE_RANDOM);

			if (NULL == m_file)
			{
				BX_ERROR_SET(_err, bx::kErrorReaderWriterOpen, "FileReader: Failed to open file.");
				return false;
			}

			m_open = true;
			return true;
		}

		virtual void close() override
		{
			if (m_open
				&& NULL != m_file)
			{
				AAsset_close(m_file);
				m_file = NULL;
			}
		}

		virtual int64_t seek(int64_t _offset, bx::Whence::Enum _whence) override
		{
			BX_ASSERT(NULL != m_file, "Reader/Writer file is not open.");
			return AAsset_seek64(m_file, _offset, _whence);

		}

		virtual int32_t read(void* _data, int32_t _size, bx::Error* _err) override
		{
			BX_ASSERT(NULL != m_file, "Reader/Writer file is not open.");
			BX_ASSERT(NULL != _err, "Reader/Writer interface calling functions must handle errors.");

			int32_t size = (int32_t)AAsset_read(m_file, _data, _size);
			if (size != _size)
			{
				if (0 == AAsset_getRemainingLength(m_file))
				{
					BX_ERROR_SET(_err, bx::kErrorReaderWriterEof, "FileReader: EOF.");
				}

				return size >= 0 ? size : 0;
			}

			return size;
		}

	private:
		AAssetManager* m_assetManager;
		AAsset* m_file;
		bool m_open;
	};
#endif
	AssetManager::AssetManager(){
#if BX_PLATFORM_ANDROID

		SDL_AndroidGetActivity();
		JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
		jobject activity = (jobject)SDL_AndroidGetActivity();
		jclass clazz(env->GetObjectClass(activity));
		jmethodID methodID = env->GetMethodID(clazz, "getAssets", "()Landroid/content/res/AssetManager;");
		jobject assetManager = env->CallObjectMethod(activity, methodID);
		AAssetManager* manager = AAssetManager_fromJava(env, assetManager);

		m_reader = std::make_unique<FileReaderAndroid>(manager,nullptr);
#else
		m_reader = std::make_unique<bx::FileReader>();
#endif

	}
	AssetManager::~AssetManager(){
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

		std::string filePath;
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
		case bgfx::RendererType::OpenGL:     shaderPath = "Assets/shaders/glsl/";  break;
		case bgfx::RendererType::OpenGLES:   shaderPath = "Assets/shaders/glsl/";  break;
		case bgfx::RendererType::Vulkan:     shaderPath = "3rdParty/bgfx/examples/runtime/shaders/spirv/"; break;
		case bgfx::RendererType::WebGPU:     shaderPath = "3rdParty/bgfx/examples/runtime/shaders/spirv/"; break;

		case bgfx::RendererType::Count:
			BX_ASSERT(false, "You should not be here!");
			break;
		}

		filePath = shaderPath + std::string(_name) + ".bin";

		auto memory = loadMem(filePath.c_str());
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
			SDL_Log("%s",(_fsName + " Not Found").c_str());
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
		if (mem == NULL){
			return "";
		}
		std::string s((char*)mem->data, mem->size - 1);
		delete mem;
		return s;
	}

	Terrasu::AudioData* AssetManager::LoadAudioFile(std::string Filepath)
	{
		AudioData* data = new AudioData();
		auto mem = loadMem(Filepath.c_str());
		data->data = mem->data;
		data->dataSize = mem->size;
		data->currentPosition = 0;
		return data;
	}
	bgfx::UniformHandle AssetManager::CreateUniformHandle(std::string name)
	{
		if (m_uniforms.find(name) != m_uniforms.end()) {
			return m_uniforms[name];
		}

		m_uniforms[name] = bgfx::createUniform(name.c_str(),bgfx::UniformType::Vec4);
		return m_uniforms[name];
	}
	void AssetManager::HotReload()
	{
	
	
	}

	NSVGimage* AssetManager::LoadSvg(std::string name)
	{
		struct NSVGimage* image;
		image = nsvgParse(&ReadFileStr(name)[0], "px", 96);
		return image;
	}
}
