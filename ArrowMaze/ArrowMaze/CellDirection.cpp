#include "CellDirection.h"

using namespace ArrowMaze;

// function for getting the opposite direction
ArrowMaze::CellDirection ArrowMaze::getOppositeDirection( const CellDirection& direction )
{
	CellDirection oppositeDirection = NO_DIRECTION;

#define CASE( direction, opposite ) \
	case direction: \
	oppositeDirection = opposite; \
	break; \
	case opposite: \
	oppositeDirection = direction; \
	break

	switch ( direction )
	{
		CASE ( LEFT, RIGHT );
		CASE ( LEFT_UP, RIGHT_DOWN );
		CASE ( UP, DOWN );
		CASE ( RIGHT_UP, LEFT_DOWN );
	}

#undef CASE
	return oppositeDirection;
}


// function for retriving the ordinal of the direction within the texture
UINT ArrowMaze::getDirectionOrdinal( const CellDirection& direction )
{
#define CASE(dir, ordinal) case dir: return ordinal
	switch( direction )
	{
		CASE( NO_DIRECTION, 1 );
		CASE( LEFT, 2 );
		CASE( LEFT_UP, 3 );
		CASE( UP, 4 );
		CASE( RIGHT_UP, 5 );
		CASE( RIGHT, 6 );
		CASE( RIGHT_DOWN, 7 );
		CASE( DOWN, 8 );
		CASE( LEFT_DOWN, 9 );
	default:
		ASSERT( false, "Wrong Direction!" );
		return 0;
	}
#undef CASE
}



ArrowMaze::CCellDirections::CCellDirections( const BYTE& directions )
{
	m_arrDirections = 0;
	setDirections( directions );
}


ArrowMaze::CCellDirections::CCellDirections( const ArrowMaze::CCellDirections& copy )
{
	m_uiArrayLength = copy.getDirectionsCount();
	
	m_arrDirections = 0;
	ASSERT_MEMORYALLOCATION( m_arrDirections = new CellDirection[ m_uiArrayLength ] );

	for ( UINT ui = 0; ui < m_uiArrayLength; ui++ )
		m_arrDirections[ ui ] = copy.getDirection( ui );
}


ArrowMaze::CCellDirections::~CCellDirections(void)
{
	SAFE_DELETE_ARRAY( m_arrDirections );
}


void ArrowMaze::CCellDirections::setDirections( const BYTE& directions )
{
	SAFE_DELETE_ARRAY( m_arrDirections );

	m_uiArrayLength = getDirectionsCount( directions );
	
	m_arrDirections = 0;
	ASSERT_MEMORYALLOCATION( m_arrDirections = new CellDirection[ m_uiArrayLength ] );
	
	UINT uiOrdinal = 0;
	for ( UINT ui = 0; ui < COUNT_OF_AVAILABLE_DIRECTIONS; ui++ )
		if ( directions & ( 1 << ui ) )
			m_arrDirections[uiOrdinal++] = CellDirection( 1 << ui );
}


UINT ArrowMaze::CCellDirections::getDirectionsCount ( const BYTE& directions ) const
{
	UINT uiCount = 0;
	for ( UINT ui = 0; ui < COUNT_OF_AVAILABLE_DIRECTIONS; ui++ )
		if ( directions & ( 1 << ui ) )
			uiCount++;
	return uiCount;
}


const CellDirection& ArrowMaze::CCellDirections::getDirection( const UINT& uiIndex ) const
{
	ASSERT( uiIndex < m_uiArrayLength, "Out of range!" );
	return m_arrDirections[ uiIndex ];
}


bool ArrowMaze::CCellDirections::hasDirection( const CellDirection& direction, UINT* puiIndex ) const
{
	for ( UINT ui = 0; ui < m_uiArrayLength; ui++ )
		if ( getDirection( ui ) == direction )
		{
			if ( puiIndex )
				*puiIndex = ui;

			return true;
		}

	return false;
}


ArrowMaze::CellDirection ArrowMaze::CCellDirections::selectRandomDirection() const
{
	const UINT uiRandomNumber = (UINT)( rand() % m_uiArrayLength );
	return getDirection( uiRandomNumber );
}


ArrowMaze::CCellDirections::operator BYTE () const
{
	BYTE out = 0;
	for ( UINT ui = 0; ui < m_uiArrayLength; ui++ )
		out |= getDirection( ui );
	return out;
}


const CCellDirections& ArrowMaze::CCellDirections::operator= ( const CCellDirections& right )
{
	setDirections( right );
	return *this;
}


bool ArrowMaze::CCellDirections::operator== ( const CCellDirections& right ) const
{
	return (BYTE)*this == (BYTE)right;
}


void ArrowMaze::CCellDirections::addDirections( const CCellDirections& directions )
{
	setDirections( (*this) | directions );
}


void ArrowMaze::CCellDirections::removeDirections( const CCellDirections& directions )
{
	setDirections( (*this) & ~directions );
}