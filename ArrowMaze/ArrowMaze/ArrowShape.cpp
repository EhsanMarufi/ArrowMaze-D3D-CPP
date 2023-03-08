#include "ArrowShape.h"
#include "ArrowsShapes.h"
#include "SquareShape.h"

using namespace ArrowMaze;
using namespace Direct3D;

bool CArrowShape::s_bClassInnited = false;
LPDIRECT3DTEXTURE9 CArrowShape::s_pArrowTexture = 0;
LPDIRECT3DINDEXBUFFER9 CArrowShape::s_pIndexBuffer = 0;
CRenderStateManagement* CArrowShape::s_pRenderStates = 0;


CArrowShape::CArrowShape( const CellDirection& direction )
{
	ASSERT( s_bClassInnited, "Please initialize the class-level static data first!" );

	// get the shared vertex buffer
	m_pVertexBuffer = CArrowShapesVertexBuffers::getVertexBuffer( direction );
}


CArrowShape::~CArrowShape(void)
{
	// don't release the buffer here, coz it's shared! It has its own memory management.
	m_pVertexBuffer = 0;
}


void CArrowShape::createStaticDeviceObjects()
{
	CArrowShape::releaseStaticDeviceObjects();
	ASSERT_D3DDEVICE( s_pD3DDevice );

	THR( ::D3DXCreateTextureFromFileA( s_pD3DDevice, "Media/Textures/Arrow_ClockWise Pointing Arrows.png", &s_pArrowTexture ) );

	// Creating and feeding the shared indexBuffer
	THR( s_pD3DDevice->CreateIndexBuffer( 6 * sizeof( WORD ), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &s_pIndexBuffer, 0 ) );
	WORD* pData = 0;
	THR( s_pIndexBuffer->Lock( 0, 0, (void**)&pData, 0 ) );
	for ( UINT ui = 0; ui < 6; ui++ )
		pData[ ui ] = CSquareShape::getIndexBufferArray()[ ui ];
	THR( s_pIndexBuffer->Unlock() );


	// Creating the Render States
	ASSERT_MEMORYALLOCATION( s_pRenderStates = new CRenderStateManagement() );
	Render_State arrRenderStates[] = {
		//{ D3DRS_CULLMODE, D3DCULL_NONE },
		{ D3DRS_ZENABLE, FALSE },
		{ D3DRS_ALPHABLENDENABLE, TRUE },
		{ D3DRS_SRCBLEND, D3DBLEND_SRCALPHA },
		{ D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA }
	};
	Texture_Stage_State arrTextureStates[] = { // For stage 0
		{ D3DTSS_COLORARG1, D3DTA_TEXTURE },
		{ D3DTSS_COLOROP, D3DTOP_SELECTARG1 },
		{ D3DTSS_ALPHAARG1, D3DTA_TEXTURE },
		{ D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 }
	};
	s_pRenderStates->setStates( arrRenderStates, NUM_OF( arrRenderStates ) );
	s_pRenderStates->setStates( 0, arrTextureStates, NUM_OF( arrTextureStates ) );
	s_pRenderStates->createStateBlock();

	s_bClassInnited = true;
}


void CArrowShape::releaseStaticDeviceObjects()
{
	SAFE_RELEASE( s_pArrowTexture );
	SAFE_RELEASE( s_pIndexBuffer );

	SAFE_DELETE( s_pRenderStates );

	s_bClassInnited = false;
}


void CArrowShape::Draw()
{
	ASSERT_D3DDEVICE( s_pD3DDevice );

	// setting vertex and index buffer on the device
	THR( s_pD3DDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(SquareShapeVertex) ) );
	THR( s_pD3DDevice->SetIndices( s_pIndexBuffer ) );

	// setting texture of ArrowShapes
	THR( s_pD3DDevice->SetTexture( 0, s_pArrowTexture ) );

	// setting the render states on the device
	s_pRenderStates->startUsingStateBlock();
	
	THR( s_pD3DDevice->SetFVF( SquareShapeVertex::FVF ) );
	THR( s_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 ) );
	
	s_pRenderStates->endUsingStateBlock();
}