#define USED_EXTENSION 1
#define USED_XINPUTMODE 1
#include "../Framework/Framework.h"

ID3DXMesh * skybox;
IDirect3DTexture9 * texture;

struct TerrainVertex {
	float x, y, z, u, v;

	TerrainVertex() { }
	TerrainVertex(float x, float y, float z, float u, float v)
		: x(x), y(y), z(z), u(u), v(v) { }

	static const DWORD Terrain_FVF = D3DFVF_XYZ | D3DFVF_TEX1;
};

IDirect3DTexture9 * heightMapTexture, *heightMap;

TerrainVertex * terrainVertices;
int * terrainIndices;

int numOfVertices, numOfIndices, terrainWidth, terrainHeight;

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
	D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI / 4, 800 / 600.0f, 1.0f, 10000.0f);
	device->d3dDevice->SetTransform(D3DTS_PROJECTION, &projection);

	D3DXLoadMeshFromX(L"skybox.x", 0, device->d3dDevice, nullptr, nullptr, nullptr, nullptr, &skybox);

	D3DXCreateTextureFromFile(device->d3dDevice, L"sky copy.png", &texture);

	D3DXCreateTextureFromFile(device->d3dDevice, L"terrain_01.jpg", &heightMap);
	D3DXCreateTextureFromFile(device->d3dDevice, L"terrain_02.jpg", &heightMapTexture);

	D3DXIMAGE_INFO heightMapInfo = { 0, };
	D3DXGetImageInfoFromFile(L"terrain_01.jpg", &heightMapInfo);
	terrainWidth = heightMapInfo.Width; terrainHeight = heightMapInfo.Height;

	D3DCOLOR * colors;
	D3DLOCKED_RECT lockedRect = { 0, };
	RECT rect = { 0, 0, terrainWidth, terrainHeight };
	terrainVertices = new TerrainVertex[numOfVertices = terrainWidth * terrainHeight];
	heightMap->LockRect(0, &lockedRect, &rect, 0);
	colors = (D3DCOLOR*)lockedRect.pBits;
	TerrainVertex * tempVertices = terrainVertices;
	for (int x = 0; x < terrainHeight; x++)
	{
		for (int z = 0; z < terrainWidth; z++)
		{
			int location = x * terrainWidth + z;
			*tempVertices = TerrainVertex(
				(x - terrainWidth / 2) * 5.0f, (colors[location] & 0xff) * 5.0f / 2,
				(z - terrainWidth / 2) * 5.0f,
				z / (float)terrainWidth, x / (float)terrainHeight
				);
			tempVertices++;
		}
	}
	heightMap->UnlockRect(0);
	heightMap->Release();

	terrainIndices = new int[numOfIndices = (terrainWidth - 1) * (terrainHeight - 1) * 2 * 3];
	struct Polygon { int index[3]; } *tempIndices = (Polygon*)terrainIndices;

	for (int z = 0; z < terrainHeight - 1; z++)
	{
		for (int x = 0; x < terrainWidth - 1; x++)
		{
			(*tempIndices).index[0] = z * terrainWidth + x;
			(*tempIndices).index[1] = z * terrainWidth + (x + 1);
			(*tempIndices).index[2] = (z + 1) * terrainWidth + x;
			tempIndices++;
			(*tempIndices).index[0] = (z + 1) * terrainWidth + x;
			(*tempIndices).index[1] = z * terrainWidth + (x + 1);
			(*tempIndices).index[2] = (z + 1) * terrainWidth + (x + 1);
			tempIndices++;
		}
	}
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
	D3DXMatrixScaling(&scaleMatrix, 10, 10, 10);
	D3DXMatrixTranslation(&transMatrix, cameraPos.x, -200, cameraPos.z);
	calcedMatrix = scaleMatrix * transMatrix;
	device->d3dDevice->SetTransform(D3DTS_WORLD, &calcedMatrix);
	device->d3dDevice->SetTexture(0, texture);
	skybox->DrawSubset(0);
	device->d3dDevice->SetTexture(0, nullptr);

	device->d3dDevice->SetFVF(TerrainVertex::Terrain_FVF);
	device->d3dDevice->SetTexture(0, heightMapTexture);
	D3DXMatrixScaling(&scaleMatrix, 1, 1, 1);
	D3DXMatrixTranslation(&transMatrix, 0, -200, 0);
	calcedMatrix = scaleMatrix * transMatrix;
	device->d3dDevice->SetTransform(D3DTS_WORLD, &calcedMatrix);
	device->d3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, numOfVertices, numOfIndices / 3,
		terrainIndices, D3DFMT_INDEX32, terrainVertices, sizeof(TerrainVertex));

	device->d3dDevice->EndScene();
}

void OnDestroy()
{
	delete[] terrainIndices;
	delete[] terrainVertices;
	heightMapTexture->Release();

	texture->Release();
	skybox->Release();
}