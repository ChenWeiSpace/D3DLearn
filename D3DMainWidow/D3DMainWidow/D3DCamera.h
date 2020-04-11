#ifndef D3DCamera_h__
#define D3DCamera_h__

#include <memory>
#include <d3dx9.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include "HyacinthUntity.h"
class D3DCamera :  public std::enable_shared_from_this<D3DCamera>
{
	friend class D3DEnvironment;
public:
	D3DCamera();
	~D3DCamera();
public:
	const D3DXMATRIX& getWorldMatrix();
	const D3DXMATRIX& getViewMatrix();
	const D3DXMATRIX& getProjectionMatrix();
	const D3DXMATRIX& getViewProjectionMatrix();

	void setWorldPosition(D3DXVECTOR3 pos);
	const D3DXVECTOR3 getWorldPosition();
	void setRotation(EulerAngle eularAngle);
	EulerAngle getRotation();

private:
	void setProject(int width, int height);
	void updateMatrix();
	void updateProjectMatrix();
	void updateViewProjectMatrix();

private:
	D3DXMATRIX                m_World;
	D3DXMATRIX                m_View;
	D3DXMATRIX                m_Projection;
	D3DXMATRIX                m_ViewProjection;
	D3DXVECTOR3               m_worldPos{0,0,-30};
	EulerAngle m_rotation;
	int m_width{ 0 };
	int m_height{ 0 };
};
typedef std::shared_ptr<D3DCamera> D3DCameraPtr;
#endif // D3DCamera_h__
