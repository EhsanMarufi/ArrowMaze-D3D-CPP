#include "CreationParameters.h"

using namespace ArrowMaze;

ArrowMaze::CCreationParameters::CCreationParameters
	(
	const UINT& uiRowsCount,
	const UINT& uiColumnsCount,
	const CCellBase* pHomeCell,
	const CCellBase* pDestinationCell,
	const UINT& uiMinRouteNodes,
	const UINT& uiPreferedMinNodeDistance,
	const UINT& uiPreferedMaxNodeDistance
	)
{
	ASSERT( (m_uiRowsCount = uiRowsCount) > 1, "Minimum value for the rows count is 2" );
	ASSERT( (m_uiColumnsCount = uiColumnsCount) > 1, "Minimum value for the columns count is 2" );;
	m_uiMinRouteNodes = uiMinRouteNodes == 0 ? calcSuitableMinRouteNodes() : uiMinRouteNodes;
	m_uiPreferedMinNodeDistance = uiPreferedMinNodeDistance == 0 ? calcMinNodeDistance() : uiPreferedMinNodeDistance;
	m_uiPreferedMaxNodeDistance = uiPreferedMaxNodeDistance == 0 ? calcMaxNodeDistance() : uiPreferedMaxNodeDistance;

	ASSERT_MEMORYALLOCATION( m_pHomeCell = pHomeCell == 0 ? new CCellBase( 1, 1 ) : new CCellBase( *pHomeCell ) );
	ASSERT_MEMORYALLOCATION( m_pDestinationCell = pDestinationCell == 0 ? new CCellBase( m_uiRowsCount, m_uiColumnsCount ) : new CCellBase( *pDestinationCell ) );

	ASSERT( *m_pHomeCell != *m_pDestinationCell, "'Home' and 'Destination' cells must be diffrent." );
}


ArrowMaze::CCreationParameters::CCreationParameters( const CCreationParameters& copy )
{
	m_uiRowsCount = copy.getRowsCount();
	m_uiColumnsCount = copy.getColumnsCount();
	m_uiMinRouteNodes = copy.getMinRouteNodes();
	m_uiPreferedMinNodeDistance = copy.getPreferedMinNodeDistance();
	m_uiPreferedMaxNodeDistance = copy.getPreferedMaxNodeDistance();

	ASSERT_MEMORYALLOCATION( m_pHomeCell = new CCellBase(copy.getHomeCell()) );
	ASSERT_MEMORYALLOCATION( m_pDestinationCell = new CCellBase(copy.getDestinationCell()) );
}


ArrowMaze::CCreationParameters::~CCreationParameters( void )
{
	SAFE_DELETE( m_pHomeCell );
	SAFE_DELETE( m_pDestinationCell );
}


UINT ArrowMaze::CCreationParameters::calcSuitableMinRouteNodes()
{
	return 5; //TODO
}


UINT ArrowMaze::CCreationParameters::calcMinNodeDistance()
{
	return 1; //TODO
}


UINT ArrowMaze::CCreationParameters::calcMaxNodeDistance()
{
	return 5; //TODO
}
