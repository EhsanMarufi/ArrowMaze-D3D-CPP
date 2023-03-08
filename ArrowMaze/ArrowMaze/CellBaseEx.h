// In the name of God
// Ehsan Marufi Azar, 2013 (c)

#pragma once
#ifndef CELLBASEEX_H
#define CELLBASEEX_H


#include "CellBase.h"
typedef unsigned int UINT;


namespace ArrowMaze
{

	class CCellBaseEx : public CCellBase
	{
	public:
		CCellBaseEx();
		CCellBaseEx( const UINT& uiRow, const UINT& uiColumn, const CellDirection& direction = NO_DIRECTION );
		CCellBaseEx( const CCellBaseEx& copy );
		~CCellBaseEx( void );


		// inline getters and setters
		inline const bool& isCellMet() const { return m_bIsMet; }
		inline void setCellMet( const bool& bIsMet ) { m_bIsMet = bIsMet; }

		inline const bool/*&*/ isCellOnAPossibleRoute() const { /*TODO:*/return false; }


	protected:
		inline void setCellIsOnAPossibleRoute( const bool& val ) { /*TODO*/ }

	private:
		bool m_bIsMet;

	}; // end of class 'CCellBaseEx'
} // end of namespace 'ArrowMaze'
#endif // CCELLBASEEX_H
