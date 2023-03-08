#include "RoutesList.h"

using namespace ArrowMaze;

ArrowMaze::CRoutesList::CRoutesList()
{
	m_uiRowsCount = 0;
	m_uiColumnsCount = 0;

	m_arrClosestRoute = 0;
}


ArrowMaze::CRoutesList::CRoutesList( const ArrowMaze::CRoutesList& copy )
{
	m_uiRowsCount = copy.getRowsCount();
	m_uiColumnsCount = copy.getColumnsCount();

	UINT uiCountOfRoutes = copy.size();
	for ( UINT ui = 0; ui < uiCountOfRoutes; ui++ )
		m_lstRoutesList.push_back( copy.getRoute( ui ) );

	m_arrClosestRoute = 0;
	
	// if the copy object is already calculated the closest routes, then copy them too!
	if ( m_uiRowsCount )
	{
		ASSERT_MEMORYALLOCATION( m_arrClosestRoute = new CRoute[ m_uiRowsCount * m_uiColumnsCount ] );

		for ( UINT uiRow = 0; uiRow < m_uiRowsCount; uiRow++ )
			for ( UINT uiColumn = 0; uiColumn < m_uiColumnsCount; uiColumn++ )
				m_arrClosestRoute[ uiRow * m_uiColumnsCount + uiColumn ] = copy.getTheClosestRouteToDestinationCellFrom( CCellBase( uiRow +1, uiColumn +1 ) );
	}
}


ArrowMaze::CRoutesList::~CRoutesList(void)
{
	SAFE_DELETE_ARRAY( m_arrClosestRoute );
}


void ArrowMaze::CRoutesList::calculateTheClosestRoutes( const UINT& uiRowsCount, const UINT& uiColumnsCount )
{
	ASSERT( uiRowsCount > 0 && uiColumnsCount > 0, "Wrong count for either 'uiRowsCount' and/or 'uiColumnsCount'." );
	
	m_uiRowsCount = uiRowsCount;
	m_uiColumnsCount = uiColumnsCount;

	SAFE_DELETE_ARRAY( m_arrClosestRoute );
	ASSERT_MEMORYALLOCATION( m_arrClosestRoute = new CRoute[ m_uiRowsCount * m_uiColumnsCount ] );

	for ( UINT uiRow = 0; uiRow < m_uiRowsCount; uiRow++ )
		for ( UINT uiColumn = 0; uiColumn < m_uiColumnsCount; uiColumn++ )
			m_arrClosestRoute[ uiRow * m_uiColumnsCount + uiColumn ] = findTheClosestRouteToDestinationCellFrom ( CCellBase( uiRow +1, uiColumn +1 ) );
}


const ArrowMaze::CRoute& ArrowMaze::CRoutesList::getTheClosestRouteToDestinationCellFrom( const CCellBase& from ) const
{
	const UINT& uiRow = from.getRow();
	const UINT& uiColumn = from.getColumn();

	ASSERT( uiRow <= m_uiRowsCount && uiColumn <= m_uiColumnsCount && uiRow > 0 && uiColumn > 0, "Out of range!" );

	return m_arrClosestRoute[ ( uiRow -1 ) * m_uiColumnsCount + ( uiColumn -1 ) ];
}


ArrowMaze::CRoute ArrowMaze::CRoutesList::findTheClosestRouteToDestinationCellFrom( const CCellBase& from ) const
{
	UINT uiMinDistance = UINT_MAX;
	const CRoute* pClosestRoute = 0;
	CRoute out;
	CRoute::constListIterator cRouteIter, cRouteEnd;
	UINT uiIndex = 0;
	UINT uiCurrentRouteSize = 0;
	UINT uiNodesCountToDestination = 0;
	bool bFound = false;

	for ( RoutesList::const_iterator cIter = m_lstRoutesList.cbegin(); cIter != m_lstRoutesList.cend(); cIter++ )
	{
		// For this route, find the cell index within the route
		uiCurrentRouteSize = cIter->length();
		cIter->getConstIterator( cRouteIter, cRouteEnd );
		for ( uiIndex = 0; cRouteIter != cRouteEnd; cRouteIter++, uiIndex++ )
			if ( **cRouteIter == from )
			{
				bFound = true;
				break;
			}

		// if this is more closer, so hold it as the most closest yet!
		if ( bFound )
		{
			uiNodesCountToDestination = uiCurrentRouteSize - uiIndex;
			if ( uiNodesCountToDestination < uiMinDistance )
			{
				uiMinDistance = uiNodesCountToDestination;
				pClosestRoute = &*cIter;
			}

			bFound = false;
		}
	}

	// create the closest route starting form the 'from' cell!
	if ( pClosestRoute )
	{
		const UINT& uiClosestRouteLength = pClosestRoute->length();
		for ( UINT ui = uiIndex; ui < uiClosestRouteLength; ui++ )
			out.pushBack( pClosestRoute->getNode( ui ) );
	}

	return out;
}


const CRoute& ArrowMaze::CRoutesList::getRoute( const UINT& uiIndex ) const
{
	ASSERT( uiIndex < m_lstRoutesList.size(), "Out of range!" );

	RoutesList::const_iterator cIter = m_lstRoutesList.cbegin();
	for ( UINT ui = 0; ui < uiIndex; ui++ )
		cIter++;

	return *cIter;
}