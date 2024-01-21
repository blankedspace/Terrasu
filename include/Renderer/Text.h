#pragma once

#include "../3rdParty/bgfx/examples/common/font/font_manager.h"
#include "../3rdParty/bgfx/examples/common/font/text_buffer_manager.h"
#include <string>
#include <nanovg/nanovg.h>

namespace Terrasu {
	class AssetManager;
	class Text {
	public:
		Text();
		~Text();

		void OnStart();

		void OnTextChange(std::wstring text, NVGcolor color);
		void Render();
		void ChangeFont(std::string path);

		AssetManager* m_assetmanager;
		TextBufferManager* tbman;
		TextBufferHandle m_staticText;
		FontHandle m_fonts;

		std::wstring m_text;
	};
}