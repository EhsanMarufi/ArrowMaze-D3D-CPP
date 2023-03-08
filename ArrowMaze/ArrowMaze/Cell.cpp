#include "Cell.h"
#include "SquareShape.h"

using namespace ArrowMaze;
using namespace Direct3D;


CCell::CCell(void)
{
	m_pOutterGlow = 0;
	m_pArrowShape = 0;
	m_pCellMesh	  = 0;
}


CCell::CCell( const UINT& uiRow, const UINT& uiColumn, const CellDirection& direction )
	: CCellBaseEx( uiRow, uiColumn, direction ), CWidget()
{
	m_pOutterGlow = 0;
	ASSERT_MEMORYALLOCATION( m_pOutterGlow = new CCellOutterGlow() );

	m_pArrowShape = 0;
	ASSERT_MEMORYALLOCATION( m_pArrowShape = new CArrowShape( direction ) );

	m_pCellMesh = 0;
	ASSERT_MEMORYALLOCATION( m_pCellMesh = new CCellMesh() );
	m_pCellMesh->getCellMesh()->setDrawingFromOrigin( true );
}


CCell::CCell( const CCell& copy )
	: CCellBaseEx( copy.getRow(), copy.getColumn(), copy.getDirection() ), CWidget( copy )
{
	// Since all of these data-members are using the shared objects, there is no need to use the
	// copies' objects (they are also using the shared ones); so just capture ordinarily capture memory.
	m_pOutterGlow = 0;
	ASSERT_MEMORYALLOCATION( m_pOutterGlow = new CCellOutterGlow() );

	m_pArrowShape = 0;
	ASSERT_MEMORYALLOCATION( m_pArrowShape = new CArrowShape( m_CellDirection ) );

	m_pCellMesh = 0;
	ASSERT_MEMORYALLOCATION( m_pCellMesh = new CCellMesh() );
	m_pCellMesh->getCellMesh()->setDrawingFromOrigin( true );
}


CCell::~CCell(void)
{
	SAFE_DELETE( m_pArrowShape );
	SAFE_DELETE( m_pOutterGlow );
	SAFE_DELETE( m_pCellMesh );
}


void CCell::Draw()
{
	ASSERT_D3DDEVICE( s_pD3DDevice );

	// setting the cell's transformations (Position, Rotation, Scale) on the device
	CWidget::applyTransformations( false );

	m_pCellMesh->Draw();
	m_pOutterGlow->Draw();
	m_pArrowShape->Draw();
}