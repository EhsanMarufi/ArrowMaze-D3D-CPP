#include "BaseCamera.h"

using namespace Direct3D;


CBaseCamera::CBaseCamera( const D3DXVECTOR3& pos )
{
	m_Position = pos;
	setDefaultValues();
	updateViewMatrix();
	updateProjectionMatrix();
}


CBaseCamera::~CBaseCamera(void)
{
}


void CBaseCamera::setDefaultValues()
{
	m_TargetPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_UpDirection = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	m_fFOVY = D3DX_PI * 0.25f;
	RECT ClientRect = ::DXUTGetWindowClientRect();
	m_fAspectRatio = (float)ClientRect.right / (float)ClientRect.bottom;
	m_fZn = 1.0f;
	m_fZf = 1000.0f;
}


void CBaseCamera::updateViewMatrix()
{
	::D3DXMatrixLookAtLH( &m_matView, &m_Position, &m_TargetPos, &m_UpDirection );
}


void CBaseCamera::updateProjectionMatrix()
{
	::D3DXMatrixPerspectiveFovLH( &m_matProjection, m_fFOVY, m_fAspectRatio , m_fZn, m_fZf );
}


const D3DXMATRIX& CBaseCamera::applyViewMatrix()
{
	updateViewMatrix();
	THR( s_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView ) );

	return m_matView;
}


const D3DXMATRIX& CBaseCamera::applyProjectionMatrix()
{
	updateProjectionMatrix();
	THR( s_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProjection ) );

	return m_matProjection;
}