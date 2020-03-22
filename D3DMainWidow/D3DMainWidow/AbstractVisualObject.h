#ifndef AbstractVisualObject_h__
#define AbstractVisualObject_h__
#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;

struct Context
{
	ID3D11Device*           g_pd3dDevice{ NULL };
	ID3D11DeviceContext*    g_pImmediateContext{ NULL };
};

class AbstractVisualObject : public std::shared_ptr<AbstractVisualObject>
{
public:
	AbstractVisualObject();
	~AbstractVisualObject();
public:
	virtual void inits(Context & con) = 0;
	virtual void frameMove(std::uint64_t frameNumber, std::uint64_t elapsed) = 0;
};
typedef std::shared_ptr<AbstractVisualObject> AbstractVisualObjectPtr;
#endif // AbstractVisualObject_h__
