#ifndef _KEYBOARD_INPUT_MANAGER_H_
#define _KEYBOARD_INPUT_MANAGER_H_

class KeyboardInputManager
{
private:
public:
	KeyboardInputManager();
	~KeyboardInputManager();

	void pollEvents();
};

#endif