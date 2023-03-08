#include "MeshWidget.h"

using namespace Direct3D;


CMeshWidget::CMeshWidget( void )
	: CWidget(), CMeshFile()
{
	m_pSolidColor = 0;
}


CMeshWidget::CMeshWidget( LPCSTR szMeshFileName, const DWORD& dwOptions, const DWORD& dwFlags )
	: CWidget(),
	CMeshFile( szMeshFileName, dwOptions )
{
	m_pSolidColor = 0;

	if ( dwFlags & MESHWIDGETFLAG_RANDOMSOLIDCOLOR )
	{
		::srand( ::timeGetTime() );
		#define RND_COLORVALUE ( ::rand() % 255 )
		setSolidColor( D3DCOLOR_XRGB(RND_COLORVALUE, RND_COLORVALUE, RND_COLORVALUE) );
		#undef RND_COLORVALUE
	}

	m_pMaterial = 0;
	m_pLight = 0;
}


CMeshWidget::~CMeshWidget(void)
{
	SAFE_DELETE( m_pSolidColor );
	SAFE_DELETE( m_pMaterial );
	SAFE_DELETE( m_pLight );
}


void CMeshWidget::setMaterial( const D3DMATERIAL9& material )
{
	SAFE_DELETE( m_pMaterial );
	ASSERT_MEMORYALLOCATION( m_pMaterial = new D3DMATERIAL9(material) );
}


void CMeshWidget::setLight( const D3DLIGHT9& light )
{
	SAFE_DELETE( m_pLight );
	ASSERT_MEMORYALLOCATION( m_pLight = new D3DLIGHT9(light) );
}


void CMeshWidget::Draw()
{
	CWidget::applyTransformations();

	getStateBlockManager()->startUsingStateBlock();

	if ( m_pMaterial )
		THR( CMeshFile::s_pD3DDevice->SetMaterial(m_pMaterial) );

	if ( m_pLight )
	{
		THR( CMeshFile::s_pD3DDevice->LightEnable(0, TRUE) );
		THR( CMeshFile::s_pD3DDevice->SetLight(0, m_pLight) );
	}

	CMeshFile::Draw();
	getStateBlockManager()->endUsingStateBlock();
	CWidget::retrieveDeviceTransformations();
}


void CMeshWidget::setSolidColor( const D3DCOLOR& solidColor )
{
	struct vertexPosDiffuse { float x, y, z; D3DCOLOR diffuse; };

	vertexPosDiffuse *pData;
	DWORD vertCount = m_pMesh->GetNumVertices();
	
	THR(m_pMesh->LockVertexBuffer( 0, (LPVOID*)&pData) );
	for ( DWORD dw = 0; dw < vertCount; dw++ )
		pData[ dw ].diffuse = solidColor;

	THR( m_pMesh->UnlockVertexBuffer() );
	
	ASSERT_MEMORYALLOCATION( m_pSolidColor = new D3DXCOLOR(solidColor) );
}