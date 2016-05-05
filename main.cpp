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

#include "DX11Framework.h"


//////////////////////////////////////////////////////////////////////////////////////
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	srand(int(time(NULL)));

	// Main message loop
	MSG msg = { 0 };
	
	DX11Framework* pDX11Framework = DX11Framework::getInstance();

	if (!pDX11Framework->Initialise(hInstance, nCmdShow)) return 0;
	
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			pDX11Framework->Update();
			pDX11Framework->Render();
		}
	}
	pDX11Framework->ReleaseInstance();

	return (int)msg.wParam;
}
