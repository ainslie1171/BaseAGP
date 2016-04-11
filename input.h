#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <dinput.h>
#include "virtualKeyCodes.h"
#include "directInputKeyCodes.h"

struct MouseData
{
	LONG x,
	y,
	scrollWheel;
	bool leftClick,
		rightClick,
		centerClick,
		other;
};

class Input
{
public:
	Input(HINSTANCE hInst, HWND hWnd);
	~Input();
	void ReadInputStates();
	bool isKeyPressed(unsigned char DI_keycode);
	MouseData getMouseData();
	MouseData getPrevMouseData();

private:
	HRESULT init(HINSTANCE hInst, HWND hWnd);

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboardDevice;
	IDirectInputDevice8* m_mouseDevice;
	unsigned char m_keyboardKeysState[256];
	DIMOUSESTATE m_mouseState;
	DIMOUSESTATE m_prevMouseState;
};

#endif