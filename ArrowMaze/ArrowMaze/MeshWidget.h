// In the name of God
// Ehsan Marufi Azar, 2012 (c)
// v1.2 Feb 2013

#pragma once
#ifndef MESHWIDGET_H
#define MESHWIDGET_H

#include <Windows.h>
#include "widget.h"
#include "MeshFile.h"
#include "hr_Exception.h"
#include "Assertion.h"
#include "safe.h"

namespace Direct3D
{

#define MESHWIDGETFLAG_RANDOMSOLIDCOLOR (0x1UL)
#define MESHWIDGETFLAG_NOFLAG (0x0UL)


	class CMeshWidget :
		public CWidget, public CMeshFile
	{
	public:
		CMeshWidget(void);
		//CMeshWidget(const CMeshWidget& copy);
		CMeshWidget( LPCSTR szMeshFileName, const DWORD& dwOptions = D3DXMESH_MANAGED, const DWORD& dwMeshWidgetFlags = 0 );
		~CMeshWidget(void);

		void Draw();


		// inline getters, and setters
		inline const D3DXCOLOR* getSolidColor() const	{ return m_pSolidColor; }
		inline D3DMATERIAL9* getMaterial() const		{ return m_pMaterial; }
		inline D3DLIGHT9* getLight() const				{ return m_pLight; }

		void setSolidColor( const D3DCOLOR& solidColor );
		void setMaterial( const D3DMATERIAL9& material );
		void setLight( const D3DLIGHT9& light );


		// Other methods
		inline void dontUseLight() { SAFE_DELETE( m_pLight ); }
		inline void dontUseMaterial() { SAFE_DELETE( m_pMaterial ); }

	protected:
		D3DXCOLOR* m_pSolidColor;
		D3DMATERIAL9* m_pMaterial;
		D3DLIGHT9* m_pLight;

	}; // end of class 'CMeshWidget'
} // end of namespace 'Direct3D'
#endif // MESHWIDGET_H