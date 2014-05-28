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

IDirect3DVertexBuffer9 * vertexBuffer;

void OnInitial(DEVICEINSTANCE *device)
{
	device->d3dDevice->CreateVertexBuffer(sizeof(MyVertex) * 6, D3DUSAGE_DYNAMIC,
		MyVertex::D3DFVF_CUSTOM, D3DPOOL_DEFAULT, &vertexBuffer,
		nullptr);
	MyVertex * bufferForVertex;
	vertexBuffer->Lock(0, sizeof(MyVertex) * 6, (void**)&bufferForVertex,
		D3DLOCK_DISCARD);
	memcpy(bufferForVertex, vertices, sizeof(vertices));
	vertexBuffer->Unlock();
}

void OnUpdate(float dt)
{

}

void OnDraw(DEVICEINSTANCE *device)
{
	device->d3dDevice->BeginScene();

	device->d3dDevice->SetRenderState(D3DRS_LIGHTING, 0);
	device->d3dDevice->SetFVF(MyVertex::D3DFVF_CUSTOM);
	device->d3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(MyVertex));
	device->d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	device->d3dDevice->EndScene();
}

void OnDestroy()
{
	vertexBuffer->Release();
}