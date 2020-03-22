#ifndef D3DEnvironment_h__
#define D3DEnvironment_h__

#include <memory>
#include <vector>
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
typedef std::shared_ptr<class AbstractVisualObject> AbstractVisualObjectPtr;

class D3DEnvironment : public std::enable_shared_from_this<D3DEnvironment>
{
	
public:
	D3DEnvironment();
	~D3DEnvironment();
	void buildDevice(int wHid,int width,int height);
	void frameMove(std::uint64_t frameNumber, std::uint64_t elapsed);
	void resize(int width, int height);
	void addVisualObject(AbstractVisualObjectPtr object,bool needInit = true);
private:
	int m_width{0};
	int m_height{0};

	ID3D11Device*           g_pd3dDevice{NULL};
	ID3D11DeviceContext*    g_pImmediateContext { NULL};
	IDXGISwapChain*         g_pSwapChain { NULL};
	ID3D11RenderTargetView* g_pRenderTargetView { NULL};

	std::vector<AbstractVisualObjectPtr> m_VisualObjects;
};
typedef std::shared_ptr<D3DEnvironment> D3DEnvironmentPtr;
#endif // D3DEnvironment_h__
