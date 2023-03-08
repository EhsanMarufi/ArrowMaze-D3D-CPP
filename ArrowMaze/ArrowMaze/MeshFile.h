// In the name of God
// Ehsan Marufi Azar, 2013 (c)
// v1.4 Feb 2013

#pragma once
#ifndef MESHFILE_H
#define MESHFILE_H

#include <Windows.h>
#include <d3dx9mesh.h>

#include "DeviceAccessor.h"
#include "BoundingSphere.h"
#include "hr_Exception.h"
#include "Assertion.h"
#include "safe.h"

namespace Direct3D
{

	class CMeshFile
		: public CDeviceAccessor
	{
	public:
		CMeshFile();
		CMeshFile( LPCSTR szMeshFileName, const DWORD& dwOptions = D3DXMESH_MANAGED );
		~CMeshFile(void);

		HRESULT Draw();

		void getMesh( LPD3DXMESH* ppMesh, LPD3DXBUFFER* ppMeshMaterialsBuffer = 0,
			LPD3DXBUFFER* ppAdjacancy = 0,	LPD3DXBUFFER* ppEffectInstances = 0 );


		// inline getters and setters
		inline void getTexture( LPDIRECT3DTEXTURE9* ppTexture ) const { *ppTexture = m_pMeshTexture; }
		inline void setTexture( LPDIRECT3DTEXTURE9 pTexture ) { m_pMeshTexture = pTexture; }

		inline const bool& isDrawingFromOrigin() const { return m_bDrawFromOrigin; }
		inline void setDrawingFromOrigin( const bool& val ) { m_bDrawFromOrigin = val; }

		inline const CBoundingSphere& getBoundingSphere() const { return m_BoundingSphere; }


	protected:
		ID3DXMesh* m_pMesh;
		bool m_bHasTexture;
		bool m_bDrawFromOrigin;
		CBoundingSphere m_BoundingSphere;

		void computeBoundingSphere();

	private:
		ID3DXBuffer *m_pMeshMaterialsBuffer;
		ID3DXBuffer *m_pAdjacency;
		ID3DXBuffer *m_pEffectInstances;
		D3DXMATERIAL *m_pMtrls;
		IDirect3DTexture9 *m_pMeshTexture;
		DWORD m_dwMeshMaterialCount;
		DWORD m_dwCreationOptions;

		D3DXMATRIX m_matTranslationToOrigin;

	}; // end of class 'CMeshFile'
} // end of namespace 'Direct3D'
#endif // MESH_H