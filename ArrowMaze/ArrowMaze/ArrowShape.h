// In the name of God
// Ehsan Marufi Azar, 2013

#pragma once
#ifndef ARROWSHAPE_H
#define ARROWSHAPE_H

#include "DeviceAccessor.h"
#include "CellDirection.h"
#include "RenderStateManagement.h"
#include <d3d9.h>


namespace ArrowMaze
{
	#define ARROWTEXTURE_OBJECTSCOUNT (9)

	class CArrowShape
		: public Direct3D::CDeviceAccessor
	{
	public:
		CArrowShape( const CellDirection& direction );
		~CArrowShape( void );


		void Draw();
		

		// Static section
		inline static const LPDIRECT3DTEXTURE9 getTexture() { return s_pArrowTexture; }

		static void createStaticDeviceObjects();
		static void releaseStaticDeviceObjects();


	protected:

		// All 9 kinds of ArrowShape's vertex buffer (LEFT, RIGHT, UP,...) are differed only by their texture coordinations,
		// and there are limited Directions; nonetheless there can be much more cells, so it's a good idea (GPU-memory wise) to use shared ones!
		
		// Texture, IndexBuffer and renderStates for the ArrowShapes are identical so share 'em all here!
		static LPDIRECT3DTEXTURE9 s_pArrowTexture;
		static LPDIRECT3DINDEXBUFFER9 s_pIndexBuffer;
		static Direct3D::CRenderStateManagement* s_pRenderStates;

		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;


		static bool s_bClassInnited;

	}; // end of class 'CArrowShape'
} // end of namespace 'ArrowMaze'
#endif // ARROWSHAPE_H