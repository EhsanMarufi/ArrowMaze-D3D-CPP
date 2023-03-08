#include "CellBaseEx.h"

using namespace ArrowMaze;


CCellBaseEx::CCellBaseEx()
	: CCellBase()
{
	setCellMet( false );
	setCellIsOnAPossibleRoute ( false );
}


CCellBaseEx::CCellBaseEx( const UINT& uiRow, const UINT& uiColumn, const CellDirection& direction )
	: CCellBase( uiRow, uiColumn, direction )
{
	setCellMet( false );
	setCellIsOnAPossibleRoute ( false );
}


CCellBaseEx::CCellBaseEx( const ArrowMaze::CCellBaseEx& copy )
	: CCellBase( copy )
{
	setCellMet( copy.isCellMet() );
	setCellIsOnAPossibleRoute ( copy.isCellOnAPossibleRoute() );
}


CCellBaseEx::~CCellBaseEx( void )
{
}
