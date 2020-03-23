#ifndef CubeObject_h__
#define CubeObject_h__

#include <memory>
#include "../AbstractVisualObject.h"

class CubeObject : public AbstractVisualObject
{
public:
	CubeObject();
	~CubeObject();
public:
	void inits(Context & con) override;
	void frameMove(std::uint64_t frameNumber, std::uint64_t elapsed) override;
private:
	ID3D11VertexShader*     g_pVertexShader{ NULL };
	ID3D11InputLayout*      g_pVertexLayout{ NULL };
	ID3D11Buffer*           g_pVertexBuffer{ NULL };
	ID3D11Buffer*           g_pIndexBuffer{ NULL };
	ID3D11Buffer*           g_pConstantBuffer{ NULL };
	ID3D11PixelShader*      g_pPixelShader{ NULL };
	ID3D11PixelShader*      g_pPixelShaderSolid{ NULL };

// 	ID3D11Device*           g_pd3dDevice{ NULL };
// 	ID3D11DeviceContext*    g_pImmediateContext{ NULL };
};

#endif // CubeObject_h__
