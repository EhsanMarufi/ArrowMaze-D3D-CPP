// In the name of God
// Ehsan Marufi Azar, 2013 (c)

#pragma once
#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include <list>
#include "stdafx.h"
//#include "Assertion.h"
#include "safe.h"
#include "CreationParameters.h"
#include "CellBase.h"
#include "CellDirection.h"
#include "Route.h"
#include "RoutesList.h"


namespace ArrowMaze
{

#define MAZEGENERATOR_SUCCEEDED								( 0      )
#define MAZEGENERATOR_ERROR_WRONGCOMBINATION_OF_DIRECTIONS	( 1 << 0 )
#define MAZEGENERATOR_ERROR_INSOLVABLEMAZE					( 1 << 1 )


	class CMazeGenerator
	{
	public:
		typedef std::list<const CCellBase*> CellsList;
		typedef std::list<CellsList> ListCollection;

	public:
		CMazeGenerator( const CCreationParameters& cp );
		~CMazeGenerator( void );

		const CCellBase* getCell_c( const UINT& uiRow, const UINT& uiColumn ) const;

		UINT generate();

		// inline getters
		inline const CRoutesList& getPossibleRoutes() const { return m_allPossibleRoutes; }
		inline const CCellBase* getCells() const { return m_arrCells; }
		inline const CellsList* getCollectionOfCellsPointed() const { return m_arrCollectionOfCellsPointed; }
		inline const CCreationParameters* getCreationParameters() const { return m_pCreationParameters; }


	protected:
		CCellBase* getCell ( const UINT& uiRow, const UINT& uiColumn ) const;
		CCellBase* getCell ( const CCellBase& cell ) const;

		CCellDirections getPossibleDirections( const CCellBase& from ) const;
		CellDirection findDirection( const CCellBase& from, const CCellBase& to ) const;
		CellsList getCellsPointedBy( const CCellBase& from ) const;
		CellsList getCellsAlong( const CCellBase& cell ) const;
		CellsList findRepeatativeCells( const ListCollection& listCollection ) const;
		bool isInvalidSequence( const CellsList& list, const CellDirection* pDirection = 0 ) const;
		ListCollection findInvalidSequences() const;

		CRoutesList getAllPossibleRoutes() const;
		CellsList getNeighborhoodCellsPointingTo( const CCellBase& cell ) const;
		const CCellBase* getCellByIndex( const CellsList& list, const UINT& uiIndex ) const;

	private:
		CCellBase* m_arrCells;
		CCreationParameters* m_pCreationParameters;
		CRoutesList m_allPossibleRoutes;

		// Maze solving utilities
		CellsList* m_arrCollectionOfCellsPointed;
		inline const CellsList& getNeighborhoodCellsPointingTo_Performace( const CCellBase& cell ) const
		{
			const UINT& uiRowsCount = m_pCreationParameters->getRowsCount();
			const UINT& uiColumnsCount = m_pCreationParameters->getColumnsCount();
			const UINT& uiRow = cell.getRow();
			const UINT& uiColumn = cell.getColumn();

			ASSERT( uiRow <= uiRowsCount && uiColumn <= uiColumnsCount && uiRow > 0 && uiColumn > 0, "Out of range!" );
			return m_arrCollectionOfCellsPointed[ ( uiRow -1 ) * uiColumnsCount + ( uiColumn - 1) ];
		}
	};

} // end of namespace 'Arrow Maze'
#endif // MAZE_GENERATOR_H