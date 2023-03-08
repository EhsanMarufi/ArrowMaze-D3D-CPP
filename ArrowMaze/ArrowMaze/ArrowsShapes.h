// In the name of God
// Ehsan Marufi Azar, 2013
// Feb 2013

#pragma once;
#ifndef ARROWSSHAPES_H
#define ARROWSSHAPES_H

#include "DeviceAccessor.h"
#include "ArrowShape.h"
#include "CellDirection.h"
#include "Assertion.h"
#include "safe.h"
#include "hr_Exception.h"
#include <map>
#include <d3d9.h>


namespace ArrowMaze
{

	class CArrowShapesVertexBuffers
		: public Direct3D::CDeviceAccessor
	{
	public:
		CArrowShapesVertexBuffers() {}
		~CArrowShapesVertexBuffers() {}


		static void createAllArrowShapesSquareVertexBuffers();
		static void releaseAllArrowShapesSquareVertexBuffers();

		static inline LPDIRECT3DVERTEXBUFFER9 getVertexBuffer( const CellDirection& direction )
		{
			return s_lstVertexBuffers.at( direction );
		}


	protected:
		typedef std::map<CellDirection, LPDIRECT3DVERTEXBUFFER9> ListOfVertexBuffers;
		static ListOfVertexBuffers s_lstVertexBuffers;

	}; // end of struct 'MemoryHelper'
}// end of namespace 'ArrowMaze'
#endif //ARROWSSHAPES_H