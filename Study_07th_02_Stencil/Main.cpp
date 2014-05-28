#define USED_EXTENSION 1
#include "../Framework/Framework.h"

ID3DXMesh * mesh1, *mesh2;

D3DLIGHT9 light;
D3DMATERIAL9 material, shadowMaterial;

void OnInitial(DEVICEINSTANCE *device)
{
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI / 2,
		800 / 600.0f, 0.0f, 1000.0f);
	device->d3dDevice->SetTransform(D3DTS_PROJECTION, &projection);

	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(5, 5, -5),
		&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	device->d3dDevice->SetTransform(D3DTS_VIEW, &view);

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = D3DXVECTOR3(1, -1, 1);
	light.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);
	light.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	material.Ambient = material.Diffuse = material.Specular =
		D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	material.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	material.Power = 1.0f;

	shadowMaterial.Ambient = shadowMaterial.Diffuse = shadowMaterial.Specular =
		shadowMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	shadowMaterial.Diffuse.a = 0.5f;

	D3DXCreateBox(device->d3dDevice, 9, 2, 9, &mesh1, NULL);
	D3DXCreateTeapot(device->d3dDevice, &mesh2, NULL);
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

	device->d3dDevice->SetTransform(D3DTS_WORLD, D3DXMatrixTranslation(&myMatrix, 0, -2, 0));
	material.Ambient = material.Diffuse = material.Specular =
		D3DXCOLOR(0.35f, 0.35f, 0.35f, 1.0f);
	device->d3dDevice->SetMaterial(&material);
	mesh1->DrawSubset(0);

	device->d3dDevice->SetTransform(D3DTS_WORLD, D3DXMatrixTranslation(&myMatrix, 0, 1, 0));
	material.Ambient = material.Diffuse = material.Specular =
		D3DXCOLOR(1, 1, 1, 1.0f);
	device->d3dDevice->SetMaterial(&material);
	mesh2->DrawSubset(0);

	device->d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->d3dDevice->SetRenderState(D3DRS_ZENABLE, false);

	/////////////////////////////////////// Stencil enable on Scene ///////////////////////////////////////
	/*
	device->d3dDevice->SetRenderState(D3DRS_STENCILENABLE, true);
	device->d3dDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	device->d3dDevice->SetRenderState(D3DRS_STENCILREF, 0);
	device->d3dDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	device->d3dDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	device->d3dDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	device->d3dDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	device->d3dDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
	*/
	/////////////////////////////////////// Stencil enabled on Scene ///////////////////////////////////////

	D3DXMATRIX shadowMatrix;
	D3DXMatrixShadow(&shadowMatrix, &D3DXVECTOR4(0.707f, -0.707f, 0.707f, 0.0f), &D3DXPLANE(0, -1, 0, 0));
	D3DXMatrixTranslation(&myMatrix, 2, -2, 2);
	shadowMatrix *= myMatrix;
	device->d3dDevice->SetMaterial(&shadowMaterial);
	device->d3dDevice->SetTexture(0, nullptr);
	device->d3dDevice->SetTransform(D3DTS_WORLD, &shadowMatrix);
	mesh2->DrawSubset(0);

	/////////////////////////////////////// Stencil disable on Scene ///////////////////////////////////////
	/*
	device->d3dDevice->SetRenderState(D3DRS_STENCILENABLE, false);
	*/
	/////////////////////////////////////// Stencil disabled on Scene ///////////////////////////////////////

	device->d3dDevice->SetRenderState(D3DRS_ZENABLE, true);
	device->d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	device->d3dDevice->SetTransform(D3DTS_WORLD, &originMatrix);

	device->d3dDevice->EndScene();
}

void OnDestroy()
{
	mesh2->Release();
	mesh1->Release();
}