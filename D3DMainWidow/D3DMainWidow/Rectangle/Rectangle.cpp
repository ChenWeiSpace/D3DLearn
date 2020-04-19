#include "Rectangle.h"
#include "../D3DCamera.h"

DrawRectangle::DrawRectangle()
{
	D3DXMatrixIdentity(&g_World);
}

DrawRectangle::~DrawRectangle()
{
}

struct ConstantBuffer_DrawRectangle
{
	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProjection;
	D3DXVECTOR4 vOutputColor;
	D3DXVECTOR4 trans;
};

void DrawRectangle::inits(Context& con)
{
	HRESULT hr = S_OK;
	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile("Rectangle/drawRectangle.hlsl", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		// 		MessageBox(NULL,
		// 			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return;
	}

	// Create the vertex shader
	hr = con.g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = con.g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return;

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("Rectangle/drawRectangle.hlsl", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		// 		MessageBox(NULL,
		// 			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return;
	}

	// Create the pixel shader
	hr = con.g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return;

	D3DXVECTOR3 pos[] = { D3DXVECTOR3(-1.0f,0.0,-1.0f),D3DXVECTOR3(-1.0f,0.0,1.0f) ,D3DXVECTOR3(1.0f,0.0,-1.0f) ,D3DXVECTOR3(1.0f,0.0,1.0f) };

	CD3D11_RASTERIZER_DESC rasterizser(D3D11_DEFAULT);
	rasterizser.CullMode = D3D11_CULL_NONE;
	hr = con.g_pd3dDevice->CreateRasterizerState(&rasterizser, &m_pRasterizerState);
	if (FAILED(hr))
		return;
	CD3D11_BLEND_DESC blendDesc(D3D11_DEFAULT);
	
	hr = con.g_pd3dDevice->CreateBlendState(&blendDesc, &m_pBlendState);
	if (FAILED(hr))
		return;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(pos);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = pos;
	hr = con.g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
		return;

	ZeroMemory(&bd, sizeof(bd));
	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer_DrawRectangle);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	hr = con.g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
	if (FAILED(hr))
		return;
}

void DrawRectangle::frameMove(std::uint64_t frameNumber, std::uint64_t elapsed)
{
	ConstantBuffer_DrawRectangle cb1;
	//D3DXMatrixRotationX(&g_World, D3DXToRadian(90));
	D3DXMatrixScaling(&g_World,1,1,100);
	D3DXMatrixTranspose(&cb1.mWorld,&g_World);

	D3DXMATRIX g_View = (m_camera->getViewMatrix());;
	D3DXMatrixTranspose(&cb1.mView, &g_View);
	D3DXMATRIX g_Projection = (m_camera->getProjectionMatrix());;
	D3DXMatrixTranspose(&cb1.mProjection, &g_Projection);

	cb1.vOutputColor = D3DXVECTOR4(1.0, 0, 0, 1.0);
	cb1.trans = D3DXVECTOR4(1.0, 0, 1.0,0.1);
	
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);

	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	UINT stride = sizeof(D3DXVECTOR3);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
	g_pImmediateContext->RSSetState(m_pRasterizerState);
	//g_pImmediateContext->OMSetBlendState(m_pBlendState,NULL,0);
	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	g_pImmediateContext->Draw(4, 0);

}
