#pragma once
#include "bgfx/bgfx.h"
namespace Terrasu {
	struct PosUvTileVertex {
		float m_x;
		float m_y;
		float uv_x;
		float uv_y;
		float m_xTile;
		float m_yTile;
		static void init() {
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float, true, true)
				.add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float, false, false)
				.end();
		};
		static bgfx::VertexLayout ms_layout;
	};
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