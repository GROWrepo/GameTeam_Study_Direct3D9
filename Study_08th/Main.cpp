#define USED_EXTENSION 1
#define USED_XINPUTMODE 1
#include "../Framework/Framework.h"

ID3DXMesh * skybox;
IDirect3DTexture9 * texture;
ID3DXMesh * teapot;

float cameraPitch = 0, cameraYaw = 0, cameraRoll = 0;
D3DXVECTOR3 cameraPos;
D3DXVECTOR3 cameraTarget, cameraUpVector;
D3DXVECTOR3 move;

void strafe(float unit)
{
	move.x -= unit;
}

void walk(float unit)
{
	move.z -= unit;
}

void pitch(float unit)
{
	cameraPitch += unit;
	if (cameraPitch > 3.141592f * 2) cameraPitch -= 3.141592f * 2;
	if (cameraPitch < 3.141592f * 2) cameraPitch += 3.141592f * 2;
}

void yaw(float unit)
{
	cameraYaw += unit;
	if (cameraYaw > 3.141592f * 2) cameraYaw -= 3.141592f * 2;
	if (cameraYaw < 3.141592f * 2) cameraYaw += 3.141592f * 2;
}

void roll(float unit)
{
	cameraRoll += unit;
	if (cameraRoll > 3.141592f * 2) cameraRoll -= 3.141592f * 2;
	if (cameraRoll < 3.141592f * 2) cameraRoll += 3.141592f * 2;
}

void setCameraMatrix(IDirect3DDevice9 * device)
{
	D3DXMATRIX cameraMatrix;
	D3DXMatrixRotationYawPitchRoll(&cameraMatrix, cameraYaw, 0, 0);
	D3DXVECTOR3 transTemp;
	D3DXVec3TransformCoord(&transTemp, &move, &cameraMatrix);
	cameraPos += transTemp;

	D3DXMatrixRotationYawPitchRoll(&cameraMatrix, cameraYaw, cameraPitch, cameraRoll);

	D3DXVec3TransformCoord(&cameraTarget, &D3DXVECTOR3(0, 0, -1), &cameraMatrix);
	D3DXVec3TransformCoord(&cameraUpVector, &D3DXVECTOR3(0, 1, 0), &cameraMatrix);

	D3DXVECTOR3 targetTemp = cameraTarget + cameraPos;

	D3DXMatrixLookAtLH(&cameraMatrix, &cameraPos, &targetTemp, &cameraUpVector);
	device->SetTransform(D3DTS_VIEW, &cameraMatrix);

	move = D3DXVECTOR3(0, 0, 0);
}

void OnInitial(DEVICEINSTANCE *device)
{
	XInputEnable(true);

	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI / 4, 800 / 600.0f, 0.0001f, 1000.0f);
	device->d3dDevice->SetTransform(D3DTS_PROJECTION, &projection);

	D3DXLoadMeshFromX(L"skybox.x", 0, device->d3dDevice, nullptr, nullptr, nullptr, nullptr, &skybox);

	D3DXCreateTeapot(device->d3dDevice, &teapot, nullptr);

	D3DXCreateTextureFromFile(device->d3dDevice, L"sky copy.png", &texture);
}

void OnUpdate(float dt)
{
	XINPUT_STATE inputState = { 0, };
	if (XInputGetState(0, &inputState) == ERROR_SUCCESS)
	{
		if (inputState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			inputState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			inputState.Gamepad.sThumbLX = 0;
		if (inputState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			inputState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			inputState.Gamepad.sThumbLY = 0;
		if (inputState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			inputState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			inputState.Gamepad.sThumbRX = 0;
		if (inputState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			inputState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			inputState.Gamepad.sThumbRY = 0;

		if (inputState.Gamepad.sThumbLY > 0) walk(30 * dt);
		else if (inputState.Gamepad.sThumbLY < 0) walk(-30 * dt);
		if (inputState.Gamepad.sThumbLX > 0) strafe(30 * dt);
		else if (inputState.Gamepad.sThumbLX < 0) strafe(-30 * dt);

		if (inputState.Gamepad.sThumbRY > 0) pitch(0.5f * dt);
		else if (inputState.Gamepad.sThumbRY < 0) pitch(-0.5f * dt);
		if (inputState.Gamepad.sThumbRX > 0) yaw(0.5f * dt);
		else if (inputState.Gamepad.sThumbRX < 0) yaw(-0.5f * dt);
	}
}

void OnDraw(DEVICEINSTANCE *device)
{
	device->d3dDevice->BeginScene();
	device->d3dDevice->SetRenderState(D3DRS_LIGHTING, 0);

	setCameraMatrix(device->d3dDevice);

	D3DXMATRIX calcedMatrix, rotateMatrix, scaleMatrix, transMatrix;
	D3DXMatrixScaling(&scaleMatrix, 3, 3, 3);
	D3DXMatrixTranslation(&transMatrix, cameraPos.x, -50, cameraPos.z);
	calcedMatrix = scaleMatrix * transMatrix;
	device->d3dDevice->SetTransform(D3DTS_WORLD, &calcedMatrix);
	device->d3dDevice->SetTexture(0, texture);
	skybox->DrawSubset(0);
	device->d3dDevice->SetTexture(0, nullptr);

	D3DXMatrixScaling(&scaleMatrix, 20, 20, 20);
	D3DXMatrixTranslation(&transMatrix, 0, 30, 0);
	calcedMatrix = scaleMatrix * transMatrix;
	device->d3dDevice->SetTransform(D3DTS_WORLD, &calcedMatrix);
	device->d3dDevice->SetTexture(0, 0);
	teapot->DrawSubset(0);

	device->d3dDevice->EndScene();
}

void OnDestroy()
{
	texture->Release();
	teapot->Release();
	skybox->Release();
}