#include "BoundingSphere.h"
using namespace Direct3D;

CBoundingSphere::CBoundingSphere(void)
{
	m_vOrigin = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_fRadius = 0.0f;
}


CBoundingSphere::CBoundingSphere( const CBoundingSphere& copy )
{
	m_vOrigin = copy.getOrigin();
	m_fRadius = copy.getRadius();
}


CBoundingSphere::~CBoundingSphere(void)
{
}
