#include "../Framework/Framework.h"

struct MyVertex
{
	float x, y, z;
	D3DCOLOR diffuse;

	static const DWORD D3DFVF_CUSTOM =
		D3DFVF_XYZ | D3DFVF_DIFFUSE;
};

MyVertex vertices[6] = {
	{ -0.5f, 0, 0, D3DCOLOR_XRGB(255, 0, 0) },
	{ 0, 0.5f, 0, D3DCOLOR_XRGB(0, 255, 0) },
	{ 0.5f, 0, 0, D3DCOLOR_XRGB(0, 0, 255) },
	{ -0.5f, 0, 0, D3DCOLOR_XRGB(255, 0, 0) },
	{ 0.5f, 0, 0, D3DCOLOR_XRGB(0, 0, 255) },
	{ 0, -0.5f, 0, D3DCOLOR_XRGB(255, 255, 255) },
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
	device->d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertices, sizeof(MyVertex));

	device->d3dDevice->EndScene();
}

void OnDestroy()
{

}