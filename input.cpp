#include "input.h"


Input::Input(HINSTANCE hInst, HWND hWnd)
{
	init(hInst, hWnd);
}

Input::~Input()
{
	if (m_keyboardDevice)
	{
		m_keyboardDevice->Unacquire();
		m_keyboardDevice->Release();
	}

	if (m_mouseDevice)
	{
		m_mouseDevice->Unacquire();
		m_mouseDevice->Release();
	}

	if (m_directInput) m_directInput->Release();
}

HRESULT Input::init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;
	ZeroMemory(m_keyboardKeysState, sizeof(m_keyboardKeysState));

	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(hr))
		return hr;

	hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboardDevice, NULL);
	if (FAILED(hr))
		return hr;

	hr = m_keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return hr;

	hr = m_keyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return hr;

	hr = m_keyboardDevice->Acquire();
	if (FAILED(hr))
		return hr;

	hr = m_directInput->CreateDevice(GUID_SysMouse, &m_mouseDevice, NULL);
	if (FAILED(hr))
		return hr;

	hr = m_mouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		return hr;

	hr = m_mouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
		return hr;

	hr = m_mouseDevice->Acquire();
	if (FAILED(hr))
		return hr;

	return S_OK;
}

void Input::ReadInputStates()
{
	HRESULT hr;

	hr = m_keyboardDevice->GetDeviceState(sizeof(m_keyboardKeysState), (LPVOID)&m_keyboardKeysState);

	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_keyboardDevice->Acquire();
		}
	}

	hr = m_mouseDevice->GetDeviceState(sizeof(m_mouseState), (LPVOID)&m_mouseState);

	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_mouseDevice->Acquire();
		}
	}
}


bool Input::isKeyPressed(unsigned char DI_keycode)
{
	return (m_keyboardKeysState[DI_keycode] & 0x80) != 0;// ? false : true;
}

MouseData Input::getMouseData()
{
	MouseData md;
	md.x = m_mouseState.lX;
	md.y = m_mouseState.lY;
	md.scrollWheel = m_mouseState.lZ;

	md.leftClick = (m_mouseState.rgbButtons[0] & 0x80) != 0;
	md.rightClick = (m_mouseState.rgbButtons[1] & 0x80)!=0;
	md.centerClick = (m_mouseState.rgbButtons[2] & 0x80)!=0;
	md.other = (m_mouseState.rgbButtons[3] & 0x80)!=0;
	return md;
}