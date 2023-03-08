#include "SquareShape.h"

using namespace ArrowMaze;


const DWORD SquareShapeVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

float CSquareShape::s_fZ = 0.0f;
WORD CSquareShape::s_warrIndexBuffer[6] = { 0, 1, 2, 0, 2, 3 }; // IndexBuffer for this simple Square

#define SQUARESHAPE_Z (CSquareShape::s_fZ)


CSquareShape::CSquareShape( const float& fCenterX, const float& fSideLength )
{
	ASSERT_MEMORYALLOCATION( m_pVerts = new SquareShapeVertex[4] );
	m_fSideLength = fSideLength;
	generateVerts( fCenterX );
}


// If you are using an StripeTexture associated with this square your welcome to use this ctor!
CSquareShape::CSquareShape( const float& fCenterX, const UINT& uiTextureStripeIndex, const UINT& uiTextureStripeSectionsCount, const float& fSideLength )
{
	ASSERT_MEMORYALLOCATION( m_pVerts = new SquareShapeVertex[4] );
	m_fSideLength = fSideLength;
	generateVerts( fCenterX, uiTextureStripeIndex, uiTextureStripeSectionsCount );
}


CSquareShape::CSquareShape( const CSquareShape& copy )
{
	ASSERT_MEMORYALLOCATION( m_pVerts = new SquareShapeVertex[4] );
	
	m_fSideLength = copy.getSideLength();

	// copying the four verts
	const SquareShapeVertex* arrCopyVerts = copy.getVerts();
	for ( UINT ui = 0; ui < 4; ui++ )
		m_pVerts[ ui ] = arrCopyVerts[ ui ];
}


CSquareShape::~CSquareShape( void )
{
	SAFE_DELETE_ARRAY( m_pVerts );
}


void CSquareShape::generateVerts( const float& fCenterX, const UINT& uiTextureStripeIndex, const UINT& uiTextureStripeSectionsCount )
{
	ASSERT( uiTextureStripeSectionsCount, "The count of TextureStripe's Sections can't be zero!" );

	float fHalfSideSize = m_fSideLength / 2.0f;
	float fTexU = (float)uiTextureStripeIndex / (float)uiTextureStripeSectionsCount;
	float fReciprocalTexStripeCount = 1.0f / (float)uiTextureStripeSectionsCount;

	// Upper-left corner
	m_pVerts[0] = SquareShapeVertex(
		fCenterX - fHalfSideSize,
		fHalfSideSize,
		SQUARESHAPE_Z, 
		fTexU, 0.0f );

	// Upper-right corner
	m_pVerts[1] = SquareShapeVertex(
		fCenterX + fHalfSideSize,
		fHalfSideSize,
		SQUARESHAPE_Z, 
		fTexU + fReciprocalTexStripeCount, 0.0f );

	// Lower-right corner
	m_pVerts[2] = SquareShapeVertex(
		fCenterX + fHalfSideSize,
		-fHalfSideSize,
		SQUARESHAPE_Z, 
		fTexU + fReciprocalTexStripeCount, 1.0f );

	// Lower-left corner
	m_pVerts[3] = SquareShapeVertex(
		fCenterX - fHalfSideSize,
		-fHalfSideSize,
		SQUARESHAPE_Z, 
		fTexU, 1.0f );
}
