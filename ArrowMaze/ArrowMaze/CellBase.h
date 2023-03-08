// In the name of God
// Ehsan Marufi Azar, 2013 (c)

#pragma once
#ifndef CELLBASE_H
#define CELLBASE_H

#include "stdafx.h"
#include "CellDirection.h"

namespace ArrowMaze
{

	class CCellBase
	{
	public:
		CCellBase( void );
		CCellBase( const UINT& uiRow, const UINT& uiColumn, const CellDirection& direction = NO_DIRECTION);
		CCellBase( const CCellBase& copy );
		~CCellBase( void );

		inline const UINT& getRow() const { return m_uiRow; }
		inline const UINT& getColumn() const { return m_uiColumn; }
		inline const CellDirection& getDirection() const { return m_CellDirection; }

		inline void setRow( const UINT& uiRow ) { m_uiRow = uiRow; }
		inline void setColumn ( const UINT& uiColumn ) { m_uiColumn = uiColumn; }
		inline void setDirection ( const CellDirection& direction ) { m_CellDirection = direction; }

		inline bool operator!= ( const CCellBase& right ) const { return !(*this == right); }
		inline bool operator== ( const CCellBase& right ) const { return m_uiRow == right.getRow() && m_uiColumn == right.getColumn(); }

	protected:
		UINT m_uiRow, m_uiColumn;
		CellDirection m_CellDirection;
	}; // end of class 'CCellBase'

} // end of namespace 'ArrowMaze'
#endif //CELLBASE_H