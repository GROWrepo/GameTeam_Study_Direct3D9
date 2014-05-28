#define USED_EXTENSION 1
#include "../Framework/Framework.h"

ID3DXMesh * mesh1, *mesh2;

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
}

void OnUpdate(float dt)
{

}

void OnDraw(DEVICEINSTANCE *device)
{
	device->d3dDevice->BeginScene();
	device->d3dDevice->SetRenderState(D3DRS_LIGHTING, 0);

	D3DXMATRIX originMatrix, myMatrix;
	device->d3dDevice->GetTransform(D3DTS_WORLD, &originMatrix);

	device->d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
	device->d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR);

	device->d3dDevice->SetTransform(D3DTS_WORLD, D3DXMatrixTranslation(&myMatrix, 0, 0, 1));
	mesh1->DrawSubset(0);

	device->d3dDevice->SetTransform(D3DTS_WORLD, D3DXMatrixTranslation(&myMatrix, 0, 0, -1));
	mesh2->DrawSubset(0);

	device->d3dDevice->SetTransform(D3DTS_WORLD, &originMatrix);

	device->d3dDevice->EndScene();
}

void OnDestroy()
{
	mesh2->Release();
	mesh1->Release();
}