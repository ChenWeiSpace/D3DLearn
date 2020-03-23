#ifndef D3DCamera_h__
#define D3DCamera_h__

#include <memory>
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class D3DCamera :  public std::enable_shared_from_this<D3DCamera>
{
public:
	D3DCamera();
	~D3DCamera();
	void initProject(float width,float height);
private:
	XMMATRIX                g_World;
	XMMATRIX                g_View;
	XMMATRIX                g_Projection;
};
typedef std::shared_ptr<D3DCamera> D3DCameraPtr;
#endif // D3DCamera_h__
