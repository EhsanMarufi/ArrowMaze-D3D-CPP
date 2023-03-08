// In the name of God
// Ehsan Marufi Azar, 2012 (c)
// v1.0 Sep 2012

#pragma once
#include <d3dx9.h>

namespace D3DConstants
{
	extern D3DMATERIAL9 InitMaterial
		(
		const D3DCOLORVALUE& ambient,
		const D3DCOLORVALUE& diffuse,
		const D3DCOLORVALUE& specular,
		const D3DCOLORVALUE& emissive,
		const float& power
		);

	extern const D3DXCOLOR WHITE;
	extern const D3DXCOLOR RED;
	extern const D3DXCOLOR GREEN;
	extern const D3DXCOLOR BLUE;
	extern const D3DXCOLOR CYAN;
	extern const D3DXCOLOR MAGENTA;
	extern const D3DXCOLOR YELLOW;
	extern const D3DXCOLOR BLACK;

	extern const D3DMATERIAL9 WHITE_MTRL;
	extern const D3DMATERIAL9 RED_MTRL;
	extern const D3DMATERIAL9 GREEN_MTRL;
	extern const D3DMATERIAL9 BLUE_MTRL;
	extern const D3DMATERIAL9 CYAN_MTRL;
	extern const D3DMATERIAL9 MAGENTA_MTRL;
	extern const D3DMATERIAL9 YELLOW_MTRL;
	extern const D3DMATERIAL9 BLACK_MTRL;

	extern const D3DMATERIAL9 SHINY_RED_MTRL;
	extern const D3DMATERIAL9 SHINY_BLUE_MTRL;

#ifndef _2PI
#define _2PI (6.283185307179586476925286766559f)
#endif
#ifndef _PI_2
#define _PI_2 (1.5707963267948966192313216916398f)
#endif
}; // end of namespace 'D3DConstants'