#include "GameLogic.h"
#include "MazeGenerator.h"
#include "ArrowsShapes.h" // For initializing the class-level static data of 'CArrowShapesVertexBuffers' class

using namespace ArrowMaze;
using namespace Direct3D;

CGameLogic::CGameLogic( void )
{
	m_pCamera = 0;
	m_pGrid = 0;
	m_cpCurrentCell = 0;
}


CGameLogic::~CGameLogic( void )
{
	// don't delete 'm_pCurrentCell', it's just a reference

	SAFE_DELETE( m_pCamera );
	SAFE_DELETE( m_pGrid );

	// release the class-level static device objects
	CCellMesh::releaseCellMeshSharedObject();
	CArrowShape::releaseStaticDeviceObjects();
	CCellOutterGlow::releaseStaticDeviceObjects();
	CArrowShapesVertexBuffers::releaseAllArrowShapesSquareVertexBuffers();
}


void CGameLogic::initializeGridObject()
{
	ArrowMaze::CCreationParameters cp( 5, 5 );
	CMazeGenerator* pArrowMazeGenerator = 0;
	ASSERT_MEMORYALLOCATION( pArrowMazeGenerator = new ArrowMaze::CMazeGenerator( cp ) );

	// Trying to generate a random arrow maze
	UINT uiRegenerationIndex = 0;
	UINT uiGenerationResult = MAZEGENERATOR_SUCCEEDED;
	do
	{
		uiGenerationResult = pArrowMazeGenerator->generate();
	} while ( uiGenerationResult != MAZEGENERATOR_SUCCEEDED && uiRegenerationIndex++ < REGENERATION_MAX );

	ASSERT( uiGenerationResult == MAZEGENERATOR_SUCCEEDED, "Generating the arrow maze FAILED." );


	// Fill the m_pGrid with the data generated through the MazeGenerator
	m_pGrid = 0;
	ASSERT_MEMORYALLOCATION( m_pGrid = new CGrid( pArrowMazeGenerator ) );


	// The maze generator is no longer needed, its neccessary contents have already been copied!
	SAFE_DELETE( pArrowMazeGenerator );
}


void CGameLogic::Draw()
{
	static const float scfPerFrameIncreament = 0.05f;
	m_pGrid->Draw();
}


HRESULT CGameLogic::onD3DDeviceCreate( LPDIRECT3DDEVICE9 lpD3DDevice )
{
	// The assignment of device pointer to base class: the 'CDeviceAccessor'
	CDeviceAccessor::setDevice( lpD3DDevice );

	// initialize the class-level static device objects
	CCellMesh::createCellMeshSharedObject();
	CArrowShape::createStaticDeviceObjects();
	CCellOutterGlow::createStaticDeviceObjects();
	CArrowShapesVertexBuffers::createAllArrowShapesSquareVertexBuffers();

	// initialize the grid object
	initializeGridObject();

	m_cpCurrentCell = m_pGrid->getHomeCell();

	// initialize the camera
	ASSERT_MEMORYALLOCATION( m_pCamera = new CBaseCamera( D3DXVECTOR3(0.0f, 0.0f, -900.0f) ) );
	m_pCamera->setFarZClipping( 2000.0f );
	m_pCamera->applyProjectionMatrix();
	m_pCamera->applyViewMatrix();
	
	return S_OK;
}


void CGameLogic::onKeyDown( const WPARAM& wParam )
{
	if ( wParam == VK_DOWN )
	{

	}

	if ( wParam == VK_UP )
	{

	}

	if ( wParam == VK_RIGHT )
	{

	}

	if ( wParam == VK_LEFT )
	{

	}

	if ( wParam == VK_SPACE )
	{

	}

	if ( wParam == VK_RETURN )
	{

	}
}