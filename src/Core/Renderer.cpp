#include "bgfx/bgfx.h"
#include "Renderer/Renderer.h"
namespace Terrasu {

	static PosUvTileVertex quadVertices[] =
	{


		{-1.0f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f },  //0
		{ 1.0f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f },  //1
		{-1.0f, -1.0f, 1.0f,  1.0f, 1.0f, 1.0f },  //2
		{ 1.0f, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f },  //3
	};

	static const uint16_t quadTriList[] =
	{

		2, 3, 1,
		1, 0, 2,

	};
	class Renderer;
	class SimpleQuad {
	public:
		SimpleQuad() {
			PosUvTileVertex::init();


			m_vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(quadVertices, sizeof(quadVertices)),
				PosUvTileVertex::ms_layout);



			m_ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(quadTriList, sizeof(quadTriList))
			);

		}
		~SimpleQuad() {
			bgfx::destroy(m_ibh);
			bgfx::destroy(m_vbh);
		};
	private:
		bgfx::VertexBufferHandle m_vbh;
		bgfx::IndexBufferHandle m_ibh;

		friend Renderer;
	};
}
namespace Terrasu
{
	bgfx::VertexLayout PosUvTileVertex::ms_layout;

	Renderer::Renderer()
	{
		quad = new SimpleQuad();
	}
	Renderer::~Renderer()
	{
	
	}
	void Renderer::DrawQuad(const TransformComponent& transform, const Material& material)
	{
		// Set model matrix for rendering.
		bgfx::setTransform(glm::value_ptr(transform.GetTransform()));

		// Set vertex and index buffer.
		bgfx::setVertexBuffer(0, quad->m_vbh);
		bgfx::setIndexBuffer(quad->m_ibh);

		for (auto texture : material.textures)
		{
			bgfx::setTexture(0, texture.texColor, texture.handle);
		}

		for (auto uniform : material.uniforms)
		{
			bgfx::setUniform(m_assetManager->CreateUniformHandle(uniform.m_name), glm::value_ptr(uniform.data));
		}
		//bgfx::setTexture(0, s_texColor, *TryLoadTexture(spr.textureName));
		//float t = 2.2;
		//glm::vec4 data = { spr.xTilesetSize, spr.yTilesetSize,spr.xTile,spr.yTile };
		//bgfx::setUniform(TileData, glm::value_ptr(data));
		//bgfx::setUniform(u_color, glm::value_ptr(spr.color));
		//bgfx::setUniform(u_fill, glm::value_ptr(glm::vec4{ spr.fill,0,0,0 }));
		uint64_t state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_BLEND_ALPHA
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			;
		// Set render states.
		bgfx::setState(state);
		// Submit primitive for rendering to view 0.
		auto prog = material.shader.handle;
		bgfx::submit(0, prog);
	}

	void Renderer::DrawText(const TransformComponent& transform, const TextUIComponent& text)
	{

		auto& material = text.material;
		// Set model matrix for rendering.
		bgfx::setTransform(glm::value_ptr(transform.GetTransform()));

		// Set vertex and index buffer.
		bgfx::setVertexBuffer(0, text.m_vbh);
		bgfx::setIndexBuffer(text.m_ibh);


		for (auto texture : material.textures)
		{
			bgfx::setTexture(0, texture.texColor, texture.handle);
		}

		for (auto uniform : material.uniforms)
		{
			bgfx::setUniform(m_assetManager->CreateUniformHandle(uniform.m_name), glm::value_ptr(uniform.data));
		}
		//bgfx::setTexture(0, s_texColor, *TryLoadTexture(spr.textureName));
		//float t = 2.2;
		//glm::vec4 data = { spr.xTilesetSize, spr.yTilesetSize,spr.xTile,spr.yTile };
		//bgfx::setUniform(TileData, glm::value_ptr(data));
		//bgfx::setUniform(u_color, glm::value_ptr(spr.color));
		//bgfx::setUniform(u_fill, glm::value_ptr(glm::vec4{ spr.fill,0,0,0 }));
		uint64_t state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_BLEND_ALPHA
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			;
		// Set render states.
		bgfx::setState(state);
		// Submit primitive for rendering to view 0.
		auto prog = material.shader.handle;
		bgfx::submit(0, prog);

	}

	void Renderer::DrawVerts(std::vector<PosUvTileVertex>* vertexArray, const glm::mat4& transform, Material* material)
	{
		if (material == nullptr)
			material = &m_assetManager->CreateMaterial("texturedQuad");
		if (m_vbh.idx > 5000)
		{
			m_vbh = bgfx::createDynamicVertexBuffer(bgfx::makeRef(vertexArray->data(), vertexArray->size() * 6 * sizeof(float)), Terrasu::PosUvTileVertex::ms_layout);
		}

		bgfx::setTransform(glm::value_ptr(transform));
		//solve not overloaded load
		auto texture = m_assetManager->LoadTexture("Assets/mix-and-match-pro.png");
		bgfx::setTexture(0, texture->texColor, texture->handle, BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC);




		bgfx::update(m_vbh, 0, bgfx::makeRef(vertexArray->data(), vertexArray->size() * 6 * sizeof(float)));


		bgfx::setVertexBuffer(0, m_vbh);


		uint64_t state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_BLEND_ALPHA
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
			;
		// Set render states.
		bgfx::setState(state);
		//solve not overloaded load


		bgfx::submit(0, material->shader.handle);
	}

}