// In the name of God
// Ehsan Marufi Azar, 2013 (c)

#ifndef ROUTESLIST_H
#define ROUTESLIST_H
#pragma once

#include "stdafx.h"
#include <list>
#include "Route.h"
#include "CellBase.h"
#include "Assertion.h"
#include "safe.h"

namespace ArrowMaze
{
	class CRoutesList
	{
	public:
		CRoutesList();
		CRoutesList( const CRoutesList& copy );
		~CRoutesList(void);

		inline void pushRoute( const CRoute& route ) { m_lstRoutesList.push_back( route ); }
		inline void pop() { m_lstRoutesList.pop_back(); }
		inline const UINT size() const { return m_lstRoutesList.size(); }
		const CRoute& getRoute( const UINT& uiIndex ) const;

		void calculateTheClosestRoutes( const UINT& uiRowsCount, const UINT& uiColumnsCount );
		const CRoute& getTheClosestRouteToDestinationCellFrom( const CCellBase& from ) const;

		inline const UINT& getRowsCount() const { return m_uiRowsCount; }
		inline const UINT& getColumnsCount() const { return m_uiColumnsCount; }

	private:
		typedef std::list<CRoute> RoutesList;
		
		RoutesList m_lstRoutesList;
		CRoute* m_arrClosestRoute;

		CRoute findTheClosestRouteToDestinationCellFrom( const CCellBase& from ) const;

		UINT m_uiRowsCount, m_uiColumnsCount;
	}; // end of class 'CRoutesList'

}// end of namespace 'ArrowMaze'
#endif // ROUTESLIST_H