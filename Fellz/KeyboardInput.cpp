#include "KeyboardInput.h"


KeyboardInput::KeyboardInput(void)
{
	
}

bool KeyboardInput::GetKey(int keycode)
{

//only implemented to windows right now
#ifdef _WIN32
	return (GetKeyState(keycode) & 0x8000);
#else
	return false;
#endif
}


