#include "CellBase.h"

using namespace ArrowMaze;


ArrowMaze::CCellBase::CCellBase(void)
{
	setRow( 0 );
	setColumn( 0 );
	setDirection( NO_DIRECTION );
}


ArrowMaze::CCellBase::CCellBase( const ArrowMaze::CCellBase& copy )
{
	setRow( copy.getRow() );
	setColumn( copy.getColumn() );
	setDirection( copy.getDirection() );
}


ArrowMaze::CCellBase::CCellBase( const UINT& uiRow, const UINT& uiColumn, const CellDirection& direction )
{
	setRow( uiRow );
	setColumn( uiColumn );
	setDirection( direction );
}


ArrowMaze::CCellBase::~CCellBase(void)
{
}
