#include "Grid.h"
#include "MazeGenerator.h"

using namespace ArrowMaze;

ArrowMaze::CGrid::CGrid( const CMazeGenerator* pMazeGenerator, const float& fHorSpace, const float& fVerSpace )
{
	const CCreationParameters* pCP = pMazeGenerator->getCreationParameters();
	m_uiRowsCount = pCP->getRowsCount();
	m_uiColumnsCount = pCP->getColumnsCount();

	const CCellBase& homeCell = pCP->getHomeCell();
	const CCellBase& destinationCell = pCP->getDestinationCell();

	m_pHomeCell = 0;
	m_pDestinationCell = 0;

	// Ceating and Filling the grid's reference table of cells
	const UINT cuiCellsCount = m_uiRowsCount * m_uiColumnsCount;
	ASSERT_MEMORYALLOCATION( m_arrOfGridCellPointers = new CCell*[ cuiCellsCount ] );
	for ( UINT ui = 0; ui < cuiCellsCount; ui++ )
		m_arrOfGridCellPointers[ ui ] = 0;


	UINT uiCellIndex = 0;
	const CCellBase* pMazeGeneratorCells = pMazeGenerator->getCells();
	for ( UINT uiRow = 0; uiRow < m_uiRowsCount; uiRow++ )
	{
		for ( UINT uiColumn = 0; uiColumn < m_uiColumnsCount; uiColumn++ )
		{
			uiCellIndex = ( uiRow * m_uiColumnsCount ) + uiColumn;
			const CCellBase& currentCell_MG = pMazeGeneratorCells[ uiCellIndex ];
			CCell* pCurrentCell = 0;

			pCurrentCell = m_arrOfGridCellPointers[ uiCellIndex ] = new CCell( currentCell_MG.getRow(), currentCell_MG.getColumn(), currentCell_MG.getDirection() );
			
			if ( !m_pHomeCell )
				if ( currentCell_MG == homeCell )
					m_pHomeCell = pCurrentCell;

			if ( !m_pDestinationCell )
				if ( currentCell_MG == destinationCell )
					m_pDestinationCell = pCurrentCell;
		}
	}


	// Although this could've been done along with the upper loops, but this is a more natural
	// way to fill 'm_arrCollectionOfCellsPointed', making things tidy and simple.
	// Filling the table in which the cells get pointed by every cell in the 
	// grid's reference table of cells
	ASSERT_MEMORYALLOCATION( m_arrCollectionOfCellsPointed = new ListOfCells[ m_uiRowsCount * m_uiColumnsCount ] );
	const CMazeGenerator::CellsList* pMGtblOfCellsList = pMazeGenerator->getCollectionOfCellsPointed();
	
	for ( UINT uiRow = 0; uiRow < m_uiRowsCount; uiRow++ )
	{
		for ( UINT uiColumn = 0; uiColumn < m_uiColumnsCount; uiColumn++ )
		{
			uiCellIndex = ( uiRow * m_uiColumnsCount ) + uiColumn;
			CCell* pCurrentCell = m_arrOfGridCellPointers[ uiCellIndex ];
			const CMazeGenerator::CellsList& lstCellsPointedByCurrentCell = pMGtblOfCellsList[ uiCellIndex ];

			// iterate through every cell, which is pointed by currentCell, and hold a pointer to 
			// its address in the table.
			CMazeGenerator::CellsList::const_iterator cIter = lstCellsPointedByCurrentCell.cbegin(),
				cEnd = lstCellsPointedByCurrentCell.cend();
			for ( ; cIter != cEnd; cIter++ )
			{
				const CCellBase* pCurrenCellInListOfMG = *cIter;
				const CCell* pCellToInsert = getCell( pCurrenCellInListOfMG->getRow(), pCurrenCellInListOfMG->getColumn() );
				m_arrCollectionOfCellsPointed[ uiCellIndex ].push_back( pCellToInsert );
			}
		}
	}


	// Position all the cells in place
	float fCellWidth = 0.0f, fCellHeight = 0.0f, fCellDepth = 0.0f;
	m_arrOfGridCellPointers[ 0 ]->getCellDimentions( fCellWidth, fCellHeight, fCellDepth );
	const float fGridHorHalfSize = ( ( fCellWidth * m_uiColumnsCount ) + ( fHorSpace * ( m_uiColumnsCount -1 ) ) ) / 2.0f;
	const float fGridVerHalfSize = ( ( fCellHeight * m_uiRowsCount ) + ( fVerSpace * ( m_uiRowsCount -1 ) ) ) / 2.0f;
	
	float fHorStartCoord = -fGridHorHalfSize + ( fCellWidth / 2.0f );
	float fVerStartCoord = fGridVerHalfSize + ( fCellHeight / 2.0f );
	float x, y, z = 0.0f;
	for ( UINT uiRow = 0; uiRow < m_uiRowsCount; uiRow++ )
	{
		x = fHorStartCoord;
		y = fVerStartCoord - ( uiRow * fCellHeight ) - ( uiRow * fVerSpace );
		for ( UINT uiColumn = 0; uiColumn < m_uiColumnsCount; uiColumn++ )
		{
			x = fHorStartCoord + ( uiColumn * fCellWidth ) + ( uiColumn * fHorSpace );
			getCell( uiRow +1, uiColumn +1 )->setPosition( D3DXVECTOR3( x, y, z ) );
		}
	}

}


CGrid::~CGrid(void)
{
	// don't delete 'm_pHomeCell' and 'm_pDestinationCell'; they are just references

	// Delete every individual cell
	const UINT uiCellsCount = m_uiRowsCount * m_uiColumnsCount;
	for ( UINT ui = 0; ui < uiCellsCount; ui++ )
		SAFE_DELETE( m_arrOfGridCellPointers[ui] );


	SAFE_DELETE_ARRAY( m_arrOfGridCellPointers );
	SAFE_DELETE_ARRAY( m_arrCollectionOfCellsPointed );
}


inline const CCell* CGrid::getCell( const UINT& uiRow, const UINT& uiColumn ) const
{
	ASSERT( uiRow <= m_uiRowsCount && uiColumn <= m_uiColumnsCount && uiRow > 0 && uiColumn > 0, "Out of range!" );
	return m_arrOfGridCellPointers[ ( uiRow -1 ) * m_uiColumnsCount + ( uiColumn -1 )];
}


inline CCell* CGrid::getCell( const UINT& uiRow, const UINT& uiColumn )
{
	ASSERT( uiRow <= m_uiRowsCount && uiColumn <= m_uiColumnsCount && uiRow > 0 && uiColumn > 0, "Out of range!" );
	return m_arrOfGridCellPointers[ ( uiRow -1 ) * m_uiColumnsCount + ( uiColumn -1 ) ];
}


ListOfCells CGrid::getListOfPossibleCells( const CCell& from )  const
{
	UINT uiIndex = ( from.getRow() * m_uiColumnsCount ) + from.getColumn();
	ListOfCells lstOut = m_arrCollectionOfCellsPointed[ uiIndex ];

	ListOfCells::const_iterator cIter = lstOut.cbegin();
	while ( cIter != lstOut.cend() )
	{
		if ( (*cIter)->isCellMet() )
			cIter = lstOut.erase( cIter );
		else
			cIter++;
	}

	return lstOut;
}


void CGrid::setCellMet( const CCell& cell, const bool& bVal )
{
	const UINT& uiCellRow = cell.getRow();
	const UINT& uiCellColumn = cell.getColumn();
	ASSERT( uiCellRow <= m_uiRowsCount && uiCellColumn <= m_uiColumnsCount, "Out of range!" );

	getCell( uiCellRow, uiCellColumn )->setCellMet( bVal );
}


void CGrid::Draw()
{
	const UINT uiCellsCount = m_uiRowsCount * m_uiColumnsCount;
	for ( UINT ui = 0; ui < uiCellsCount; ui++ )
	{
		m_arrOfGridCellPointers[ui]->Draw();
	}
}