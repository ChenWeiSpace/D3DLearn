#include "D3DEnvironment.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "AbstractVisualObject.h"


D3DEnvironment::D3DEnvironment()
{
}


D3DEnvironment::~D3DEnvironment()
{
}

void D3DEnvironment::buildDevice(int wHid, int width, int height)
{

	HWND                    g_hWnd = (HWND)wHid;
	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
// 	ID3D11Device*           g_pd3dDevice = NULL;
// 	ID3D11DeviceContext*    g_pImmediateContext = NULL;
// 	IDXGISwapChain*         g_pSwapChain = NULL;
//  ID3D11RenderTargetView* g_pRenderTargetView = NULL;
//	ID3D11VertexShader*     g_pVertexShader = NULL;
// 	ID3D11PixelShader*      g_pPixelShader = NULL;
// 	ID3D11InputLayout*      g_pVertexLayout = NULL;
// 	ID3D11Buffer*           g_pVertexBuffer = NULL;
// 	ID3D11Buffer*           g_pIndexBuffer = NULL;
// 	ID3D11Buffer*           g_pConstantBuffer = NULL;
// 	XMMATRIX                g_World;
// 	XMMATRIX                g_View;
// 	XMMATRIX                g_Projection;
	ID3D11Texture2D* pBackBuffer = NULL;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT hr = S_FALSE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return;

	// Create a render target view
	//ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return;


	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);
}

template<typename T>
inline void SafeRelease(T* p)
{
	if (0 != p)
	{
		p->Release();
	}
}

template<typename T>
inline void SafeAddRef(T* p)
{
	if (0 != p)
	{
		p->AddRef();
	}
}

void D3DEnvironment::frameMove(std::uint64_t frameNumber, std::uint64_t elapsed)
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	for (auto visula: m_VisualObjects)
	{
		visula->frameMove(frameNumber, elapsed);
	}
	g_pSwapChain->Present(0, 0);
}

void D3DEnvironment::resize(int width, int height)
{
	if (m_width != width || height != m_height )
	{
		m_width = width;
		m_height = height;
		HRESULT hr = g_pSwapChain->ResizeBuffers(1, width,height, DXGI_FORMAT_R8G8B8A8_UNORM,0);
		if (SUCCEEDED(hr))
		{
			ID3D11RenderTargetView* renderTargetView = 0;
			ID3D11DepthStencilView* depthStencilView = 0;
			g_pImmediateContext->OMGetRenderTargets(1, &renderTargetView, &depthStencilView);

			ID3D11Texture2D* backBuffer = 0;
			hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
			if (SUCCEEDED(hr))
			{
				ID3D11Texture2D* backBuffer = 0;
				hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
				if (SUCCEEDED(hr))
				{
					D3D11_TEXTURE2D_DESC backbufferdesc;
					backBuffer->GetDesc(&backbufferdesc);
					ID3D11RenderTargetView* newRenderTargetView;
					hr = g_pd3dDevice->CreateRenderTargetView(backBuffer, 0, &newRenderTargetView);

					// 				ID3D11Texture2D* depthStencilBuffer = 0;
					// 				ID3D11DepthStencilView* newdepthStencilView = 0;
					// 				UINT multiSample = 4;
					// 				D3D11_TEXTURE2D_DESC dsDesc;
					// 				ZeroMemory(&dsDesc, sizeof(dsDesc));
					// 				dsDesc.Width = width;
					// 				dsDesc.Height = height;
					// 				dsDesc.MipLevels = 1;
					// 				dsDesc.ArraySize = 1;
					// 				dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
					// 				dsDesc.SampleDesc.Count = backbufferdesc.SampleDesc.Count;
					// 				dsDesc.SampleDesc.Quality = 0;
					// 				dsDesc.Usage = D3D11_USAGE_DEFAULT;
					// 				dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
					// 				dsDesc.CPUAccessFlags = 0;
					// 				dsDesc.MiscFlags = 0;
					// 				hr = g_pd3dDevice->CreateTexture2D(&dsDesc, NULL, &depthStencilBuffer);
					// 
					// 				D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
					// 				ZeroMemory(&dsvDesc, sizeof(dsvDesc));
					// 				dsvDesc.Format = dsDesc.Format;
					// 				if (1 == multiSample)
					// 				{
					// 					dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					// 					dsvDesc.Texture2D.MipSlice = 0;
					// 				}
					// 				else
					// 				{
					// 					dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
					// 				}
									//hr = g_pd3dDevice->CreateDepthStencilView(depthStencilBuffer, &dsvDesc, &newdepthStencilView);

					g_pImmediateContext->OMSetRenderTargets(1, &newRenderTargetView, NULL);
					g_pRenderTargetView = newRenderTargetView;

					SafeRelease(backBuffer);
				}

				SafeRelease(renderTargetView);
				SafeRelease(depthStencilView);
			}
		}

	}
}

void D3DEnvironment::addVisualObject(AbstractVisualObjectPtr object, bool needInit /*= true*/)
{
	if (needInit)
	{
		Context con;
		con.g_pd3dDevice = g_pd3dDevice;
		con.g_pImmediateContext = g_pImmediateContext;
		object->inits(con);
	}
	m_VisualObjects.push_back(object);
}
