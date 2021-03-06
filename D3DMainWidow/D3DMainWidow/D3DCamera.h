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
	inline const D3DXMATRIX& getWorldMatrix() {
		return m_World;
	}
	inline const D3DXMATRIX& getViewMatrix() {
		return m_View;
	}
	inline const D3DXMATRIX& getProjectionMatrix() {
		return m_Projection;
	}
	inline const D3DXMATRIX& getViewProjectionMatrix() {
		return m_ViewProjection;
	}

	void setWorldPosition(D3DXVECTOR3 pos);
	inline const D3DXVECTOR3 getWorldPosition() {
		return m_worldPos;
	}

	void setRotation(EulerAngle eularAngle);
	inline EulerAngle getRotation() {
		return m_rotation;
	}
	inline D3DXVECTOR3 getHeading() { return m_zVector; };//z
	inline D3DXVECTOR3 getRight() { return m_xVector; };//x
	inline D3DXVECTOR3 getUp() { return m_yVector; };//y
	inline float getZn() { return m_zn; }
	inline float getZf() { return m_zf; }
private:
	void setCameraParam(int width, int height,float zn = 0.01f,float zf = 100.0f);
	void updateMatrix();
	void updateProjectMatrix();
	void updateViewProjectMatrix();

private:
	D3DXMATRIX                m_World;
	D3DXMATRIX                m_View;
	D3DXMATRIX                m_Projection;
	D3DXMATRIX                m_ViewProjection;
	D3DXVECTOR3               m_worldPos{0,0,-10};
	EulerAngle m_rotation;
	int m_width{ 0 };
	int m_height{ 0 };
	float m_zn{ 0.1f };
	float m_zf{ 100.0f };
	D3DXVECTOR3 m_xVector{ 1,0,0 };
	D3DXVECTOR3 m_yVector{ 0,1,0 };
	D3DXVECTOR3 m_zVector{ 0,0,1 };
};
typedef std::shared_ptr<D3DCamera> D3DCameraPtr;
#endif // D3DCamera_h__
