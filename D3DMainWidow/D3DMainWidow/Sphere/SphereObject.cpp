#include "SphereObject.h"
#include "..\GlobalInstance.h"
#include "..\D3DCamera.h"

struct ConstantBuffer_SphereObject
{
	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProjection;
	XMFLOAT4 vLightDir;
	XMFLOAT4 vLightColor;
	XMFLOAT4 vOutputColor;
};

SphereObject::SphereObject()
{

}

SphereObject::~SphereObject()
{
	
}

void SphereObject::inits(Context& con)
{
	HRESULT hr = S_OK;
	ID3D10Blob* pVSBlob = NULL;
	hr = CompileShaderFromFile("Sphere/SphereObject.fx", "VS", "vs_4_0", &pVSBlob);
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return;
	}
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return;
	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("Sphere/SphereObject.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		return;
	}

	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return;


	int x_size = 180;
	int y_size = 10;
	float x_scall = 1.0f;
	float y_scall = 0.3f;

	std::vector<VertexP_S_T> vertices;
	vertices.reserve(x_size*y_size);

	for (size_t j = 0; j < y_size; j++)
	{
		for (size_t i = 0; i < x_size; i++)
		{
			VertexP_S_T pos;
			pos.Pos = D3DXVECTOR3( cos(D3DX_PI * 2 / x_size * i), /*i* x_scall,*/j * y_scall, sin(D3DX_PI * 2 / x_size * i));
			D3DXVECTOR3 nor = D3DXVECTOR3(cos(D3DX_PI * 2 / x_size * i), 0, sin(D3DX_PI * 2 / x_size * i));;

			//pos.Nor = D3DXVECTOR3(cos(D3DX_PI * 2 / x_size * i), 0, sin(D3DX_PI * 2 / x_size * i));
			D3DXVec3Normalize(&pos.Nor, &nor);
			vertices.push_back(pos);
		}

		VertexP_S_T pos;
		pos.Pos = D3DXVECTOR3(1, /*i* x_scall,*/j * y_scall, 0);
		vertices.push_back(pos);
	}
	

	UniformGridPlane gridPlane;
	gridPlane.setPosList(vertices);
	gridPlane.setSize(Size(x_size+1, y_size));
	std::vector<unsigned int> indexList;
	gridPlane.buildIndexList(indexList);
	m_indexSize = indexList.size();
	vertices = gridPlane.getPosList();

	{
		int x_size = 100;
		int y_size = 100;
		for (size_t i = 0; i < y_size; i++)
		{
			for (size_t j = 0; j < x_size; j++)
			{

			}
		}
	}


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexP_S_T) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices.data();
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
		return;



	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * indexList.size();        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	InitData.pSysMem = indexList.data();
	hr = con.g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
		return;

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer_SphereObject);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = con.g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
	if (FAILED(hr))
		return;
	CD3D11_RASTERIZER_DESC rasterizser(D3D11_DEFAULT);
	rasterizser.DepthBiasClamp = 1.0f;
	rasterizser.CullMode = D3D11_CULL_BACK;
	rasterizser.FillMode = D3D11_FILL_SOLID;

	hr = con.g_pd3dDevice->CreateRasterizerState(&rasterizser, &m_pRasterizerState);
}

void SphereObject::frameMove(std::uint64_t frameNumber, std::uint64_t elapsed)
{

	ConstantBuffer_SphereObject cb1;
	D3DXMatrixTranspose(&(cb1.mWorld),&g_World);
	//g_View = *(XMMATRIX*)&(m_camera->getViewMatrix());;
	D3DXMatrixTranspose(&cb1.mView,&(m_camera->getViewMatrix()));
	D3DXMatrixTranspose(&cb1.mProjection,&(m_camera->getProjectionMatrix()));
	
	cb1.vLightColor = XMFLOAT4(0.3f, 0.3f, 0, 1.0f);
	cb1.vLightDir = XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f);

	//g_Projection = *(XMMATRIX*)&(m_camera->getProjectionMatrix());
	//cb1.mProjection = XMMatrixTranspose(g_Projection);
	//cb1.vLightDir[0] = vLightDirs[0];
	//cb1.vLightDir[1] = vLightDirs[1];
	//cb1.vLightColor[0] = vLightColors[0];
	//cb1.vLightColor[1] = vLightColors[1];
	//cb1.vOutputColor = XMFLOAT4(0, 0, 0, 0);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);
	g_pImmediateContext->RSSetState(m_pRasterizerState);

	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
	UINT stride = sizeof(VertexP_S_T);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	g_pImmediateContext->DrawIndexed(m_indexSize, 0,0);
}


REGIST_VISUALOBJCT(SphereObject);

UniformGridPlane::UniformGridPlane()
{

}

UniformGridPlane::~UniformGridPlane()
{

}

void UniformGridPlane::setPosList(std::vector<VertexP_S_T>& posVec)
{
	posVector = posVec;
}

std::vector<VertexP_S_T>& UniformGridPlane::getPosList()
{
	// TODO: 在此处插入 return 语句
	return posVector;
}

XMFLOAT3 operator -(const XMFLOAT3& l, const XMFLOAT3& r)
{
	return XMFLOAT3(l.x - r.x, l.y - r.y, l.z - r.z);
}

void UniformGridPlane::buildIndexList(std::vector<unsigned int> & indexList)
{
	indexList.clear();
	if (m_size.size()<=0 )
	{
		return;
	}
	for (size_t i = 0; i < m_size.y-1; i++)
	{
		for (size_t j = 0; j < m_size.x - 1; j++)
		{
			int a1 = i* m_size.x + j;
			int a2 = i * m_size.x + j+1;
			int a3 = (i+1) * m_size.x + j;
			int a4 = (i + 1) * m_size.x + j + 1;

			//第一个三角行
			{
				indexList.push_back(a2);
				indexList.push_back(a1);
				indexList.push_back(a3);
			}

			//第二个三角形
			{
				indexList.push_back(a3);
				indexList.push_back(a4);
				indexList.push_back(a2);
			}
			D3DXVECTOR3 d1 = posVector[a1].Pos - posVector[a2].Pos;
			D3DXVECTOR3 d2 = posVector[a3].Pos - posVector[a1].Pos;
			
			D3DXVec3Normalize(&d1,&d1);
			D3DXVec3Normalize(&d2, &d2);
			D3DXVECTOR3 nor;
			D3DXVec3Cross(&nor, &d1, &d2);
			//posVector[a1].Nor = nor;
		}
	}
}
