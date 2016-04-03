#define _CRT_SECURE_NO_WARNINGS
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT


//////////////////////////////////////////////////////////////////////////////////////
//	Headers
//////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include <d3dx11.h>
#include <dxerr.h>

#include "input.h"
#include "game.h"


//////////////////////////////////////////////////////////////////////////////////////
//	Global Variables
//////////////////////////////////////////////////////////////////////////////////////
LARGE_INTEGER StartingTime, EndingTime, DeltaTime, Frequency;

//////////////////////////////////////////////////////////////////////////////////////
//	Forward declarations
//////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ShutdownD3D();
void UpdateDeltaTime();

//////////////////////////////////////////////////////////////////////////////////////
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Game* game = new Game(hInstance);

	if (FAILED(game->InitialiseWindow(WndProc, nCmdShow)))
	{
		DXTRACE_MSG("Failed to create Window");
		return 0;
	}

	if (FAILED(game->InitialiseD3D()))
	{
		DXTRACE_MSG("Failed to create Device");
		return 0;
	}

	if (FAILED(game->InitialiseGraphics()))
	{
		DXTRACE_MSG("Failed to initialise graphics");
		return 0;
	}

	Input* input = new Input(hInstance, game->getHWND());
	game->setInput(input);

	StartingTime.QuadPart = 0;
	Frequency.QuadPart = 1;
	srand(int(time(NULL)));

	// Main message loop
	MSG msg = { 0 };
	//clock_t beginTime = 0;

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// do something
			input->ReadInputStates();
			game->Update(DeltaTime.QuadPart/1000000.0f);
			game->Render(DeltaTime.QuadPart / 1000000.0f);
			UpdateDeltaTime();
		}
	}

	delete game;
	delete input;

	return (int)msg.wParam;
}


//////////////////////////////////////////////////////////////////////////////////////
// Called every time the application receives a message
//////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//int x, y;
	

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void UpdateDeltaTime()
{
	QueryPerformanceCounter(&EndingTime);
	DeltaTime.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	DeltaTime.QuadPart *= 1000000;
	DeltaTime.QuadPart /= Frequency.QuadPart;
	//Delta time is done :)
	//char mybuff1[100];
	//sprintf(mybuff1, "End: %I64u\nStart: %I64u\nDelta Time: %I64u\n\n", StartingTime.QuadPart, EndingTime.QuadPart, DeltaTime.QuadPart);
	//OutputDebugString(mybuff1);
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);
}