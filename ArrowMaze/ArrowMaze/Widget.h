// In the name of God
// Ehsan Marufi Azar, 2012 (c)
// v1.2 Jan 2013

#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include <Windows.h>
#include <d3dx9.h>

#include "DeviceAccessor.h"
#include "RenderStateManagement.h"
#include "hr_Exception.h"
#include "safe.h"

namespace Direct3D
{

	//Capture Transform
#define CT_NONE  (0x0)
#define CT_WORLD (0x1)		//1 << 0
#define CT_VIEW  (0x2)		//1 << 1
#define CT_PROJECTION (0x4)	//1 << 2

	//TODO: ADD THE CAPABILITY OF SPECIFYING THE PIVOT POINT

	class CWidget
		: public CDeviceAccessor
	{
	public:
		CWidget();
		CWidget( const CWidget &copy );
		~CWidget(void);

		// inline getters and setters
		inline const D3DXVECTOR3& getPosition() const			{ return m_Position; }
		inline const D3DXVECTOR3& getRotation() const			{ return m_Rotation; }
		inline const D3DXVECTOR3& getScale()    const			{ return m_Scale; }
		inline const LPD3DXMATRIX getRotationMatrix() const		{ return m_pmatRotation; }
		
		inline void setRotationMatrix( const D3DXMATRIX& mat )	{ ASSERT_MEMORYALLOCATION( m_pmatRotation = new D3DXMATRIX(mat) ); }
		inline void setRotation( const D3DXVECTOR3& rotation )	{ m_Rotation = rotation; }
		inline const D3DXVECTOR3& setPosition( const D3DXVECTOR3& pos ) { return m_Position = pos; }
		inline void setScale( const D3DXVECTOR3& scale )		{ m_Scale = scale; }

		inline const D3DXVECTOR3& getTransformationCenter() const { return m_vecTransformationCenter; }
		inline void setTransformationCenter( const D3DXVECTOR3& vecTransformationCenter ) { m_vecTransformationCenter = vecTransformationCenter; }

		// read-only properties
		inline const LPD3DXMATRIX getCaptruedWordTransformation() const { return m_pmatCapturedWorldTransformation; }
		inline const LPD3DXMATRIX getCaptruedViewTransformation() const { return m_pmatCapturedViewTransformation; }
		inline const LPD3DXMATRIX getCaptruedProjectionTransformation() const { return m_pmatCapturedProjectionTransformation; }
		inline CRenderStateManagement* getStateBlockManager() const { return m_pRenderStatesManager; }

		
		// 
		void applyTransformations( const bool& bCaptureDeviceTransformations = true );

		void captureDeviceTransformations( const DWORD& dwCaptureTransform = CT_WORLD | CT_VIEW | CT_PROJECTION );
		void retrieveDeviceTransformations( const DWORD& dwRetrieveTransform = CT_WORLD | CT_VIEW | CT_PROJECTION );

		void captureDeviceStates();


	protected:
		D3DXVECTOR3 m_Position;
		D3DXVECTOR3 m_Rotation;
		D3DXVECTOR3 m_Scale;
		LPD3DXMATRIX m_pmatRotation;

		LPD3DXMATRIX m_pmatCapturedWorldTransformation;
		LPD3DXMATRIX m_pmatCapturedViewTransformation;
		LPD3DXMATRIX m_pmatCapturedProjectionTransformation;

		CRenderStateManagement* m_pRenderStatesManager;

		D3DXVECTOR3 m_vecTransformationCenter;

	}; // end of class 'CWidget'
} // end of namespace 'Direct3D'
#endif // WIDGET_H