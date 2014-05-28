#include <d3d9.h>
#include <tchar.h>
#include <Windows.h>

#pragma comment (lib, "d3d9.lib")

#define CUSTOM_WINDOW_CLASS _T("growstudy")

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASS wndClass = { 0, };
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = CUSTOM_WINDOW_CLASS;

	if (RegisterClass(&wndClass) == 0)
		return -1;

	HWND hWnd = CreateWindow(CUSTOM_WINDOW_CLASS, _T("Direct3D9 Window"), WS_OVERLAPPEDWINDOW,
		0, 0, 800, 600, NULL, NULL, hInstance, NULL);

	if (hWnd == 0)
		return -1;

	/* ----------------------- Direct3D9 Initialize ----------------------- */
	IDirect3D9 * d3d = Direct3DCreate9(D3D_SDK_VERSION);
	IDirect3DDevice9 * d3dDevice;

	D3DPRESENT_PARAMETERS pp = { 0, };
	pp.BackBufferWidth = 1280;
	pp.BackBufferHeight = 720;
	pp.BackBufferFormat = D3DFMT_X8R8G8B8;
	pp.BackBufferCount = 1;
	pp.hDeviceWindow = hWnd;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	pp.Windowed = false;
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &d3dDevice);
	/* ----------------------- Direct3D9 Initialize ----------------------- */

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0xffff00ff, 1.0f, 0);

			d3dDevice->Present(NULL, NULL, NULL, NULL);
			Sleep(1);
		}
	}

	/* ----------------------- Direct3D9 Destroy ----------------------- */
	d3dDevice->Release();
	d3d->Release();
	/* ----------------------- Direct3D9 Destroy ----------------------- */

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}