// In the name of God
// Ehsan Marufi Azar, 2013

#pragma once;
#ifndef CELLOUTTERGLOW_H
#define CELLOUTTERGLOW_H

#include "DeviceAccessor.h"
#include "CellDirection.h"
#include "RenderStateManagement.h"
#include <d3d9.h>

namespace ArrowMaze
{

	class CCellOutterGlow
		: public Direct3D::CDeviceAccessor
	{
	public:
		CCellOutterGlow();
		~CCellOutterGlow( void );


		void Draw();
		
		void setOpacity( const float& fOpacity );
		inline const float& getOpacity() const { return m_fOpacity; }

		// Static section
		inline static const LPDIRECT3DTEXTURE9 getTexture() { return s_pTexture; }

		static void createStaticDeviceObjects();
		static void releaseStaticDeviceObjects();


	protected:
		float m_fOpacity;
		
		// Texture, IndexBuffer, VertexBuffer and renderStates for the OutterGlows are identical so share 'em all here!
		static LPDIRECT3DTEXTURE9 s_pTexture;
		static LPDIRECT3DINDEXBUFFER9 s_pIndexBuffer;
		static LPDIRECT3DVERTEXBUFFER9 s_pVertexBuffer;
		static Direct3D::CRenderStateManagement* s_pRenderStates;

		static bool s_bClassInnited;

	private:
		D3DMATERIAL9 m_material;

	}; // end of class 'CCelOutterGlow'

} // end of namespace 'ArrowMaze'
#endif // CELLOUTTERGLOW_H