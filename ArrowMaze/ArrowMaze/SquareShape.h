// In the name of God
// Ehsan Marufi Azar, (c) 2013
// v1.1 Feb 2013

#pragma once
#ifndef SQUARESHAPE_H
#define SQUARESHAPE_H

#include "hr_Exception.h"
#include "safe.h"
#include "Assertion.h"
#include <d3d9.h> // for D3DFVF_XYZ, D3DFVF_TEX1

typedef unsigned int UINT;


namespace ArrowMaze
{

	struct SquareShapeVertex
	{
		SquareShapeVertex()
		{
			m_fX = m_fY = m_fZ = 0.0f;
			m_fU = m_fV = 0.0f;
		}

		SquareShapeVertex( const float& x, const float& y, const float& z, const float& u, const float& v )
		{
			m_fX = x; m_fY = y; m_fZ = z;
			m_fU = u; m_fV = v;
		}

		float m_fX, m_fY, m_fZ;
		float m_fU, m_fV;
		static const DWORD FVF;
	}; // end of struct 'SquareShapeVertex'


	class CSquareShape
	{
	public:
		CSquareShape( const float& fCenterX, const float& fSideLength = 15.0f );
		CSquareShape( const float& fCenterX, const UINT& uiTextureStripeIndex, const UINT& uiTextureStripeSectionsCount, const float& fSideLength = 15.0f );
		CSquareShape( const CSquareShape& copy );
		~CSquareShape( void );

		inline const SquareShapeVertex* getVerts() const { return m_pVerts; }
		inline const float& getSideLength() const { return m_fSideLength; }
		

		// static methodes
		inline static const WORD* getIndexBufferArray( unsigned int* pArrayLengthOut = 0 )
		{
			// if the user wants to know the length of IndexBuffer for this squareShape
			// return it through the parameter pointer.
			if ( pArrayLengthOut )
				*pArrayLengthOut = 6;

			return s_warrIndexBuffer;
		}

		inline static const float& getCreationZ() { return s_fZ; }
		inline static void setCreationZ( const float& z ) { s_fZ = z; }


	protected:
		float m_fSideLength;
		static float s_fZ;
		static WORD s_warrIndexBuffer[6];

		SquareShapeVertex* m_pVerts;

		void generateVerts( const float& fCenterX, const UINT& uiTextureStripeIndex = 0, const UINT& uiTextureStripeSectionsCount = 1 );

	}; // end of class 'CSquareShape'
} // end of namespace 'ArrowMaze'
#endif // end of 'SQUARESHAPE_H'