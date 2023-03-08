// In the name of God
// Ehsan Marufi Azar, 2013
// v1.2 Jan 2013

#pragma once;
#ifndef DEVICEACCESSOR_H
#define DEVICEACCESSOR_H

#include <d3dx9.h>
#include "Assertion.h"

namespace Direct3D
{

	class CDeviceAccessor
	{
	public:
		CDeviceAccessor() {}
		~CDeviceAccessor() {}


		static inline void setDevice( LPDIRECT3DDEVICE9 pD3DDevice )
		{
			ASSERT_D3DDEVICE( s_pD3DDevice = pD3DDevice ); 
		}

		static inline LPDIRECT3DDEVICE9 getDevice()
		{ 
			return s_pD3DDevice;
		}

	protected:
		static LPDIRECT3DDEVICE9 s_pD3DDevice;

	}; // end of class 'CDeviceAccessor'
} // end of namespace 'Direct3D'

#endif // DEVICEACCESSOR_H