// In the name of God
// Ehsan Marufi Azar, 2013 (c)

#pragma once
#ifndef ROUTE_H
#define ROUTE_H

#include "stdafx.h"
#include <list>
#include "CellBase.h"
#include "Assertion.h"


namespace ArrowMaze
{

	class CRoute
	{
	public:
		typedef std::list<CCellBase*>::iterator ListIterator;
		typedef std::list<CCellBase*>::const_iterator constListIterator;

	public:
		CRoute( void );
		CRoute( const CRoute& copy );
		~CRoute( void );

		inline const CCellBase* getFront() const { return m_lstNodes.front(); }
		inline const CCellBase* getBack() const { return m_lstNodes.back(); }
		void pushBack( const CCellBase* pNode );
		void pushFront( const CCellBase* pNode );
		void pop();


		void getConstIterator( constListIterator& cBegin, constListIterator& cEnd ) const;
		void getIterator( ListIterator& begin, ListIterator& end );
		CRoute getReverseRoute();
		
		inline const UINT length() const { return m_lstNodes.size(); }
		CCellBase* getNode( const UINT& uiNodeIndex ) const;

		bool cellExists( const CCellBase* cell ) const;

	private:
		std::list<CCellBase*> m_lstNodes;
	};// end of class 'CRoute'

} // end of namespace 'ArrowMaze'
#endif //ROUTE_H