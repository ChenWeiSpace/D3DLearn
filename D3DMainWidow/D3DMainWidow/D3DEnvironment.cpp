#include "D3DEnvironment.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "AbstractVisualObject.h"
#include "D3DCamera.h"

D3DEnvironment::D3DEnvironment()
{
	m_camera = std::make_shared<D3DCamera>();
}


D3DEnvironment::~D3DEnvironment()
{
	SafeRelease(g_pd3dDevice);
	SafeRelease(g_pImmediateContext);
	SafeRelease(g_pSwapChain);
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
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;


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
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = (HWND)wHid;
	sd.Windowed =  TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT deviceFlag = 0;// D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	deviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
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

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	ID3D11Texture2D*        pDepthStencil = NULL;
	hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr))
		return ;

	
	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return;
	pDepthStencil->Release();

	g_pImmediateContext = immediateContext;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	g_pRenderTargetView->Release();
	g_pDepthStencilView->Release();

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);
	m_width = width;
	m_height = height;

	m_camera->setCameraParam(width, height,0.01,1000);
}


void D3DEnvironment::frameMove(std::uint64_t frameNumber, std::uint64_t elapsed)
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0);

	g_pImmediateContext->VSSetShader(NULL, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 0, NULL);
	g_pImmediateContext->PSSetShader(NULL, NULL, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 0, NULL);
	g_pImmediateContext->IASetInputLayout(NULL);
	g_pImmediateContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	g_pImmediateContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R16_UINT, 0);
	g_pImmediateContext->RSSetState(NULL);
	g_pImmediateContext->OMSetBlendState(NULL, NULL, 0);
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED);

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
			ID3D11Texture2D* backBuffer = 0;
			hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
			if (SUCCEEDED(hr))
			{
				D3D11_TEXTURE2D_DESC backbufferdesc;
				backBuffer->GetDesc(&backbufferdesc);
				ID3D11RenderTargetView* newRenderTargetView;
				hr = g_pd3dDevice->CreateRenderTargetView(backBuffer, 0, &newRenderTargetView);
				if (SUCCEEDED(hr))
				{
					ID3D11Texture2D* depthStencilBuffer = 0;
					ID3D11DepthStencilView* newdepthStencilView = 0;
					UINT multiSample = 4;
					D3D11_TEXTURE2D_DESC dsDesc;
					ZeroMemory(&dsDesc, sizeof(dsDesc));
					dsDesc.Width = width;
					dsDesc.Height = height;
					dsDesc.MipLevels = 1;
					dsDesc.ArraySize = 1;
					dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
					dsDesc.SampleDesc.Count = backbufferdesc.SampleDesc.Count;
					dsDesc.SampleDesc.Quality = 0;
					dsDesc.Usage = D3D11_USAGE_DEFAULT;
					dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
					dsDesc.CPUAccessFlags = 0;
					dsDesc.MiscFlags = 0;
					hr = g_pd3dDevice->CreateTexture2D(&dsDesc, NULL, &depthStencilBuffer);
					if (SUCCEEDED(hr))
					{
						D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
						ZeroMemory(&dsvDesc, sizeof(dsvDesc));
						dsvDesc.Format = dsDesc.Format;
						dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
						dsvDesc.Texture2D.MipSlice = 0;

						hr = g_pd3dDevice->CreateDepthStencilView(depthStencilBuffer, &dsvDesc, &newdepthStencilView);
						if (SUCCEEDED(hr))
						{
							g_pImmediateContext->OMSetRenderTargets(1, &newRenderTargetView, newdepthStencilView);
							g_pRenderTargetView = newRenderTargetView;
							g_pDepthStencilView = newdepthStencilView;

							newRenderTargetView->Release();
							newdepthStencilView->Release();
							depthStencilBuffer->Release();
							backBuffer->Release();

							D3D11_VIEWPORT vp;
							vp.Width = (FLOAT)width;
							vp.Height = (FLOAT)height;
							vp.MinDepth = 0.0f;
							vp.MaxDepth = 1.0f;
							vp.TopLeftX = 0;
							vp.TopLeftY = 0;
							g_pImmediateContext->RSSetViewports(1, &vp);

							m_width = width;
							m_height = height;
							m_camera->setCameraParam(width, height);
						}
					}
				}

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
		object->setCamera(m_camera);
		object->inits(con);
	}
	m_VisualObjects.push_back(object);
}
