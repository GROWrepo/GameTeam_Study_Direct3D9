#define USED_EXTENSION 1
#include "../Framework/Framework.h"

struct MyVertex
{
	float x, y, z;
	float u, v;

	static const DWORD D3DFVF_CUSTOM = D3DFVF_XYZ | D3DFVF_TEX1;
};

MyVertex vertices[6] = {
		{ -0.5f, 0, 0, 0, 1 },
		{ 0, 0.5f, 0, 0, 0 },
		{ 0.5f, 0, 0, 1, 0 },
		{ -0.5f, 0, 0, 0, 1 },
		{ 0.5f, 0, 0, 1, 0 },
		{ 0, -0.5f, 0, 1, 1 },
};

IDirect3DTexture9 * m_tex;

void OnInitial(DEVICEINSTANCE *device)
{
	D3DXCreateTextureFromFile(device->d3dDevice, L"texture.jpg", &m_tex);
}

void OnUpdate(float dt)
{

}

void OnDraw(DEVICEINSTANCE *device)
{
	device->d3dDevice->BeginScene();

	device->d3dDevice->SetRenderState(D3DRS_LIGHTING, 0);
	device->d3dDevice->SetFVF(MyVertex::D3DFVF_CUSTOM);
	device->d3dDevice->SetTexture(0, m_tex);
	device->d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		2, vertices, sizeof(MyVertex));

	device->d3dDevice->EndScene();
}

void OnDestroy()
{
	m_tex->Release();
}