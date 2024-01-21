#pragma once
#include "SDL.h"
#include <unordered_map>
namespace Terrasu {
		//key Ranges presses
		//mouse(touch) pos
		//mouse(touch) click and drag
		//
	struct mouse{
		int x = 0, y = 0;
		bool leftPressed = false;
		bool rightPressed = false;
		bool leftClicked = false;
	};


	typedef SDL_KeyCode Keys;

	class Input{
	public:
		static bool IsClicked(Keys key) {
			if (m_keyClicked.find(key) == m_keyClicked.end()) {
				return false;
			}
			else return m_keyClicked[key];
		}
		static bool IsPressed(Keys key) {
			if (m_keyPressed.find(key) == m_keyPressed.end()){
				return false;
			}
			else return m_keyPressed[key];
		}
		static void SetKeyState(Keys key, bool isPressed) {
			m_keyPressed[key] = isPressed;
			m_keyClicked[key] = isPressed;
		}
		static void ClickClear() {
			m_keyClicked.clear();
		}
		inline static mouse Mouse;
		inline static std::wstring lastadded;
	private:
		inline static std::unordered_map<Keys,bool> m_keyPressed;
		inline static std::unordered_map<Keys, bool> m_keyClicked;
	};
	/*
	update
	if(input::ispressed(Key.w))
	if(input::keyboard.w)
	*/
}