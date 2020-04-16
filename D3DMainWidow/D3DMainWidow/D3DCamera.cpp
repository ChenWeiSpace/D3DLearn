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

void D3DCamera::setCameraParam(int width, int height, float zn /*= 0.01f*/, float zf /*= 100.0f*/)
{
	if ( width!= m_width || height != m_height || m_zn != zn || m_zf != zf)
	{
		m_width = width;
		m_height = height;
		m_zn = zn;
		m_zf = zf;
		updateProjectMatrix();
		updateViewProjectMatrix();
	}
}

void D3DCamera::setWorldPosition(D3DXVECTOR3 pos)
{
	if (memcmp(&m_worldPos, &pos, sizeof(XMVECTOR))!=0)
	{
		m_worldPos = pos;
		updateMatrix();
		updateViewProjectMatrix();
	}
}

void D3DCamera::setRotation(EulerAngle eularAngle)
{
	if (m_rotation != eularAngle)
	{
		m_rotation = eularAngle;
		updateMatrix();
		updateViewProjectMatrix();
	}
}

void D3DCamera::updateMatrix()
{
	D3DXMATRIX mCameraRot;
	D3DXMatrixRotationYawPitchRoll(&mCameraRot, D3DXToRadian(m_rotation.heading), D3DXToRadian(m_rotation.pitch), D3DXToRadian(m_rotation.roll));

	D3DXVECTOR3 vWorldUp, vWorldAhead;
	D3DXVECTOR3 vLocalUp = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 vLocalAhead = D3DXVECTOR3(0, 0, 1);

	D3DXVec3TransformCoord(&vWorldUp, &vLocalUp, &mCameraRot);
	D3DXVec3TransformCoord(&vWorldAhead, &vLocalAhead, &mCameraRot);

	D3DXVec3TransformNormal(&m_xVector, &s_xAxis, &mCameraRot);
	D3DXVec3TransformNormal(&m_yVector, &s_yAxis, &mCameraRot);
	D3DXVec3TransformNormal(&m_zVector, &s_zAxis, &mCameraRot);

	//vWorldUp += m_worldPos;
	vWorldAhead += m_worldPos;
	D3DXMatrixLookAtLH(&m_View, &m_worldPos, &vWorldAhead, &vWorldUp);

	D3DXMatrixInverse(&m_World, NULL, &m_View);
}

void D3DCamera::updateProjectMatrix()
{
	D3DXMatrixPerspectiveFovLH(&m_Projection, D3DX_PI / 4, (float)m_width/ (float)m_height, m_zn, m_zf);
}

void D3DCamera::updateViewProjectMatrix()
{
	D3DXMatrixMultiply(&m_ViewProjection, &m_View, &m_Projection);
}
