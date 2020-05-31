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

struct Vetrtex
{
	XMFLOAT3 pos;
};

struct Vetrtex_pos_nor : public Vetrtex
{
	XMFLOAT3 nor;
};

struct Vetrtex_pos_nor_tex :public Vetrtex_pos_nor
{
	XMFLOAT2 tex;
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

struct VertexP_S_T
{
	VertexP_S_T()
	{
		Pos = D3DXVECTOR3(0, 0, 0);
		Nor = D3DXVECTOR3(0, 0, 0);
		Tex = D3DXVECTOR2(0, 0);
	}
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Nor;
	D3DXVECTOR2 Tex;
};

class StaticAbstractVisualObject : public AbstractVisualObject
{
public:
	StaticAbstractVisualObject();
	~StaticAbstractVisualObject();
protected:
	ID3D11VertexShader* g_pVertexShader{ NULL };
	ID3D11InputLayout* g_pVertexLayout{ NULL };
	ID3D11Buffer* g_pVertexBuffer{ NULL };

	ID3D11Buffer* g_pIndexBuffer{ NULL };
	ID3D11Buffer* g_pConstantBuffer{ NULL };
	ID3D11PixelShader* g_pPixelShader{ NULL };
	ID3D11RasterizerState* m_pRasterizerState{ NULL };

	D3DXMATRIX                g_World;
};

#endif // AbstractVisualObject_h__
