#include "Renderer\Text.h"
#include <cstdint>
#include <unordered_map>


#include "AssetManager.h"
#include <iostream>


namespace Terrasu {



	Text::Text()
	{
		
	}

	Text::~Text()
	{

		std::cout << m_staticText.idx << std::endl;
		tbman->destroyTextBuffer(m_staticText);

	}

	void Text::OnStart()
	{



	}
	int floatRGBAToIntColor(float r, float g, float b, float a = 1.0) {
		// Convert float RGBA values to integers
		int r_int = static_cast<int>(r * 255);
		int g_int = static_cast<int>(g * 255);
		int b_int = static_cast<int>(b * 255);
		int a_int = static_cast<int>(a * 255);

		// Pack the color components into a single integer
		uint32_t color_int = (r_int << 24) | (g_int  << 16) | (b_int << 8) | a_int;

		return color_int;
	}

	void Text::OnTextChange(std::wstring text, NVGcolor color)
	{
	
		auto coloru = floatRGBAToIntColor(color.r,color.g,color.b);

		m_text = text;
		tbman->destroyTextBuffer(m_staticText);
		m_staticText = tbman->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Static);

		tbman->setPenPosition(m_staticText, -32, -32);
		tbman->setTextColor(m_staticText, coloru);
		tbman->appendText(m_staticText, m_fonts, m_text.c_str());
	}

	void Text::Render()
	{

		try {
			// Submit the static text.
			if (m_staticText.idx <= 64)
				tbman->submitTextBuffer(m_staticText, 0);
		}
		catch(const std::exception& e) {
			// Code that executes when an exception of type
			// networkIOException is thrown in the try block
			// ...
			// Log error message in the exception object
			std::cout << e.what();
		}


	}
	void Text::ChangeFont(std::string path)
	{


		
		m_fonts = m_assetmanager->LoadFont(path);
		
		tbman->destroyTextBuffer(m_staticText);
		m_staticText = tbman->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Static);
		tbman->setPenPosition(m_staticText, -32, -32);
		tbman->setTextColor(m_staticText, -16777216);
		tbman->appendText(m_staticText, m_fonts, L"YASOBI\n");
		
		//m_text = L"YASOBI\n";

	}
}
