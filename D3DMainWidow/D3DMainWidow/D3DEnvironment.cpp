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
#define sizeofarray(a)	(sizeof(a)/sizeof(a[0]))

void D3DEnvironment::buildDevice(int wHid, int width, int height)
{

	ID3D11Device* device = 0;
	ID3D11DeviceContext* immediateContext = 0;
	IDXGISwapChain* swapChain = 0;
	ID3D11Texture2D* backBuffer = 0;
	ID3D11Texture2D* depthStencilBuffer = 0;
	ID3D11RenderTargetView* renderTargetView = 0;
	ID3D11DepthStencilView* depthStencilView = 0;
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};
	D3D_FEATURE_LEVEL featureLevel;


	//IDXGIFactory1 *pDXGIFactory(NULL);
	//HRESULT hRes = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);

	//if(hRes != S_OK || pDXGIFactory == NULL)
	//	return 0 ;

	// Search for a PerfHUD adapter.  
	UINT nAdapter = 0;
	IDXGIAdapter* adapter = NULL;
	IDXGIAdapter* selectedAdapter = NULL;
	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = (HWND)wHid;
	sd.Windowed =  TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT deviceFlag = 0;// D3D11_CREATE_DEVICE_BGRA_SUPPORT;
//#ifdef _DEBUG
//	deviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
	HRESULT hr = D3D11CreateDeviceAndSwapChain(selectedAdapter, driverType,
		0, deviceFlag, featureLevels, sizeofarray(featureLevels),
		D3D11_SDK_VERSION, &sd,
		&swapChain, &device, &featureLevel, &immediateContext);
	if (FAILED(hr))
	{
		return;
	}
	g_pSwapChain = swapChain;

	ID3D11Texture2D* pBackBuffer = 0;
	 hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return;
	g_pd3dDevice = device;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return;

	g_pImmediateContext = immediateContext;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
	g_pRenderTargetView->Release();

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
	g_pSwapChain->Present(1, 0);
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
		con.width = m_width;
		con.height= m_height;
		object->setDeviceAndContext(g_pd3dDevice, g_pImmediateContext);
		object->inits(con);
	}
	m_VisualObjects.push_back(object);
}
