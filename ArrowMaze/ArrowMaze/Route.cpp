#include "Route.h"

using namespace ArrowMaze;

ArrowMaze::CRoute::CRoute(void)
{

}


ArrowMaze::CRoute::CRoute( const ArrowMaze::CRoute& copy )
{
	constListIterator cIter, cEnd;
	copy.getConstIterator( cIter, cEnd );
	for ( ; cIter != cEnd; cIter++ )
		m_lstNodes.push_front( *cIter );
}


ArrowMaze::CRoute::~CRoute( void )
{
}


void ArrowMaze::CRoute::pushBack( const ArrowMaze::CCellBase* node )
{
	ASSERT_POINTER( node );
	m_lstNodes.push_back( const_cast<CCellBase*>(node) );
}


void ArrowMaze::CRoute::pushFront( const ArrowMaze::CCellBase* node )
{
	ASSERT_POINTER( node );
	m_lstNodes.push_front( const_cast<CCellBase*>(node) );
}


void ArrowMaze::CRoute::pop()
{
	m_lstNodes.pop_back();
}


ArrowMaze::CCellBase* ArrowMaze::CRoute::getNode( const UINT& uiNodeIndex ) const
{
	ASSERT( uiNodeIndex < length(), "Out of range!" );
	UINT uiCurrentIndex = 0;
	for ( constListIterator cIter = m_lstNodes.cbegin(); cIter != m_lstNodes.cend(); cIter++ )
		if( uiCurrentIndex++ == uiNodeIndex )
			return *cIter;
	return 0;
}


void ArrowMaze::CRoute::getConstIterator( constListIterator& cBegin, constListIterator& cEnd ) const
{
	cBegin = m_lstNodes.cbegin();
	cEnd = m_lstNodes.cend();
}


void ArrowMaze::CRoute::getIterator( ListIterator& begin, ListIterator& end )
{
	begin = m_lstNodes.begin();
	end = m_lstNodes.end();
}


bool ArrowMaze::CRoute::cellExists( const ArrowMaze::CCellBase* node ) const
{
	ASSERT_POINTER( node );
	for ( constListIterator cIter = m_lstNodes.cbegin(); cIter != m_lstNodes.cend(); cIter++ )
		if ( *node == **cIter )
			return true;
	return false;
}


ArrowMaze::CRoute ArrowMaze::CRoute::getReverseRoute()
{
	CRoute out;
	for ( std::list<CCellBase*>::const_reverse_iterator crIter = m_lstNodes.crbegin(); crIter != m_lstNodes.crend(); crIter++ )
		out.pushBack( *crIter );
	return out;
}