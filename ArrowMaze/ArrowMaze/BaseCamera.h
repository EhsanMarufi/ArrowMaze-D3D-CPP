// In the name of God
// Ehsan Marufi Azar, 2012 (c)
// Simple camera class, could be used as a base for other purposes.
// v1.2 Jan 2013

#pragma once
#ifndef BASECAMERA_H
#define BASECAMERA_H

#include "d3dx9.h"
#include "DeviceAccessor.h"
#include "hr_Exception.h"
#include "Assertion.h"
#include "DXUT\Core\DXUT.h"

namespace Direct3D
{

	class CBaseCamera
		: public CDeviceAccessor
	{
	public:
		CBaseCamera( const D3DXVECTOR3& pos );
		~CBaseCamera( void );


		const D3DXMATRIX& applyViewMatrix();
		const D3DXMATRIX& applyProjectionMatrix();


		// inline getters and setters
		inline const D3DXVECTOR3& getCameraPositon() const			{ return m_Position; }
		inline const D3DXVECTOR3& getCameraTargetPosition() const	{ return m_TargetPos; }
		inline const D3DXVECTOR3& getCameraUpDirection() const		{ return m_UpDirection; }
		inline const float& getFieldOfViewY() const					{ return m_fFOVY; }
		inline const float& getAspectRatio() const					{ return m_fAspectRatio; }
		inline const float& getFarZClipping() const					{ return m_fZf; }
		inline const float& getNearZClipping() const				{ return m_fZn; }

		inline const D3DXMATRIX& getCurrentViewMatrix() const		{ return m_matView; }
		inline const D3DXMATRIX& getCurrentProjectionMatrix() const	{ return m_matProjection; }

		inline void setCameraPositon( const D3DXVECTOR3& pos )				{ m_Position = pos; }
		inline void setCameraTargetPosition( const D3DXVECTOR3& targetPos )	{ m_TargetPos = targetPos; }
		inline void setCameraUpDirection( const D3DXVECTOR3& upDir )		{ m_UpDirection = upDir; }
		inline void setFieldOfViewY( const float& fovY )					{ m_fFOVY = fovY; }
		inline void setAspectRatio( const float& AspectRatio )				{ m_fAspectRatio = AspectRatio; }
		inline void setFarZClipping( const float& Zf )						{ m_fZf = Zf; }
		inline void setNearZClipping( const float& Zn )						{ m_fZn = Zn; }


	protected:
		D3DXVECTOR3 m_Position;
		D3DXVECTOR3 m_TargetPos;
		D3DXVECTOR3 m_UpDirection;

		float m_fFOVY;
		float m_fAspectRatio;
		float m_fZn, m_fZf;

		D3DXMATRIX m_matView, m_matProjection;


	private:
		void setDefaultValues();
		void updateViewMatrix();
		void updateProjectionMatrix();

	}; // end of class 'CBaseCamera'
} // end of namespace 'Direct3D'
#endif // BASECAMERA_H