#include "MeshFile.h"

using namespace Direct3D;


CMeshFile::CMeshFile()
{
	m_pMesh = 0;
	m_pMeshMaterialsBuffer = 0;
	m_pAdjacency = 0;
	m_pEffectInstances = 0;
	m_pMtrls = 0;
	m_pMeshTexture = 0;
	m_pMesh = 0;
	m_bHasTexture = false;
	m_dwCreationOptions = 0;
	m_dwMeshMaterialCount = 0;

	m_bDrawFromOrigin = false;
}


CMeshFile::CMeshFile( LPCSTR szMeshFileName, const DWORD& dwOptions )
{
	m_pMesh = 0;
	m_pMeshMaterialsBuffer = 0;
	m_pAdjacency = 0;
	m_pEffectInstances = 0;
	m_pMtrls = 0;
	m_pMeshTexture = 0;
	m_pMesh = 0;
	m_bHasTexture = false;
	m_dwCreationOptions = dwOptions;

	m_bDrawFromOrigin = false;

	// Loading the mesh file
	THRM( D3DXLoadMeshFromXA(	szMeshFileName, m_dwCreationOptions, s_pD3DDevice,
								&m_pAdjacency, &m_pMeshMaterialsBuffer, &m_pEffectInstances,
								&m_dwMeshMaterialCount, &m_pMesh ),
								"D3DXLoadMeshFromX() - FAILED." );

	m_pMtrls = (D3DXMATERIAL*)m_pMeshMaterialsBuffer->GetBufferPointer();

	std::string strPath = "Media/Textures/", FullPath;
	for ( UINT i = 0; i < m_dwMeshMaterialCount; i++ )
	{
		m_pMtrls[i].MatD3D.Ambient = m_pMtrls[i].MatD3D.Diffuse;

		if( m_pMtrls[i].pTextureFilename ) 
		{
			FullPath.clear();
			FullPath += strPath;
			FullPath += m_pMtrls[i].pTextureFilename;

			THRM(::D3DXCreateTextureFromFileA( s_pD3DDevice, FullPath.c_str(), &m_pMeshTexture ),
				"D3DXCreateTextureFromFile() - FAILED." );
			m_bHasTexture = true;
		}
	}


	computeBoundingSphere();
	
	// compute the TranslationToOrigin matrix
	const D3DXVECTOR3& vCenter = m_BoundingSphere.getOrigin();
	D3DXMatrixTranslation( &m_matTranslationToOrigin, -vCenter.x, -vCenter.y, -vCenter.z );
}


void CMeshFile::getMesh( LPD3DXMESH *ppMesh, LPD3DXBUFFER* ppMeshMaterialsBuffer,
	LPD3DXBUFFER* ppAdjacancy,	LPD3DXBUFFER* ppEffectInstances )
{
	*ppMesh = m_pMesh;
	*ppMeshMaterialsBuffer = m_pMeshMaterialsBuffer;
	*ppAdjacancy = m_pAdjacency;
	*ppEffectInstances = m_pEffectInstances;
}


void CMeshFile::computeBoundingSphere()
{
	// Obtain the declaration
    D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];

	THR( m_pMesh->GetDeclaration( decl ) );
	LPVOID pVB = 0;
	THR( m_pMesh->LockVertexBuffer( D3DLOCK_READONLY, &pVB ) );
	
	// Compute the bounding sphere
	D3DXVECTOR3 vCtr;
	float fRadius = 0.0f;
    UINT uStride = D3DXGetDeclVertexSize( decl, 0 );
    THR( D3DXComputeBoundingSphere( (const D3DXVECTOR3 *)pVB, m_pMesh->GetNumVertices(),
                                    uStride, &vCtr, &fRadius ) );
    m_pMesh->UnlockVertexBuffer();

	// storing in the object
	m_BoundingSphere.setOrigin( vCtr );
	m_BoundingSphere.setRadius( fRadius );
}


HRESULT CMeshFile::Draw()
{
	if ( m_bHasTexture )
	{
		THR( s_pD3DDevice->SetTexture(0, m_pMeshTexture) );
		THR( s_pD3DDevice->SetTexture(1, m_pMeshTexture) );
	}

	if ( m_bDrawFromOrigin )
	{
		// Translate the mesh to its center point
		D3DXMATRIX matTranslation, matCurrentWorldMatrix;
		THR( s_pD3DDevice->GetTransform( D3DTS_WORLD, &matCurrentWorldMatrix ) );
		D3DXMatrixMultiply( &matTranslation, &m_matTranslationToOrigin, &matCurrentWorldMatrix );
		THR( s_pD3DDevice->SetTransform( D3DTS_WORLD, &matTranslation ) );
	}

	return m_pMesh->DrawSubset( 0 );
}


CMeshFile::~CMeshFile(void)
{
	SAFE_RELEASE( m_pAdjacency );
	SAFE_RELEASE( m_pMeshMaterialsBuffer );
	SAFE_RELEASE( m_pEffectInstances );
	SAFE_RELEASE( m_pMeshTexture );
	SAFE_RELEASE( m_pMesh );
}
