#include "inputManager.h"

InputManager::InputManager(HINSTANCE hInst, HWND hWnd)
{
	m_hInst = hInst;
	m_hWnd = hWnd;

	Init();
}

InputManager::~InputManager(void)
{
	if (m_pKeyboardDevice)
	{
		m_pKeyboardDevice->Unacquire();
		m_pKeyboardDevice->Release();
	}

	if (m_pMouseDevice)
	{
		m_pMouseDevice->Unacquire();
		m_pMouseDevice->Release();
	}

	if (m_pDirectInput) m_pDirectInput->Release();
}

HRESULT InputManager::Init(void)
{
	HRESULT hr;
	ZeroMemory(m_keyboardKeysState, sizeof(m_keyboardKeysState));

	hr = DirectInput8Create(m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	if (FAILED(hr))
		return hr;

	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL);
	if (FAILED(hr))
		return hr;

	hr = m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return hr;

	hr = m_pKeyboardDevice->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return hr;

	hr = m_pKeyboardDevice->Acquire();
	if (FAILED(hr))
		return hr;

	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL);
	if (FAILED(hr))
		return hr;

	hr = m_pMouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		return hr;

	hr = m_pMouseDevice->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
		return hr;

	hr = m_pMouseDevice->Acquire();
	if (FAILED(hr))
		return hr;

	return S_OK;
}

void InputManager::ReadInputStates(void)
{
	HRESULT hr;

	memcpy(m_prevKeyboardKeysState, m_keyboardKeysState, sizeof(m_keyboardKeysState));

	hr = m_pKeyboardDevice->GetDeviceState(sizeof(m_keyboardKeysState), (LPVOID)&m_keyboardKeysState);

	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pKeyboardDevice->Acquire();
		}
	}

	//legacy code
	//m_prevMouseState = m_mouseState;
	hr = m_pMouseDevice->GetDeviceState(sizeof(m_mouseState), (LPVOID)&m_mouseState);

	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pMouseDevice->Acquire();
		}
	}
}

bool InputManager::IsKeyPressed(UCHAR DI_keycode)
{
	return (m_keyboardKeysState[DI_keycode] & 0x80) != 0;
}

bool InputManager::IsKeyPressedOnce(UCHAR DI_keycode)
{
	return (((m_keyboardKeysState[DI_keycode] & 0x80)) && (!(m_prevKeyboardKeysState[DI_keycode] & 0x80) !=0)) ? true : false;
}

void InputManager::KeyAlreadyDown(UCHAR DI_keycode)
{
	UNREFERENCED_PARAMETER(DI_keycode);
}

float InputManager::GetMouseDeltaX(float sensitivity)
{
	UNREFERENCED_PARAMETER(sensitivity);
	return 0;
}

float InputManager::GetMouseDeltaY(float sensitivity)
{
	UNREFERENCED_PARAMETER(sensitivity);
	return 0;
}

bool InputManager::CheckControllerEnabled(void)
{
	return 0;
}

Thumb InputManager::getControllerLeftThumb(void)
{
	Thumb r;
	r.x = 0.0f;
	return r;
}

Thumb InputManager::getControllerRightThumb(void)
{
	Thumb r;
	r.x = 0.0f;
	return r;
}

bool InputManager::IsControllerButtonPressed(SHORT button)
{
	UNREFERENCED_PARAMETER(button);
	return 0;
}

float InputManager::GetControlllerTrigger(UCHAR trigger)
{
	UNREFERENCED_PARAMETER(trigger);
	return 0;
}

void InputManager::VibrateController(WORD left, WORD right)
{
	UNREFERENCED_PARAMETER(left);
	UNREFERENCED_PARAMETER(right);
}

Thumb InputManager::ReadControllerThumb(SHORT thumbX, SHORT thumbY, int deadzoneValue)
{
	UNREFERENCED_PARAMETER(thumbX);
	UNREFERENCED_PARAMETER(thumbY);
	UNREFERENCED_PARAMETER(deadzoneValue);
	Thumb r;
	r.x = 0.0f;
	return r;
}

float InputManager::ReadControllerTrigger(BYTE trigger, int deadzoneValue)
{
	UNREFERENCED_PARAMETER(trigger);
	UNREFERENCED_PARAMETER(deadzoneValue);
	return 0;
}

HRESULT InputManager::InitDevice(const GUID guidSystem, IDirectInput8** p_device, const DIDATAFORMAT dataFormat, DWORD deviceFlag)
{
	UNREFERENCED_PARAMETER(guidSystem);
	UNREFERENCED_PARAMETER(p_device);
	UNREFERENCED_PARAMETER(dataFormat);
	UNREFERENCED_PARAMETER(deviceFlag);
	return 0;
}

MouseData InputManager::getMouseData()
{
	MouseData md;
	md.x = m_mouseState.lX;
	md.y = m_mouseState.lY;
	md.scrollWheel = m_mouseState.lZ;

	md.leftClick = (m_mouseState.rgbButtons[0] & 0x80) != 0;
	md.rightClick = (m_mouseState.rgbButtons[1] & 0x80) != 0;
	md.centerClick = (m_mouseState.rgbButtons[2] & 0x80) != 0;
	md.other = (m_mouseState.rgbButtons[3] & 0x80) != 0;
	return md;
}