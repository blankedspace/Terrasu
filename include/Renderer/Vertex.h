#pragma once
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
}