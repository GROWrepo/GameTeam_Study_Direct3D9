#define USED_EXTENSION 1
#include "../Framework/Framework.h"

struct MyVertex
{
	float x, y, z;
	float u, v;

	static const DWORD D3DFVF_CUSTOM = D3DFVF_XYZ | D3DFVF_TEX1;
};

MyVertex vertices[12] = {
		{ -0.5f, 0.25f, 0, 0, 0 },
		{ -0.25f, 0.25f, 0, 1, 0 },
		{ -0.5f, -0.25f, 0, 0, 1 },
		{ -0.25f, -0.25f, 0, 1, 1 },

		{ -0.25f, 0.25f, 0, 0, 0 },
		{ 0.25f, 0.25f, 0, 1, 0 },
		{ -0.25f, -0.25f, 0, 0, 1 },
		{ 0.25f, -0.25f, 0, 1, 1 },

		{ 0.25f, 0.25f, 0, 0, 0 },
		{ 0.5f, 0.25f, 0, 1, 0 },
		{ 0.25f, -0.25f, 0, 0, 1 },
		{ 0.5f, -0.25f, 0, 1, 1 },
};

IDirect3DTexture9 * m_tex1, * m_tex2, * m_tex3;

void OnInitial(DEVICEINSTANCE *device)
{
	D3DXCreateTextureFromFile(device->d3dDevice, L"texture1.jpg", &m_tex1);
	D3DXCreateTextureFromFile(device->d3dDevice, L"texture2.png", &m_tex2);
	D3DXCreateTextureFromFile(device->d3dDevice, L"texture3.png", &m_tex3);
}

void OnUpdate(float dt)
{

}

void OnDraw(DEVICEINSTANCE *device)
{
	device->d3dDevice->BeginScene();

	device->d3dDevice->SetRenderState(D3DRS_LIGHTING, 0);
	device->d3dDevice->SetFVF(MyVertex::D3DFVF_CUSTOM);

	device->d3dDevice->SetTexture(0, m_tex1);
	device->d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
		2, vertices, sizeof(MyVertex));

	device->d3dDevice->SetTexture(0, m_tex2);
	device->d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
		2, vertices + 4, sizeof(MyVertex));

	device->d3dDevice->SetTexture(0, m_tex3);
	device->d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
		2, vertices + 8, sizeof(MyVertex));

	device->d3dDevice->EndScene();
}

void OnDestroy()
{
	m_tex1->Release();
	m_tex2->Release();
	m_tex3->Release();
}