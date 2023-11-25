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
		enum State {
			none,
			leftPressed,
			rightPressed,
		} state;
	};


	typedef SDL_KeyCode Keys;

	class Input{
	public:
		static bool IsPressed(Keys key) {
			if (m_keyPressed.find(key) == m_keyPressed.end()){
				return false;
			}
			else return m_keyPressed[key];
		}
		static void SetKeyState(Keys key, bool isPressed) {
			m_keyPressed[key] = isPressed;
		}
		inline static mouse Mouse;
	private:
		inline static std::unordered_map<Keys,bool> m_keyPressed;
	};
	/*
	update
	if(input::ispressed(Key.w))
	if(input::keyboard.w)
	*/
}