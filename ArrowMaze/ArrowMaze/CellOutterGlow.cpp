#include "CellOutterGlow.h"
#include "ConstantValues.h"
#include "SquareShape.h"

using namespace ArrowMaze;
using namespace Direct3D;

bool CCellOutterGlow::s_bClassInnited = false;
LPDIRECT3DTEXTURE9 CCellOutterGlow::s_pTexture = 0;
LPDIRECT3DINDEXBUFFER9 CCellOutterGlow::s_pIndexBuffer = 0;
CRenderStateManagement* CCellOutterGlow::s_pRenderStates = 0;
LPDIRECT3DVERTEXBUFFER9 CCellOutterGlow::s_pVertexBuffer = 0;


CCellOutterGlow::CCellOutterGlow()
{
	m_material = D3DConstants::WHITE_MTRL;
	setOpacity( 1.0f );
}


CCellOutterGlow::~CCellOutterGlow()
{
}


void CCellOutterGlow::createStaticDeviceObjects()
{
	CCellOutterGlow::releaseStaticDeviceObjects();
	ASSERT_D3DDEVICE( s_pD3DDevice );

	THR( ::D3DXCreateTextureFromFileA( s_pD3DDevice, "Media/Textures/CellOutterGlow.png", &s_pTexture ) );

	// Creating and feeding the shared indexBuffer
	THR( s_pD3DDevice->CreateIndexBuffer( 6 * sizeof( WORD ), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &s_pIndexBuffer, 0 ) );
	WORD* pData = 0;
	THR( s_pIndexBuffer->Lock( 0, 0, (void**)&pData, 0 ) );
	for ( UINT ui = 0; ui < 6; ui++ )
		pData[ ui ] = CSquareShape::getIndexBufferArray()[ ui ];
	THR( s_pIndexBuffer->Unlock() );


	// Creating and feeding the shared vertexBuffer
	CSquareShape* pTempSquareShapeForOutterGlow = 0;
	ASSERT_MEMORYALLOCATION( pTempSquareShapeForOutterGlow = new CSquareShape( 0.0f, 90.0f ) );
	THR( s_pD3DDevice->CreateVertexBuffer( 4 * sizeof( SquareShapeVertex ), D3DUSAGE_WRITEONLY,
		SquareShapeVertex::FVF, D3DPOOL_MANAGED, &s_pVertexBuffer, 0 ) );
	SquareShapeVertex* pVertextData = 0;
	THR( s_pVertexBuffer->Lock( 0, 0, (void**)&pVertextData, 0 ) );
	for ( UINT ui = 0; ui < 4; ui++ )
		pVertextData[ ui ] = pTempSquareShapeForOutterGlow->getVerts()[ ui ];
	THR( s_pVertexBuffer->Unlock() );
	SAFE_DELETE( pTempSquareShapeForOutterGlow );


	// Creating the Render States
	ASSERT_MEMORYALLOCATION( s_pRenderStates = new CRenderStateManagement() );
	Render_State arrRenderStates[] = {
		{ D3DRS_CULLMODE, D3DCULL_NONE },
		{ D3DRS_LIGHTING, TRUE },
		{ D3DRS_SPECULARENABLE, FALSE },
		{ D3DRS_ALPHABLENDENABLE, TRUE },
		{ D3DRS_SRCBLEND, D3DBLEND_SRCALPHA },
		{ D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA }
	};
	Texture_Stage_State arrTextureStates[] = { // For stage 0
		{ D3DTSS_COLORARG1, D3DTA_TEXTURE },
		{ D3DTSS_COLOROP, D3DTOP_SELECTARG1 },
		{ D3DTSS_ALPHAARG1, D3DTA_TEXTURE },
		{ D3DTSS_ALPHAARG2, D3DTA_DIFFUSE },
		{ D3DTSS_ALPHAOP, D3DTOP_MODULATE }
	};
	s_pRenderStates->setStates( arrRenderStates, NUM_OF( arrRenderStates ) );
	s_pRenderStates->setStates( 0, arrTextureStates, NUM_OF( arrTextureStates ) );
	s_pRenderStates->createStateBlock();

	s_bClassInnited = true;
}


void CCellOutterGlow::releaseStaticDeviceObjects()
{
	SAFE_RELEASE( s_pTexture );
	SAFE_RELEASE( s_pIndexBuffer );
	SAFE_RELEASE( s_pVertexBuffer );

	SAFE_DELETE( s_pRenderStates );

	s_bClassInnited = false;
}


void CCellOutterGlow::setOpacity( const float& fOpacity )
{
	m_fOpacity = fOpacity;
	m_material.Diffuse.a = m_fOpacity;
}


void CCellOutterGlow::Draw()
{
	ASSERT_D3DDEVICE( s_pD3DDevice );

	// setting vertex and index buffer on the device
	THR( s_pD3DDevice->SetStreamSource( 0, s_pVertexBuffer, 0, sizeof(SquareShapeVertex) ) );
	THR( s_pD3DDevice->SetIndices( s_pIndexBuffer ) );

	// setting the texture
	THR( s_pD3DDevice->SetTexture( 0, s_pTexture ) );

	// setting the render states on the device
	s_pRenderStates->startUsingStateBlock();

	THR( s_pD3DDevice->SetFVF( SquareShapeVertex::FVF ) );
	THR( s_pD3DDevice->SetMaterial( &m_material ) );
	THR( s_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 ) );
	
	s_pRenderStates->endUsingStateBlock();
}