// In the name of God
// Ehsan Marufi Azar, 2013
// v1.0 Feb 2013

#pragma once
#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H


#include <d3dx9.h>

namespace Direct3D
{

	class CBoundingSphere
	{
	public:
		CBoundingSphere( void );
		CBoundingSphere( const CBoundingSphere& copy );
		~CBoundingSphere( void );
	

		// inline getters and setters
		inline const D3DXVECTOR3& getOrigin() const { return m_vOrigin; }
		inline const float& getRadius() const { return m_fRadius; }

		inline void setOrigin( const D3DXVECTOR3& vOrigin ) { m_vOrigin = vOrigin; }
		inline void setRadius( const float& fRadius ) { m_fRadius = fRadius; }

	protected:
		D3DXVECTOR3 m_vOrigin;
		float m_fRadius;

	}; // end of class 'CBoundingSphere'
} // end of namespace 'Direct3D'

#endif // BOUNDINGSPHERE_H