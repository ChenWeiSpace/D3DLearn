#include "../AbstractVisualObject.h"
#ifndef TriangleObject_h__
#define TriangleObject_h__

class TriangleObject : public AbstractVisualObject
{
public:
	TriangleObject();
	~TriangleObject();
public:
	void inits(Context & con) override;
	void frameMove(std::uint64_t frameNumber, std::uint64_t elapsed) override;
 private:
	ID3D11VertexShader*     g_pVertexShader = NULL;
	ID3D11PixelShader*      g_pPixelShader = NULL;
	ID3D11InputLayout*      g_pVertexLayout = NULL;
	ID3D11Buffer*           g_pVertexBuffer = NULL;
	ID3D11Buffer*           g_pConstantBuffer = NULL;
};
#endif // TriangleObject_h__
