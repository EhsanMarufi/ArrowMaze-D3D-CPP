// In the name of God
// Ehsan Marufi Azar, 2013 (c)

#pragma once
#ifndef CGRID_H
#define CGRID_H

#include <list>
#include "stdafx.h"
#include "Assertion.h"
#include "safe.h"
#include "Cell.h"

namespace ArrowMaze
{
	typedef std::list<const CCell*> ListOfCells;
	class CMazeGenerator; // Forward declaration

	class CGrid
	{
	public:
		CGrid( const CMazeGenerator* pMazeGenerator, const float& fHorSpace = 40.0f, const float& fVerSpace = 40.0f );
		~CGrid( void );

		const CCell* getCell( const UINT& uiRow, const UINT& uiColumn ) const;
		ListOfCells getListOfPossibleCells( const CCell& from )  const;

		void setCellMet( const CCell& cell, const bool& val );
		inline const bool& getCellMet( const CCell& cell ) const { return getCell( cell.getRow(), cell.getColumn() )->isCellMet(); }

		void Draw();


		// inline getters
		inline const UINT& getRowsCount() const { return m_uiRowsCount; }
		inline const UINT& getColumnsCount() const { return m_uiColumnsCount; }

		inline const CCell* getHomeCell() const { return m_pHomeCell; }
		inline const CCell* getDestinationCell() const { return m_pDestinationCell; }

	
	protected:
		CCell* getCell( const UINT& uiRow, const UINT& uiColumn );
		CCell* m_pHomeCell;
		CCell* m_pDestinationCell;
	
	private:
		UINT m_uiRowsCount, m_uiColumnsCount;
		CCell** m_arrOfGridCellPointers;
		ListOfCells* m_arrCollectionOfCellsPointed;
	};

} // end of namespace 'ArrowMaze'
#endif //CGRID_H