#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <dinput.h>

#include "inputStructs.h"
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


class InputManager
{
private:
	HINSTANCE m_hInst;
	HWND m_hWnd;
	IDirectInput8* m_pDirectInput;
	IDirectInputDevice8* m_pKeyboardDevice;
	IDirectInputDevice8* m_pMouseDevice;
	DIMOUSESTATE m_mouseState;
	DIMOUSESTATE m_prevMouseState;
	UCHAR m_keyboardKeysState[256];
	UCHAR m_prevKeyboardKeysState[256];
	tagPOINT m_mousePos;
	Controller m_controller;

	Thumb ReadControllerThumb(SHORT thumbX, SHORT thumbY, int deadzoneValue);
	float ReadControllerTrigger(BYTE trigger, int deadzoneValue);
	HRESULT InitDevice(const GUID guidSystem, IDirectInput8** p_device, const DIDATAFORMAT dataFormat, DWORD deviceFlag);
public:
	InputManager(HINSTANCE hInst, HWND hWnd);
	~InputManager(void);
	HRESULT Init(void);
	void ReadInputStates(void);
	bool IsKeyPressed(UCHAR DI_keycode);
	bool IsKeyPressedOnce(UCHAR DI_keycode);
	void KeyAlreadyDown(UCHAR DI_keycode);
	float GetMouseDeltaX(float sensitivity);
	float GetMouseDeltaY(float sensitivity);
	bool CheckControllerEnabled(void);
	Thumb getControllerLeftThumb(void);
	Thumb getControllerRightThumb(void);
	bool IsControllerButtonPressed(SHORT button);
	float GetControlllerTrigger(UCHAR trigger);
	void VibrateController(WORD left, WORD right);
	MouseData getMouseData();
	MouseData getPrevMouseData();
};

#endif//_INPUTMANAGER_H_

/*
Change Log:

*/