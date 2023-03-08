#include "MazeGenerator.h"

using namespace ArrowMaze;

ArrowMaze::CMazeGenerator::CMazeGenerator( const CCreationParameters& cp )
	: m_allPossibleRoutes()
{
	m_pCreationParameters = 0;
	ASSERT_MEMORYALLOCATION( m_pCreationParameters = new CCreationParameters( cp ) );

	const UINT& uiRowsCount = m_pCreationParameters->getRowsCount();
	const UINT& uiColumnsCount = m_pCreationParameters->getColumnsCount();

	m_arrCells = 0;
	ASSERT_MEMORYALLOCATION( m_arrCells = new CCellBase[ uiRowsCount * uiColumnsCount ] );

	m_arrCollectionOfCellsPointed = 0;
	ASSERT_MEMORYALLOCATION( m_arrCollectionOfCellsPointed = new CellsList[ uiRowsCount * uiColumnsCount ] );

	
	// Initializing the coordinations of grid's cells
	CCellBase* pCell = 0;
	for ( UINT uiRow = 1; uiRow <= uiRowsCount; uiRow++ )
		for ( UINT uiColumn = 1; uiColumn <= uiColumnsCount; uiColumn++ )
		{
			pCell = getCell( uiRow, uiColumn );
			pCell->setRow( uiRow );
			pCell->setColumn( uiColumn );
		}
}


CMazeGenerator::~CMazeGenerator(void)
{
	SAFE_DELETE_ARRAY( m_arrCells );
	SAFE_DELETE_ARRAY( m_arrCollectionOfCellsPointed );
	SAFE_DELETE( m_pCreationParameters );
}


const CCellBase* ArrowMaze::CMazeGenerator::getCell_c( const UINT& uiRow, const UINT& uiColumn ) const
{
	ASSERT( uiRow > m_pCreationParameters->getRowsCount() || uiColumn > m_pCreationParameters->getColumnsCount(), "Out of range!" );

	return &m_arrCells[ (uiRow -1) * m_pCreationParameters->getColumnsCount() + uiColumn ];
}


CCellBase* ArrowMaze::CMazeGenerator::getCell ( const UINT& uiRow, const UINT& uiColumn ) const
{
	const UINT& uiRowsCount = m_pCreationParameters->getRowsCount();
	const UINT& uiColumnsCount = m_pCreationParameters->getColumnsCount();
	ASSERT( uiRow <= uiRowsCount && uiColumn <= uiColumnsCount, "Out of range!" );

	return &m_arrCells[ (uiRow -1) * uiColumnsCount + (uiColumn -1) ];
}


CCellBase* ArrowMaze::CMazeGenerator::getCell( const CCellBase& cell ) const
{
	const UINT& uiRow = cell.getRow();
	const UINT& uiColumn = cell.getColumn();
	const UINT& uiRowsCount = m_pCreationParameters->getRowsCount();
	const UINT& uiColumnsCount = m_pCreationParameters->getColumnsCount();
	ASSERT( uiRow <= uiRowsCount && uiColumn <= uiColumnsCount, "Out of range!" );

	return &m_arrCells[ (uiRow -1) * uiColumnsCount + (uiColumn -1) ];
}

UINT ArrowMaze::CMazeGenerator::generate()
{
	const CCellBase& DestinationCell = *getCell( m_pCreationParameters->getDestinationCell().getRow(), m_pCreationParameters->getDestinationCell().getColumn() );
	const CCellBase& HomeCell = *getCell( m_pCreationParameters->getHomeCell().getRow(), m_pCreationParameters->getHomeCell().getColumn() );
	const UINT& uiRowsCount = m_pCreationParameters->getRowsCount();
	const UINT& uiColumnsCount = m_pCreationParameters->getColumnsCount();

	// HomeCell doesn't have to point to Destination cell, directly
	CCellDirections homeCellPossibleDirs = getPossibleDirections( HomeCell );
	CellDirection homeCellInvalidDirs = findDirection( HomeCell, DestinationCell );
	homeCellPossibleDirs.removeDirections( homeCellInvalidDirs );
	getCell( HomeCell )->setDirection( homeCellPossibleDirs.selectRandomDirection() );


	// Randomly fill the cell with possible directions
	CCellBase tempCell;
	CCellDirections possibleDirs( NO_DIRECTION );
	for ( UINT uiRow = 1; uiRow <= uiRowsCount; uiRow++ )
	{
		for ( UINT uiColumn = 1; uiColumn <= uiColumnsCount; uiColumn++ )
		{
			tempCell = CCellBase( uiRow, uiColumn );
			if ( getCell( tempCell )->getDirection() != NO_DIRECTION )
				continue; // don't touch cells whose direction is set
			possibleDirs = getPossibleDirections( tempCell );
			getCell( tempCell )->setDirection( possibleDirs.selectRandomDirection() );
		}
	}
	// DestinationCell's direction should be NO_DIRECTION, otherwise it's nonsense.
	getCell( DestinationCell )->setDirection( NO_DIRECTION );


	// The cells which are pointed by HomeCell, also doesn't have to point to Destination cell, directly
	CellsList cellPointedByHomeCell = getCellsPointedBy( HomeCell );
	CellDirection dirTowardsDestinationCell = NO_DIRECTION;
	for ( CellsList::iterator Iter = cellPointedByHomeCell.begin(); Iter != cellPointedByHomeCell.end(); Iter++ )
	{
		dirTowardsDestinationCell = findDirection( **Iter, DestinationCell );
		if ( dirTowardsDestinationCell == (*Iter)->getDirection() )
		{
			tempCell = CCellBase( (*Iter)->getRow(), (*Iter)->getColumn() );
			possibleDirs = getPossibleDirections( tempCell );
			possibleDirs.removeDirections( dirTowardsDestinationCell );
			getCell( tempCell )->setDirection( possibleDirs.selectRandomDirection() );
		}
	}


	// InSolvable Maze: There must be atleast a cell in the 'cells ending to DestinationCell'


	// Remove 'Invalid Sequences': which means: Horizontal, Vertical or Diagonal Sequences which have
	// a same direcion. e.g: a row full of 'Right' or 'Left' Direction, or a column full of 'Up' or 'Down' directions;
	ListCollection listCollection = findInvalidSequences();
	UINT uiRandomCellOrinal = 0;
	for ( ListCollection::iterator listIter = listCollection.begin(); listIter != listCollection.end(); listIter++ )
	{
		while ( listIter->size() )
		{
			// randomly pick a cell from current list
			uiRandomCellOrinal = UINT( rand() ) % listIter->size();
			CellsList::iterator cellsIter = listIter->begin();
			for ( UINT ui = 0; ui < uiRandomCellOrinal; ui++ )
				cellsIter++;
			const CCellBase* cpCurrentSelectedCell = *cellsIter;

			const CellDirection currentCellDirection = cpCurrentSelectedCell->getDirection();
			possibleDirs = getPossibleDirections( *cpCurrentSelectedCell );
			possibleDirs.removeDirections( currentCellDirection | getOppositeDirection( currentCellDirection ) );

			// randomly test every possible direction, if the direction doesn't produce another 'Invalid' sequence hold it!
			CellsList *pTestListForInvalidSequence = 0;
			while( possibleDirs.getDirectionsCount() )
			{
				const CellDirection& randomCellDirection = possibleDirs.selectRandomDirection();
				possibleDirs.removeDirections( randomCellDirection );
				
				getCell( *cpCurrentSelectedCell )->setDirection( randomCellDirection );
				pTestListForInvalidSequence = &getCellsAlong( *cpCurrentSelectedCell );
				if ( !isInvalidSequence( *pTestListForInvalidSequence ) )
					goto IterateToNextList;
			}

			// this cell is unhealable; give its original direction back, and remove it from the list
			getCell( *cpCurrentSelectedCell )->setDirection( currentCellDirection );
			listIter->erase( cellsIter );
		}

		// If program flow catches here, it means that this combination of Directions is Invalid
		// try recreating the maze! (This is a very severe case!)
		return MAZEGENERATOR_ERROR_WRONGCOMBINATION_OF_DIRECTIONS;

		IterateToNextList:
		;
	}

	// fill the 'm_arrCollectionOfCellsPointed' for performance considerations
	for ( UINT uiRow = 0; uiRow < uiRowsCount; uiRow++ )
		for ( UINT uiColumn = 0; uiColumn < uiColumnsCount; uiColumn++ )
			m_arrCollectionOfCellsPointed[ uiRow * uiColumnsCount + uiColumn ] = getNeighborhoodCellsPointingTo( *getCell( uiRow +1, uiColumn +1 ) );

	
	// determine the possible routes
	m_allPossibleRoutes = getAllPossibleRoutes();
	
	if ( m_allPossibleRoutes.size() == 0 )
		return MAZEGENERATOR_ERROR_INSOLVABLEMAZE;

	m_allPossibleRoutes.calculateTheClosestRoutes( uiRowsCount, uiColumnsCount );

	return MAZEGENERATOR_SUCCEEDED;
}


CCellDirections ArrowMaze::CMazeGenerator::getPossibleDirections( const CCellBase& from ) const
{
	BYTE allDirections = 0xFF;

	if ( from.getRow() == 1 )
		allDirections &= ~( LEFT_UP | UP | RIGHT_UP );
	else if ( from.getRow() == m_pCreationParameters->getRowsCount() )
		allDirections &= ~( RIGHT_DOWN | DOWN | LEFT_DOWN );

	if ( from.getColumn() == 1 )
		allDirections &= ~( LEFT_DOWN | LEFT | LEFT_UP );
	else if ( from.getColumn() == m_pCreationParameters->getColumnsCount() )
		allDirections &= ~( RIGHT_UP | RIGHT | RIGHT_DOWN );

	return CCellDirections( allDirections );
}


CellDirection ArrowMaze::CMazeGenerator::findDirection( const CCellBase& from, const CCellBase& to ) const
{
	if ( from == to )
	return NO_DIRECTION;

	if ( from.getRow() == to.getRow() )
		return ( from.getColumn() > to.getColumn() ) ? LEFT : RIGHT;
	else if ( from.getColumn() == to.getColumn() )
		return ( from.getRow() > to.getRow() ) ? DOWN : UP;

	UINT uiSlope = ( from.getColumn() - to.getColumn() ) / ( from.getRow() - to.getRow() );
	if ( uiSlope == 1 )
		return ( from.getRow() > to.getRow() ) ? LEFT_UP : RIGHT_DOWN;
	else if ( uiSlope == -1 )
		return ( from.getRow() > to.getRow() ) ? RIGHT_UP : LEFT_DOWN;

	return NO_DIRECTION;
}


ArrowMaze::CMazeGenerator::CellsList ArrowMaze::CMazeGenerator::getCellsPointedBy( const CCellBase& from ) const
{
	CMazeGenerator::CellsList lst;
	const CellDirection& direction = from.getDirection();
	short int iHorizontalCoefficient = 
		direction & ( RIGHT_UP | RIGHT | RIGHT_DOWN ) ?  1 :
		direction & ( LEFT_DOWN | LEFT | LEFT_UP )    ? -1 : 0;
	short int iVerticalCoefficient = 
		direction & ( LEFT_UP | UP | RIGHT_UP )       ? -1 :
		direction & ( RIGHT_DOWN | DOWN | LEFT_DOWN ) ?  1 : 0;

	UINT uiCurrentRow = from.getRow();
	UINT uiCurrentColumn = from.getColumn();

	const UINT& cuiRowsCount = m_pCreationParameters->getRowsCount();
	const UINT& cuiColumnsCount = m_pCreationParameters->getColumnsCount();
	
	// don't include currentCell, so proceed to the next one
	uiCurrentRow += iVerticalCoefficient;
	uiCurrentColumn += iHorizontalCoefficient;
	
	while
		(
		( uiCurrentRow <= cuiRowsCount && uiCurrentRow > 0 )
		&&
		( uiCurrentColumn <= cuiColumnsCount && uiCurrentColumn > 0 )
		)
	{
		lst.push_back( getCell(uiCurrentRow, uiCurrentColumn) );
		uiCurrentRow += iVerticalCoefficient;
		uiCurrentColumn += iHorizontalCoefficient;
	}

	return lst;
}


ArrowMaze::CMazeGenerator::ListCollection ArrowMaze::CMazeGenerator::findInvalidSequences() const
{
	ListCollection out;

	const UINT& uiRowsCount = m_pCreationParameters->getRowsCount();
	const UINT& uiColumnsCount = m_pCreationParameters->getColumnsCount();

	CCellBase fakeCell;
	CellsList cellSequence;
	CellDirection directionToTest;

	// checking the rows:
	directionToTest = RIGHT;
	for ( UINT uiRow = 1; uiRow <= uiRowsCount; uiRow++ )
	{
		fakeCell = *getCell( uiRow, 1 );
		fakeCell.setDirection( directionToTest );
		cellSequence = getCellsAlong( fakeCell );
		if ( isInvalidSequence( cellSequence, &directionToTest ) )
			out.push_back( cellSequence );
	}

	// checking the columns:
	directionToTest = DOWN;
	for ( UINT uiColumn = 1; uiColumn <= uiColumnsCount; uiColumn++ )
	{
		fakeCell = *getCell( 1, uiColumn );
		fakeCell.setDirection( directionToTest );
		cellSequence = getCellsAlong( fakeCell );
		if ( isInvalidSequence( cellSequence, &directionToTest ) )
			out.push_back( cellSequence );
	}

	// checking the forwardOblique cells(/): row = (-1*col) + c
	// upper half of grid
	directionToTest = RIGHT_UP;
	for ( UINT uiRow = 2; uiRow <= uiRowsCount; uiRow++ )
	{
		fakeCell = *getCell( uiRow, 1 );
		fakeCell.setDirection( directionToTest );
		cellSequence = getCellsAlong( fakeCell );
		if ( isInvalidSequence( cellSequence, &directionToTest ) )
			out.push_back( cellSequence );
	}
	// lower half of grid
	for ( UINT uiColumn = 2; uiColumn <= uiColumnsCount -1; uiColumn++ )
	{
		fakeCell = *getCell( uiRowsCount, uiColumn );
		fakeCell.setDirection( directionToTest );
		cellSequence = getCellsAlong( fakeCell );
		if ( isInvalidSequence( cellSequence, &directionToTest ) )
			out.push_back( cellSequence );
	}


	// checking the backwardOblique cells(\): row = (+1*col) + c
	// upper half of grid
	directionToTest = LEFT_UP;
	for ( UINT uiRow = 2; uiRow <= uiRowsCount; uiRow++ )
	{
		fakeCell = *getCell( uiRow, uiColumnsCount );
		fakeCell.setDirection( directionToTest );
		cellSequence = getCellsAlong( fakeCell );
		if ( isInvalidSequence( cellSequence, &directionToTest ) )
			out.push_back( cellSequence );
	}
	// lower half of grid
	for ( UINT uiColumn = uiColumnsCount -1; uiColumn > 1; uiColumn-- )
	{
		fakeCell = *getCell( uiRowsCount, uiColumn );
		fakeCell.setDirection( directionToTest );
		cellSequence = getCellsAlong( fakeCell );
		if ( isInvalidSequence( cellSequence, &directionToTest ) )
			out.push_back( cellSequence );
	}

	return out;
}


bool ArrowMaze::CMazeGenerator::isInvalidSequence( const CellsList& list, const CellDirection* pDirection ) const
{
	const CCellBase* pTempCell = 0;
	CellDirection currentCellDir = NO_DIRECTION;
	CellDirection forwardDir = pDirection ? *pDirection : list.front()->getDirection();
	CellDirection oppositeDir = getOppositeDirection( forwardDir );
	for ( CellsList::const_iterator cIter = list.cbegin(); cIter != list.cend(); cIter++ )
	{
		pTempCell = getCell( (*cIter)->getRow(), (*cIter)->getColumn() );
		currentCellDir = pTempCell->getDirection();
		if ( (currentCellDir != forwardDir && currentCellDir != oppositeDir) )
			return false; // this sequence of cells is not an 'Invalid Sequence'
	}
	return true;
}


ArrowMaze::CMazeGenerator::CellsList ArrowMaze::CMazeGenerator::getCellsAlong( const CCellBase& cell ) const
{
	CMazeGenerator::CellsList lst;
	const CellDirection& direction = cell.getDirection();
	short int iHorizontalCoefficient = 
		direction & ( RIGHT_UP | RIGHT | RIGHT_DOWN ) ?  1 :
		direction & ( LEFT_DOWN | LEFT | LEFT_UP )    ? -1 : 0;
	short int iVerticalCoefficient = 
		direction & ( LEFT_UP | UP | RIGHT_UP )       ? -1 :
		direction & ( RIGHT_DOWN | DOWN | LEFT_DOWN ) ?  1 : 0;

	UINT uiCurrentRow = cell.getRow();
	UINT uiCurrentColumn = cell.getColumn();

	const UINT& cuiRowsCount = m_pCreationParameters->getRowsCount();
	const UINT& cuiColumnsCount = m_pCreationParameters->getColumnsCount();
	while
		(
		( uiCurrentRow <= cuiRowsCount && uiCurrentRow > 0 )
		&&
		( uiCurrentColumn <= cuiColumnsCount && uiCurrentColumn > 0 )
		)
	{
		lst.push_back( getCell(uiCurrentRow, uiCurrentColumn) );
		uiCurrentRow += iVerticalCoefficient;
		uiCurrentColumn += iHorizontalCoefficient;
	}

	return lst;
}


ArrowMaze::CMazeGenerator::CellsList ArrowMaze::CMazeGenerator::findRepeatativeCells( const ListCollection& listCollection ) const
{
	CellsList out;
	UINT* arrTestGrid = 0;
	const UINT& uiRowsCount = m_pCreationParameters->getRowsCount();
	const UINT& uiColumnsCount = m_pCreationParameters->getColumnsCount();

	ASSERT_MEMORYALLOCATION( arrTestGrid = new UINT[ uiRowsCount * uiColumnsCount ] );

	for ( ListCollection::const_iterator cIter = listCollection.cbegin(); cIter != listCollection.cend(); cIter++ )
		for ( CellsList::const_iterator cIterCells = cIter->cbegin(); cIterCells != cIter->cend(); cIterCells++ )
			arrTestGrid[ (*cIterCells)->getRow() * uiColumnsCount + (*cIterCells)->getColumn() ]++;

	for ( UINT uiRow = 0; uiRow < uiRowsCount; uiRow++ )
		for ( UINT uiColumn = 0; uiColumn < uiColumnsCount; uiColumn++ )
			if ( arrTestGrid[ uiRow * uiColumnsCount + uiColumn ] > 1 )
				out.push_back( getCell( uiRow, uiColumn ) );

	return out;
}


ArrowMaze::CRoutesList ArrowMaze::CMazeGenerator::getAllPossibleRoutes() const
{
	class stackItem
	{
	public:
		stackItem( const CCellBase* pCell, const UINT& uiIndex = 0 )
			: m_pCell( pCell ), m_uiIndex( uiIndex ) {}

		inline const UINT& getIndex() const { return m_uiIndex; }
		inline UINT& getIndex() { return m_uiIndex; }

		inline const CCellBase* getCell() const { return m_pCell; }
	
	private:
		const CCellBase* m_pCell;
		UINT m_uiIndex;
	};

	std::list<stackItem*> stack;
	CRoutesList lstRoutes;
	CRoute currentRoute;

	const CCellBase& HomeCell = m_pCreationParameters->getHomeCell();
	const CCellBase& DestinationCell = m_pCreationParameters->getDestinationCell();

	// add the last node (destinationCell) to the route
	const CCellBase* cpCurrentCell = &DestinationCell;
	currentRoute.pushBack( cpCurrentCell );

	// add the first cell which is, directly pointing to the 'destination cell'
	CellsList listOfCellsPointingToCurrentCell = getNeighborhoodCellsPointingTo( *cpCurrentCell );
	cpCurrentCell = listOfCellsPointingToCurrentCell.front();
	currentRoute.pushBack( cpCurrentCell );

	// now iterate through the maze
	stack.push_back( new stackItem( cpCurrentCell ) );
	while( stack.size() )
	{
		while( stack.back()->getIndex() < ( listOfCellsPointingToCurrentCell = getNeighborhoodCellsPointingTo_Performace(*stack.back()->getCell()) ).size() )
		{
			cpCurrentCell = getCellByIndex( listOfCellsPointingToCurrentCell, stack.back()->getIndex()++ );

			if ( !currentRoute.cellExists( cpCurrentCell ) )
			{
				currentRoute.pushBack( cpCurrentCell );
				if ( *cpCurrentCell == HomeCell )
				{
					lstRoutes.pushRoute( currentRoute.getReverseRoute() );
					currentRoute.pop();
					continue;
				}
				stack.push_back( new stackItem( cpCurrentCell ) );
			}
		}

		delete stack.back();
		stack.pop_back();
		currentRoute.pop();
	}

	return lstRoutes;
}


const ArrowMaze::CCellBase* ArrowMaze::CMazeGenerator::getCellByIndex( const CellsList& list, const UINT& uiIndex ) const
{
	ASSERT( list.size() > uiIndex, "Out of range!" );
	CellsList::const_iterator cIter = list.cbegin();
	for ( UINT ui = 0; ui < uiIndex; ui++ )
		cIter++;
	return *cIter;
}


ArrowMaze::CMazeGenerator::CellsList ArrowMaze::CMazeGenerator::getNeighborhoodCellsPointingTo( const CCellBase& cell ) const
{
	CellsList out, lstTempCells;
	CCellBase fakeCell( cell );

	CellDirection forwardDir = NO_DIRECTION;
	CellDirection oppositeDir = NO_DIRECTION;
	CCellDirections possibleDirs = getPossibleDirections( cell );
	const UINT& possibleDirsCount = possibleDirs.getDirectionsCount();
	for ( UINT ui = 0; ui < possibleDirsCount; ui++ )
	{
		forwardDir = possibleDirs.getDirection( ui );
		oppositeDir = getOppositeDirection( forwardDir );
		fakeCell.setDirection( forwardDir );
		lstTempCells = getCellsPointedBy( fakeCell );

		for( CellsList::const_iterator cIter = lstTempCells.cbegin(); cIter != lstTempCells.cend(); cIter++ )
			if ( (*cIter)->getDirection() == oppositeDir )
				out.push_back( *cIter );
	}
	
	return out;
}