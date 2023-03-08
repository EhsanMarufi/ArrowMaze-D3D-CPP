// In the name of God
// Ehsan Marufi Azar, 2013 (c)

#pragma once;
#ifndef CREATIONPARAMETERS_H
#define CREATIONPARAMETERS_H

#include "stdafx.h"
#include "CellBase.h"
#include "safe.h"

namespace ArrowMaze
{

	class CCreationParameters
	{
	public:
		CCreationParameters
			(
			const UINT& uiRowsCount,
			const UINT& uiColumnsCount,
			const CCellBase* pHomeCell = 0,
			const CCellBase* pDestinationCell = 0,
			const UINT& uiMinRouteNodes = 0,
			const UINT& uiPreferedMinNodeDistance = 0,
			const UINT& uiPreferedMaxNodeDistance = 0
			);
		CCreationParameters( const CCreationParameters& copy );
		~CCreationParameters();

		inline const UINT& getRowsCount() const { return m_uiRowsCount; }
		inline const UINT& getColumnsCount() const { return m_uiColumnsCount; }
		inline const UINT& getMinRouteNodes() const { return m_uiMinRouteNodes; }
		inline const UINT& getPreferedMinNodeDistance() const { return m_uiPreferedMinNodeDistance; }
		inline const UINT& getPreferedMaxNodeDistance() const { return m_uiPreferedMaxNodeDistance; }
		inline const CCellBase& getHomeCell() const { return *m_pHomeCell; }
		inline const CCellBase& getDestinationCell() const { return *m_pDestinationCell; }

	protected:
		UINT calcSuitableMinRouteNodes();
		UINT calcMinNodeDistance();
		UINT calcMaxNodeDistance();

	private:
		UINT m_uiRowsCount, m_uiColumnsCount;
		CCellBase* m_pHomeCell, *m_pDestinationCell;
		UINT m_uiMinRouteNodes;
		UINT m_uiPreferedMinNodeDistance, m_uiPreferedMaxNodeDistance;

	};// end of class 'CCreationParameters'
}// end of namespace 'ArrowMaze'

#endif // CREATIONPARAMETERS_H