#pragma once

#include <Windows.h>

#pragma region Graphics Library Definitions
/////////////////// DO NOT TOUCH THIS ///////////////////
#define OPENGL2			(1 << 2)
#define OPENGL1			OPENGL2
#define OPENGL3			(1 << 3)
#define OPENGL4			(1 << 4)

#define DIRECT3DMODE9	(1 << 9)
#define DIRECT3DMODE11	(1 << 11)
/////////////////// DO NOT TOUCH THIS ///////////////////
#pragma endregion


// 3D 프레임워크 모드
// - DIRECT3DMODE9	: Direct3D9
// - DIRECT3DMODE11	: Direct3D11
// - OPENGL2		: OpenGL 2.0
// - OPENGL3		: OpenGL 3.0
// - OPENGL4		: OpenGL 4.0
#define USED_3DFRAMEWORKMODE		DIRECT3DMODE9
// XInput 사용 여부(0:비사용, 1:사용)
#ifndef USED_XINPUTMODE
#define USED_XINPUTMODE				0
#endif
// Direct3D 확장 라이브러리 사용 여부(0:비사용, 1:사용)
#ifndef USED_EXTENSION
#define USED_EXTENSION				0
#endif
// Resize 이벤트 사용 여부(0:비사용, 1:사용)
#define USING_RESIZE				0


#pragma region Graphics Library Includes
/////////////////// DO NOT TOUCH THIS ///////////////////
#define CHECK_3DMODE(x) (USED_3DFRAMEWORKMODE == x)
#define SAFE_RELEASE(x) if(x) (x)->Release()
/////////////////// DO NOT TOUCH THIS ///////////////////

#if(CHECK_3DMODE(DIRECT3DMODE9))
///////////////// DIRECT3D9 DEFINITIONS /////////////////
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")
#if(USED_EXTENSION != 0)
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")

bool LoadTexture(IDirect3DDevice9 * device, IDirect3DTexture9 ** texture, LPCTSTR filename, int * width, int * height)
{
	D3DXIMAGE_INFO info;
	if(FAILED(D3DXGetImageInfoFromFile(filename, &info))) return false;

	if(FAILED(D3DXCreateTextureFromFileEx(device, filename, info.Width, info.Height, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		-1, -1, D3DCOLOR_XRGB(255, 0, 255), &info, nullptr, texture)))
		return false;

	if(width != nullptr) *width = info.Width;
	if(height != nullptr) *height = info.Height;

	return true;
}
#endif
struct DEVICEINSTANCE
{
	IDirect3D9 * d3d;
	IDirect3DDevice9 * d3dDevice;
};
///////////////// DIRECT3D9 DEFINITIONS /////////////////
#elif(CHECK_3DMODE(DIRECT3DMODE11))
//////////////// DIRECT3D 11 DEFINITIONS ////////////////
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include <dxgi.h>
#pragma comment (lib, "dxgi.lib")
#if(USED_EXTENSION != 0)
#include <d3dx11.h>
#pragma comment (lib, "d3dx11.lib")
#endif
struct DEVICEINSTANCE
{
	ID3D11Device * d3dDevice;
	ID3D11DeviceContext * d3dContext;

	IDXGISwapChain * dxgiSwapChain;
	ID3D11RenderTargetView * d3dRenderTargetView;
	ID3D11Texture2D * d3dDepthStencilBuffer;
	ID3D11DepthStencilView * d3dDepthStencilView;
//////////////// DIRECT3D 11 DEFINITIONS ////////////////
#elif(CHECK_3DMODE(OPENGL2) || CHECK_3DMODE(OPENGL3) || CHECK_3DMODE(OPENGL4))
////////////////// OPENGLs DEFINITIONS //////////////////
#include <GL/GL.h>
#include <GL/GLU.h>
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
struct DEVICEINSTANCE
{

};
#if(CHECK_3DMODE(OPENGL3) || CHECK_3DMODE(OPENGL4))
#include "wglext.h"
#endif
////////////////// OPENGLs DEFINITIONS //////////////////
#endif
#pragma endregion

#pragma region Input Methods
bool IsKeyDown(BYTE keys);

int GetMouseX();
int GetMouseY();
bool IsMouseButtonDown(BYTE button);

#if (USED_XINPUTMODE == 1)
////////////////// XINPUT1 DEFINITIONS //////////////////
#include <XInput.h>
#pragma comment (lib, "XInput.lib")

int GetLeftThumbStickX();
int GetLeftThumbStickY();
int GetRightThumbStickX();
int GetRightThumbStickY();
int GetLeftTrigger();
int GetRightTrigger();
bool IsGamePadButtonDown(int button);
void GamePadVibration(int left, int right);
////////////////// XINPUT1 DEFINITIONS //////////////////
#endif
#pragma endregion

// User Definition methods
void OnInitial(DEVICEINSTANCE *device);
void OnUpdate(float dt);
void OnDraw(DEVICEINSTANCE *device);
void OnDestroy();
void OnResize(int width, int height);