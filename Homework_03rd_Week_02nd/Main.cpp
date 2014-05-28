#include "../Framework/Framework.h"

struct MyVertex
{
	float x, y, z;
	D3DCOLOR diffuse;

	static const DWORD D3DFVF_CUSTOM =
		D3DFVF_XYZ | D3DFVF_DIFFUSE;
};

MyVertex vertices[5] = {
		{ -0.5f, 0, 0, D3DCOLOR_XRGB(255, 0, 0) },
		{ 0, 0.5f, 0, D3DCOLOR_XRGB(0, 255, 0) },
		{ 0.5f, 0, 0, D3DCOLOR_XRGB(0, 0, 255) },
		{ -0.25f, -0.5f, 0, D3DCOLOR_XRGB(255, 255, 255) },
		{ 0.25f, -0.5f, 0, D3DCOLOR_XRGB(255, 255, 255) },
};

int indices[9] = {
	0, 1, 2,
	0, 2, 3,
	3, 2, 4,
};

void OnInitial(DEVICEINSTANCE *device)
{

}

void OnUpdate(float dt)
{

}

void OnDraw(DEVICEINSTANCE *device)
{
	device->d3dDevice->BeginScene();

	device->d3dDevice->SetRenderState(D3DRS_LIGHTING, 0);
	device->d3dDevice->SetFVF(MyVertex::D3DFVF_CUSTOM);
	device->d3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
		0, 5, 3, indices, D3DFMT_INDEX32, vertices, sizeof(MyVertex));

	device->d3dDevice->EndScene();
}

void OnDestroy()
{

}