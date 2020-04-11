#include "D3DCamera.h"



D3DCamera::D3DCamera()
{
	m_World = *(D3DXMATRIX*)&XMMatrixIdentity();
	m_Projection = *(D3DXMATRIX*)&XMMatrixIdentity();

	m_Projection = *(D3DXMATRIX*)&XMMatrixIdentity();
	m_ViewProjection = *(D3DXMATRIX*)&XMMatrixIdentity();
	updateMatrix();
}


D3DCamera::~D3DCamera()
{
}

void D3DCamera::setProject(int width, int height)
{
	if ( width!= m_width || height != m_height )
	{
		m_width = width;
		m_height = height;
		updateProjectMatrix();
		updateViewProjectMatrix();
	}
}

void D3DCamera::setWorldPosition(D3DXVECTOR3 pos)
{
	if (memcmp(&m_worldPos, &pos, sizeof(XMVECTOR))!=0)
	{
		updateMatrix();
		updateViewProjectMatrix();
	}
}

void D3DCamera::setRotation(EulerAngle eularAngle)
{
	if (m_rotation == eularAngle)
	{
		m_rotation = eularAngle;
		updateMatrix();
		updateViewProjectMatrix();
	}
}

EulerAngle D3DCamera::getRotation()
{
	return m_rotation;
}

void D3DCamera::updateMatrix()
{
	D3DXMATRIX mCameraRot;
	D3DXMatrixRotationYawPitchRoll(&mCameraRot, m_rotation.heading, m_rotation.pitch, m_rotation.roll);

	D3DXVECTOR3 vWorldUp, vWorldAhead;
	D3DXVECTOR3 vLocalUp = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 vLocalAhead = D3DXVECTOR3(0, 0, 1);

	D3DXVec3TransformCoord(&vWorldUp, &vLocalUp, &mCameraRot);
	D3DXVec3TransformCoord(&vWorldAhead, &vLocalAhead, &mCameraRot);


	vWorldUp += m_worldPos;
	vWorldAhead += m_worldPos;
	D3DXMatrixLookAtLH(&m_View, &m_worldPos, &vWorldAhead, &vWorldUp);

	D3DXMatrixInverse(&m_World, NULL, &m_View);
}

void D3DCamera::updateProjectMatrix()
{
	D3DXMatrixPerspectiveFovLH(&m_Projection, D3DX_PI / 4, (float)m_width/ (float)m_height, 0.01f, 100.0f);
}

void D3DCamera::updateViewProjectMatrix()
{
	D3DXMatrixMultiply(&m_ViewProjection, &m_View, &m_Projection);
}

const D3DXMATRIX& D3DCamera::getWorldMatrix()
{
	// TODO: 在此处插入 return 语句
	return m_World;
}

const D3DXMATRIX& D3DCamera::getViewMatrix()
{
	// TODO: 在此处插入 return 语句
	return m_View;
}

const D3DXMATRIX& D3DCamera::getProjectionMatrix()
{
	// TODO: 在此处插入 return 语句
	return m_Projection;
}

const D3DXMATRIX& D3DCamera::getViewProjectionMatrix()
{
	// TODO: 在此处插入 return 语句
	return m_ViewProjection;
}

const D3DXVECTOR3 D3DCamera::getWorldPosition()
{
	return m_worldPos;
}
