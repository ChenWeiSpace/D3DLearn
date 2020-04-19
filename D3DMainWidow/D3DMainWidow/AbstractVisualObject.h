#ifndef AbstractVisualObject_h__
#define AbstractVisualObject_h__
#include <string>
#include <memory>
#include <d3dx9.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>


#define SHAREDPTR(OBJ) typedef std::shared_ptr<OBJ> OBJ##Ptr

struct ID3D11Device;
struct ID3D11DeviceContext;
typedef std::shared_ptr<class D3DCamera> D3DCameraPtr;

struct Context
{
	ID3D11Device*           g_pd3dDevice{ NULL };
	ID3D11DeviceContext*    g_pImmediateContext{ NULL };
	float width{0};
	float height{0};
};

std::string getResouseDir();
class AbstractVisualObject : public std::enable_shared_from_this<AbstractVisualObject>
{
public:
	AbstractVisualObject();
	~AbstractVisualObject();
	HRESULT CompileShaderFromFile(std::string szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
public:
	virtual void inits(Context & con) = 0;
	virtual void frameMove(std::uint64_t frameNumber, std::uint64_t elapsed) = 0;
	void setDeviceAndContext(ID3D11Device*           device, ID3D11DeviceContext*    context);
	void setCamera(D3DCameraPtr camera);
protected:
	ID3D11Device*           g_pd3dDevice{ NULL };
	ID3D11DeviceContext*    g_pImmediateContext{ NULL };
	D3DCameraPtr m_camera;
};
typedef std::shared_ptr<AbstractVisualObject> AbstractVisualObjectPtr;
#endif // AbstractVisualObject_h__
