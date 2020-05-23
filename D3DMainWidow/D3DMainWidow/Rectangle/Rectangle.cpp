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
	rasterizser.DepthBiasClamp = 1.0f;
	rasterizser.CullMode = D3D11_CULL_NONE;

	hr = con.g_pd3dDevice->CreateRasterizerState(&rasterizser, &m_pRasterizerState);
	if (FAILED(hr))
		return;
	CD3D11_BLEND_DESC blendDesc(D3D11_DEFAULT);
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	hr = con.g_pd3dDevice->CreateBlendState(&blendDesc, &m_pBlendState);
	if (FAILED(hr))
		return;
	CD3D11_DEPTH_STENCIL_DESC depDesc(D3D11_DEFAULT);
// 	depDesc.DepthEnable = FALSE;
// 	depDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	con.g_pd3dDevice->CreateDepthStencilState(&depDesc, &m_depthStencilState);

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
	D3DXMatrixScaling(&g_World,1,1,1000);
	D3DXMATRIX    tanslat;
	D3DXMatrixTranslation(&tanslat, 0, 0, -30);
	D3DXMatrixMultiplyTranspose(&cb1.mWorld, &g_World, &tanslat);
	//D3DXMatrixTranspose(&cb1.mWorld,&g_World);

	D3DXMATRIX g_View = (m_camera->getViewMatrix());;
	D3DXMatrixTranspose(&cb1.mView, &g_View);
	D3DXMATRIX g_Projection = (m_camera->getProjectionMatrix());;
	D3DXMatrixTranspose(&cb1.mProjection, &g_Projection);

	cb1.vOutputColor = D3DXVECTOR4(1.0, 0, 0, 1.0);
	cb1.trans = D3DXVECTOR4(1.0, 0, 1.0,0.1);
	
	//ID3D11DepthStencilState* ppDepthStencilState = NULL;
	//UINT pStencilRef = 0;
	//D3D11_DEPTH_STENCIL_DESC pDesc;

	//g_pImmediateContext->OMGetDepthStencilState(&ppDepthStencilState,&pStencilRef);
	//if (ppDepthStencilState)
	//{
	//	ppDepthStencilState->GetDesc(&pDesc);
	//}
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);

	g_pImmediateContext->OMSetDepthStencilState(m_depthStencilState, 0);
	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	UINT stride = sizeof(D3DXVECTOR3);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
	g_pImmediateContext->RSSetState(m_pRasterizerState);
	float blendStats[4] = { 1.0f,1.0f, 1.0f, 1.0f };
	g_pImmediateContext->OMSetBlendState(m_pBlendState, blendStats, 0xffffffff);
	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	g_pImmediateContext->Draw(4, 0);

}
