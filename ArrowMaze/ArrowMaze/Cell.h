// In the name of God
// Ehsan Marufi Azar, 2013

#pragma once
#ifndef CELL_H
#define CELL_H

#include "CellBaseEx.h"
#include "Widget.h"
#include "safe.h"
#include "Assertion.h"

#include "CellMesh.h"
#include "ArrowShape.h"
#include "CellOutterGlow.h"


namespace ArrowMaze
{

	class CCell
		: public CCellBaseEx, public Direct3D::CWidget
	{
	public:
		CCell( void );
		CCell( const CCell& copy );
		CCell( const UINT& uiRow, const UINT& uiColumn, const CellDirection& direction = NO_DIRECTION );
		~CCell( void );


		void Draw();

		// TODO: change to reflect the accurate cell's dimentions
		inline void getCellDimentions( float& fWidth, float& fHeight, float& fDepth ) const
		{
			fWidth  = 50.0f;
			fHeight = 50.0f;
			fDepth  = 10.0f;
		}

	protected:
		CCellMesh* m_pCellMesh;
		CArrowShape* m_pArrowShape;
		CCellOutterGlow* m_pOutterGlow;

	}; // end of class 'CCell'
} // end of namespcae 'ArrowMaze'
#endif // CELL_H