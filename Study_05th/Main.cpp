#define USED_EXTENSION 1
#include "../Framework/Framework.h"

struct MyVertex
{
	float x, y, z;
	D3DCOLOR diffuse;

	static const DWORD D3DFVF_CUSTOM =
		D3DFVF_XYZ | D3DFVF_DIFFUSE;
};

MyVertex vertices[3] = {
		{ -01, 00, 00, D3DCOLOR_XRGB(255, 0, 0) },
		{ 00, 01, 00, D3DCOLOR_XRGB(255, 255, 0) },
		{ 01, 00, 00, D3DCOLOR_XRGB(255, 255, 255) },
};

void OnInitial(DEVICEINSTANCE *device)
{
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI / 2,
		800 / 600.0f, 0.0f, 1000.0f);
	device->d3dDevice->SetTransform(D3DTS_PROJECTION, &projection);

	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, -1),
		&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	device->d3dDevice->SetTransform(D3DTS_VIEW, &view);
}

void OnUpdate(float dt)
{

}

void OnDraw(DEVICEINSTANCE *device)
{
	device->d3dDevice->BeginScene();

	device->d3dDevice->SetRenderState(D3DRS_LIGHTING, 0);
	device->d3dDevice->SetFVF(MyVertex::D3DFVF_CUSTOM);

	D3DXMATRIX world, rotate1, rotate2;
	D3DXMatrixRotationX(&rotate1, 45 * D3DX_PI / 180);
	D3DXMatrixRotationY(&rotate2, 45 * D3DX_PI / 180);
	world = rotate1 * rotate2;
	device->d3dDevice->SetTransform(D3DTS_WORLD, &world);

	device->d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(MyVertex));

	device->d3dDevice->EndScene();
}

void OnDestroy()
{

}