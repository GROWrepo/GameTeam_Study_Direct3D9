#define USED_EXTENSION 1
#include "../Framework/Framework.h"

struct MyVertex
{
	float x, y, z; float n1, n2, n3;

	static const DWORD D3DFVF_CUSTOM = D3DFVF_XYZ | D3DFVF_NORMAL;
};

MyVertex vertices[12] = {
		{ -1, 0, -1, 0, 0.707f, -0.707f }, { 0, 1, 0, 0, 0.707f, -0.707f }, { 1, 0, -1, 0, 0.707f, -0.707f },
		{ -1, 0, 1, -0.707f, 0.707f, 0 }, { 0, 1, 0, -0.707f, 0.707f, 0 }, { -1, 0, -1, -0.707f, 0.707f, 0 },
		{ 1, 0, -1, 0.707f, 0.707f, 0 }, { 0, 1, 0, 0.707f, 0.707f, 0 }, { 1, 0, 1, 0.707f, 0.707f, 0 },
		{ 1, 0, 1, 0, 0.707f, 0.707f }, { 0, 1, 0, 0, 0.707f, 0.707f }, { -1, 0, 1, 0, 0.707f, 0.707f },
};

D3DLIGHT9 light;
D3DMATERIAL9 material;
float a = 0;


void OnInitial(DEVICEINSTANCE *device)
{
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI / 2,
		800 / 600.0f, 0.0f, 1000.0f);
	device->d3dDevice->SetTransform(D3DTS_PROJECTION, &projection);

	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, -5),
		&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	device->d3dDevice->SetTransform(D3DTS_VIEW, &view);

	D3DCOLORVALUE originColor = { 1, 1, 1, 1 },
		specularColor = { 1 * 0.3f, 1 * 0.3f, 1 * 0.3f, 1 * 0.3f },
		ambientColor = { 1 * 0.6f, 1 * 0.6f, 1 * 0.6f, 1 * 0.6f },
		blackColor = { 0, 0, 0, 1 };

	light.Type = D3DLIGHT_SPOT;
	light.Diffuse = originColor;
	light.Specular = specularColor;
	light.Ambient = ambientColor;
	light.Direction = D3DXVECTOR3(0, 0, 0);
	light.Position = D3DXVECTOR3(0, 0, -2);
	light.Theta = 0.6f;
	light.Phi = 0.2f;
	light.Range = 2;
	light.Falloff = 1;
	light.Attenuation0 = 0;
	light.Attenuation1 = 0.5f;
	light.Attenuation2 = 1;

	material.Ambient = material.Diffuse = material.Specular = originColor;
	material.Emissive = blackColor;
	material.Power = 5.0f;
}

void OnUpdate(float dt)
{
	a += dt;
}

void OnDraw(DEVICEINSTANCE *device)
{
	device->d3dDevice->BeginScene();

	device->d3dDevice->SetFVF(MyVertex::D3DFVF_CUSTOM);

	device->d3dDevice->LightEnable(0, TRUE);
	device->d3dDevice->SetLight(0, &light);

	device->d3dDevice->SetMaterial(&material);

	D3DXMATRIX rotate;
	D3DXMatrixRotationY(&rotate, a);
	device->d3dDevice->SetTransform(D3DTS_WORLD, &rotate);

	device->d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 4, vertices, sizeof(MyVertex));

	device->d3dDevice->EndScene();
}

void OnDestroy()
{

}