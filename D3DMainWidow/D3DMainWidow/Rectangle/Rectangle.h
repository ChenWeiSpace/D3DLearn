#ifndef Rectangle_H
#define Rectangle_H
#include "../AbstractVisualObject.h"

class DrawRectangle :
	public AbstractVisualObject
{
public:
	DrawRectangle();
	~DrawRectangle();
public:
	void inits(Context& con) override;
	void frameMove(std::uint64_t frameNumber, std::uint64_t elapsed) override;
private:
	ID3D11VertexShader* g_pVertexShader{ NULL };
	ID3D11PixelShader* g_pPixelShader{ NULL };

	ID3D11InputLayout* g_pVertexLayout{ NULL };
	ID3D11Buffer* g_pVertexBuffer{ NULL };
	ID3D11Buffer* g_pIndexBuffer{ NULL };
	ID3D11Buffer* g_pConstantBuffer{ NULL };
	ID3D11RasterizerState* m_pRasterizerState{ NULL };
	ID3D11BlendState* m_pBlendState{NULL};
	D3DXMATRIX    g_World;
};

SHAREDPTR(DrawRectangle);

#endif // !Rectangle_H

