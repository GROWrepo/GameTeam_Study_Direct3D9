#define USED_EXTENSION 1
#include "../Framework/Framework.h"

ID3DXMesh * mesh1, *mesh2;
D3DMATERIAL9 material1, material2;
D3DLIGHT9 light;

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

	D3DXCreateBox(device->d3dDevice, 2, 2, 2, &mesh1, NULL);
	D3DXCreateTeapot(device->d3dDevice, &mesh2, NULL);

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = D3DXVECTOR3(1, -1, 1);
	light.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);
	light.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	material1.Ambient = material1.Diffuse = material1.Specular =
		D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	material1.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	material1.Power = 1.0f;

	material2.Ambient = material2.Diffuse = material2.Specular =
		material2.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material2.Diffuse.a = 0.5f;
}

void OnUpdate(float dt)
{

}

void OnDraw(DEVICEINSTANCE *device)
{
	device->d3dDevice->BeginScene();
	device->d3dDevice->SetLight(0, &light);
	device->d3dDevice->LightEnable(0, true);

	D3DXMATRIX originMatrix, myMatrix;
	device->d3dDevice->GetTransform(D3DTS_WORLD, &originMatrix);

	device->d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device->d3dDevice->SetTransform(D3DTS_WORLD, D3DXMatrixTranslation(&myMatrix, 0, 0, 1));
	device->d3dDevice->SetMaterial(&material1);
	mesh1->DrawSubset(0);

	device->d3dDevice->SetTransform(D3DTS_WORLD, D3DXMatrixTranslation(&myMatrix, 0, 0, -1));
	device->d3dDevice->SetMaterial(&material2);
	mesh2->DrawSubset(0);

	device->d3dDevice->SetTransform(D3DTS_WORLD, &originMatrix);

	device->d3dDevice->EndScene();
}

void OnDestroy()
{
	mesh2->Release();
	mesh1->Release();
}