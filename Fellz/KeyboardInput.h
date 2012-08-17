#ifndef __KEYBOARD_INPUT_CLASS__
#define __KEYBOARD_INPUT_CLASS__


// this static class will provide basic keyboard input since I cant find this on Cocos2D-x

// only implemented on windows for now
#ifdef _WIN32
#include <Windows.h>

// define the keycodes based on Windows API
#define CC_KEY_UP            VK_UP
#define CC_KEY_DOWN          VK_DOWN
#define CC_KEY_LEFT          VK_LEFT
#define CC_KEY_RIGHT         VK_RIGHT

#endif

class KeyboardInput
{
public:
	// get key sending a keycode defined in KeyboardInput.h
	static bool GetKey(int keycode);
private:
	KeyboardInput(void);
};


#endif // __KEYBOARD_INPUT_CLASS__
