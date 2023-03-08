// In the name of God
// Ehsan Marufi Azar, 2013

#pragma once
#ifndef CELLDIRECTIONS_H
#define CELLDIRECTIONS_H

#include "stdafx.h"
#include <stdlib.h>
#include "Assertion.h"
#include "safe.h"

#define COUNT_OF_AVAILABLE_DIRECTIONS (8)
typedef unsigned int UINT;

namespace ArrowMaze
{
	typedef enum _CellDirection
	{
		LEFT  = 1,			LEFT_UP    = 1 << 1,
		UP    = 1 << 2,		RIGHT_UP   = 1 << 3,
		RIGHT = 1 << 4,		RIGHT_DOWN = 1 << 5,
		DOWN  = 1 << 6,		LEFT_DOWN  = 1 << 7,
		NO_DIRECTION = 0
	} CellDirection;


	CellDirection getOppositeDirection( const CellDirection& direction );
	UINT getDirectionOrdinal( const CellDirection& direction );


	class CCellDirections
	{
	public:
		CCellDirections( const BYTE& directions );
		CCellDirections( const CCellDirections& copy );
		~CCellDirections(void);

		void setDirections( const BYTE& directions );
		inline const UINT& getDirectionsCount() const { return m_uiArrayLength; }
		const CellDirection& getDirection( const UINT& uiIndex ) const;
		bool hasDirection( const CellDirection& direction, UINT* puiIndex = 0 ) const;

		void addDirections( const CCellDirections& directions );
		void removeDirections( const CCellDirections& directions );
		
		CellDirection selectRandomDirection() const;

		
		operator BYTE() const;
		const CCellDirections& operator= ( const CCellDirections& right );
		bool operator== ( const CCellDirections& right ) const;
		inline bool operator!= ( const CCellDirections& right ) const { return !( *this == right ); }

	protected:
		UINT getDirectionsCount ( const BYTE& directions ) const;

	private:
		CellDirection* m_arrDirections;
		UINT m_uiArrayLength;
	}; // end of class CCellDirections

} // end of namespame 'ArrowMaze'
#endif // CELLDIRECTIONS_H