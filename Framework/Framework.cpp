#include "Framework.h"

#ifndef nullptr
#define nullptr 0
#endif

DEVICEINSTANCE deviceInstance;

#pragma region Input methods
bool pressedKeys[256];
bool pressedMouseButton[3];
int mouseX, mouseY, mouseWheel;

bool IsKeyDown(BYTE keys) { return pressedKeys[keys]; }
int GetMouseX() { return mouseX; }
int GetMouseY() { return mouseY; }
bool IsMouseButtonDown(BYTE button) { return pressedMouseButton[button]; }

#if (USED_XINPUTMODE == 1)
int leftThumbStickX, leftThumbStickY;
int rightThumbStickX, rightThumbStickY;
int leftTrigger, rightTrigger;
int pressedGamePadButton;

int GetLeftThumbStickX() { return leftThumbStickX; }
int GetLeftThumbStickY() { return leftThumbStickY; }
int GetRightThumbStickX() { return rightThumbStickX; }
int GetRightThumbStickY() { return rightThumbStickY; }
int GetLeftTrigger() { return leftTrigger; }
int GetRightTrigger() { return rightTrigger; }
bool IsGamePadButtonDown(int button) { return pressedGamePadButton & button; }
void GamePadVibration(int left, int right)
{
	XINPUT_VIBRATION vib = {0, };
	vib.wLeftMotorSpeed = left;
	vib.wRightMotorSpeed = right;
	XInputSetState(0, &vib);
}
#endif
#pragma endregion

#if(USING_RESIZE == 0)
void OnResize(int width, int height)
{
	
}
#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_KEYDOWN:
		pressedKeys[wParam] = true;
		break;
	case WM_KEYUP:
		pressedKeys[wParam] = false;
		break;

	case WM_LBUTTONDOWN:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		pressedMouseButton[0] = true;
		break;
	case WM_LBUTTONUP:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		pressedMouseButton[0] = false;
		break;
	case WM_RBUTTONDOWN:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		pressedMouseButton[1] = true;
		break;
	case WM_RBUTTONUP:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		pressedMouseButton[1] = false;
		break;
	case WM_MBUTTONDOWN:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		pressedMouseButton[2] = true;
		break;
	case WM_MBUTTONUP:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		pressedMouseButton[2] = false;
		break;

	case WM_MOUSEWHEEL:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
		break;

	case WM_MOUSEMOVE:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		break;

	case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			int width = rect.right - rect.left,
				height = rect.bottom - rect.top;
			OnResize(width, height);
		}
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	WNDCLASS wndClass = {0, };
	wndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = TEXT("3DFramework");
	wndClass.lpfnWndProc = WndProc;
	RegisterClass(&wndClass);

	int x, y, width, height;
	RECT adjust = {0, 0, 800, 600};
	AdjustWindowRect(&adjust, WS_OVERLAPPEDWINDOW, FALSE);
	width = adjust.right - adjust.left;
	height = adjust.bottom - adjust.top;
	x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	HWND hWnd = CreateWindow(TEXT("3DFramework"), TEXT("GROW 3D Framework"),
		WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, hInstance, NULL);

#if(CHECK_3DMODE(DIRECT3DMODE9))
	deviceInstance.d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS pp = {0, };
	pp.BackBufferWidth = 800;
	pp.BackBufferHeight = 600;
	pp.hDeviceWindow = hWnd;
	pp.BackBufferCount = 1;
	pp.BackBufferFormat = D3DFMT_A8R8G8B8;
	pp.EnableAutoDepthStencil = true;
	pp.AutoDepthStencilFormat = D3DFMT_D24S8;
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.Windowed = true;

	if(FAILED(deviceInstance.d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, 
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &deviceInstance.d3dDevice)))
		return -1;
#elif(CHECK_3DMODE(DIRECT3DMODE11))
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc = {0, };
	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Width = 800;
	dxgiSwapChainDesc.BufferDesc.Height = 600;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	dxgiSwapChainDesc.Windowed = true;
	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3};

	if(FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &dxgiSwapChainDesc,
		&deviceInstance.dxgiSwapChain, &deviceInstance.d3dDevice, nullptr, 
		&deviceInstance.d3dContext)))
		return -1;

	ID3D11Texture2D * buffer;
	if(FAILED(deviceInstance.dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(void**)&buffer)))
		return -1;

	if(FAILED(deviceInstance.d3dDevice->CreateRenderTargetView(buffer, nullptr, 
		&deviceInstance.d3dRenderTargetView)))
		return -1;

	D3D11_TEXTURE2D_DESC textureDesc = {0, };
	textureDesc.Width = 800;
	textureDesc.Height = 600;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if(FAILED(deviceInstance.d3dDevice->CreateTexture2D(&textureDesc, 
		nullptr, &deviceInstance.d3dDepthStencilBuffer)))
		return -1;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if(FAILED(deviceInstance.d3dDevice->CreateDepthStencilView(deviceInstance.d3dDepthStencilBuffer,
		&depthStencilViewDesc, &deviceInstance.d3dDepthStencilView)))
		return -1;

	deviceInstance.d3dContext->OMSetRenderTargets(1, &deviceInstance.d3dRenderTargetView,
		deviceInstance.d3dDepthStencilView);
#elif(CHECK_3DMODE(OPENGL2) || CHECK_3DMODE(OPENGL3) || CHECK_3DMODE(OPENGL4))
	HDC hDC = GetDC(hWnd);

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {
														sizeof(PIXELFORMATDESCRIPTOR), 1,
														PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW |
														PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32,
														0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
														16, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0,
												  };
	int pixelFormat = ChoosePixelFormat(hDC, &pixelFormatDescriptor);
	if(!SetPixelFormat(hDC, pixelFormat, &pixelFormatDescriptor))
		return -1;

	HGLRC glContext = wglCreateContext(hDC);
	if(glContext == 0)
		return -1;
	if(!wglMakeCurrent(hDC, glContext))
		return -1;

#if(CHECK_3DMODE(OPENGL3) || CHECK_3DMODE(OPENGL4))
	int attribs[] = {
						WGL_CONTEXT_MAJOR_VERSION_ARB,
#if(CHECK_3DMODE(OPENGL4))
														4,
#else
														3,
#endif
						WGL_CONTEXT_MINOR_VERSION_ARB, 0, 0
					};
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)
		wglGetProcAddress("wglCreateContextAttribsARB");

	if(wglCreateContextAttribsARB)
	{
		HGLRC temp = wglCreateContextAttribsARB(hDC, 0, attribs);
		wglDeleteContext(glContext);
		glContext = temp;
		if(!wglMakeCurrent(hDC, glContext))
			return -1;
	}
#endif
#endif
	
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

#if (USED_XINPUTMODE == 1)
	XInputEnable(true);
#endif

	OnInitial(&deviceInstance);

	MSG msg;
	DWORD nowTickCount, lastTickCount;
	lastTickCount = GetTickCount();

	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float dt = ((nowTickCount = GetTickCount()) - lastTickCount) / 1000.0f;
			lastTickCount = nowTickCount;

#if (USED_XINPUTMODE == 1)
			XINPUT_STATE inputState;
			if(XInputGetState(0, &inputState) == ERROR_SUCCESS)
			{
				if(inputState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
					inputState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
					inputState.Gamepad.sThumbLX = 0;
				if(inputState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
					inputState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
					inputState.Gamepad.sThumbLY = 0;
				if(inputState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && 
					inputState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
					inputState.Gamepad.sThumbRX = 0;
				if(inputState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && 
					inputState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
					inputState.Gamepad.sThumbRY = 0;

				leftThumbStickX = inputState.Gamepad.sThumbLX;
				leftThumbStickY = inputState.Gamepad.sThumbLY;
				rightThumbStickX = inputState.Gamepad.sThumbRX;
				rightThumbStickY = inputState.Gamepad.sThumbRY;

				leftTrigger = inputState.Gamepad.bLeftTrigger;
				rightTrigger = inputState.Gamepad.bRightTrigger;

				pressedGamePadButton = inputState.Gamepad.wButtons;
			}
#endif

			OnUpdate(dt);
#if(CHECK_3DMODE(DIRECT3DMODE9))
			deviceInstance.d3dDevice->Clear(1, nullptr, 
				D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
				0xFF6495ED, 1.0f, 0);
#elif(CHECK_3DMODE(DIRECT3DMODE11))
			float clearColor[] = {0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f};
			deviceInstance.d3dContext->ClearRenderTargetView(deviceInstance.d3dRenderTargetView,
				clearColor);
			deviceInstance.d3dContext->ClearDepthStencilView(deviceInstance.d3dDepthStencilView,
				D3D11_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
#elif(CHECK_3DMODE(OPENGL2) || CHECK_3DMODE(OPENGL3) || CHECK_3DMODE(OPENGL4))
			glClearColor(0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
			OnDraw(&deviceInstance);
#if(CHECK_3DMODE(DIRECT3DMODE9))
			deviceInstance.d3dDevice->Present(nullptr, nullptr, NULL, nullptr);
#elif(CHECK_3DMODE(DIRECT3DMODE11))
			deviceInstance.dxgiSwapChain->Present(0, 0);
#elif(CHECK_3DMODE(OPENGL2) || CHECK_3DMODE(OPENGL3) || CHECK_3DMODE(OPENGL4))
			SwapBuffers(hDC);
#endif
			Sleep(1);
		}
	}

	OnDestroy();

#if (USED_XINPUTMODE == 1)
	XInputEnable(false);
#endif

#if(CHECK_3DMODE(DIRECT3DMODE9))
	SAFE_RELEASE(deviceInstance.d3dDevice);
	SAFE_RELEASE(deviceInstance.d3d);
#elif(CHECK_3DMODE(DIRECT3DMODE11))
	SAFE_RELEASE(deviceInstance.d3dDepthStencilView);
	SAFE_RELEASE(deviceInstance.d3dDepthStencilBuffer);
	SAFE_RELEASE(deviceInstance.d3dRenderTargetView);
	SAFE_RELEASE(deviceInstance.dxgiSwapChain);
	if(deviceInstance.d3dContext)
		deviceInstance.d3dContext->ClearState();
	SAFE_RELEASE(deviceInstance.d3dContext);
	SAFE_RELEASE(deviceInstance.d3dDevice);
#elif(CHECK_3DMODE(OPENGL2) || CHECK_3DMODE(OPENGL3) || CHECK_3DMODE(OPENGL4))
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(glContext);
	ReleaseDC(hWnd, hDC);
#endif

	return msg.wParam;
}