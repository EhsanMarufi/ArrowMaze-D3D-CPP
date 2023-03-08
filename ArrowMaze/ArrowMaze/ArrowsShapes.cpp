#include "ArrowsShapes.h"
#include "SquareShape.h"

using namespace ArrowMaze;
CArrowShapesVertexBuffers::ListOfVertexBuffers CArrowShapesVertexBuffers::s_lstVertexBuffers;

void CArrowShapesVertexBuffers::createAllArrowShapesSquareVertexBuffers()
{
	releaseAllArrowShapesSquareVertexBuffers();

	CellDirection currentDirection = NO_DIRECTION;
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = 0;
	CSquareShape* pSquareShape = 0;

	CSquareShape::setCreationZ( -5.0f );

	for( UINT ui = 0; ui < COUNT_OF_AVAILABLE_DIRECTIONS +1; ui++ )
	{
		ASSERT_MEMORYALLOCATION( pSquareShape = new CSquareShape( 0.0f, getDirectionOrdinal( currentDirection ) -1, ARROWTEXTURE_OBJECTSCOUNT, 50.0f ) );

		// Create and fill the vertex buffer with data generated through CArrowShape
		pVertexBuffer = 0;
		THR( s_pD3DDevice->CreateVertexBuffer( 4 * sizeof( SquareShapeVertex ), D3DUSAGE_WRITEONLY,
			SquareShapeVertex::FVF, D3DPOOL_MANAGED, &pVertexBuffer, 0 ) );
		SquareShapeVertex* pData = 0;
		THR( pVertexBuffer->Lock( 0, 0, (void**)&pData, 0 ) );
		for( UINT uiJ = 0; uiJ < 4; uiJ++ )
		{
			pData[ uiJ ] = pSquareShape->getVerts()[ uiJ ];
		}
		THR( pVertexBuffer->Unlock() );

		// Add the created vertexBuffer to the list
		s_lstVertexBuffers.insert( ListOfVertexBuffers::value_type( currentDirection, pVertexBuffer ) );

		// free-up the unnecessary memory
		SAFE_DELETE( pSquareShape );

		// Proceed to the next one here, because we will also need the 'NO_DIRECTION'!
		currentDirection = (CellDirection)( 1 << ui );
	}
}


void CArrowShapesVertexBuffers::releaseAllArrowShapesSquareVertexBuffers()
{
	ListOfVertexBuffers::iterator Iter = s_lstVertexBuffers.begin(),
		end = s_lstVertexBuffers.end();
	for( ; Iter != end; Iter++ )
		SAFE_RELEASE( Iter->second );
}
