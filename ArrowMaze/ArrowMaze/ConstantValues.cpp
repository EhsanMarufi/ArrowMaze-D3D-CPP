#include "ConstantValues.h"

using namespace D3DConstants;

D3DMATERIAL9 D3DConstants::InitMaterial(
	const D3DCOLORVALUE& ambient,
	const D3DCOLORVALUE& diffuse,
	const D3DCOLORVALUE& specular,
	const D3DCOLORVALUE& emissive,
	const float& power)
{
	D3DMATERIAL9 out;
	out.Ambient = ambient;
	out.Diffuse = diffuse;
	out.Specular = specular;
	out.Emissive = emissive;
	out.Power = power;
	return out;
}

const D3DXCOLOR D3DConstants::WHITE		(D3DCOLOR_XRGB(255, 255, 255));
const D3DXCOLOR D3DConstants::RED		(D3DCOLOR_XRGB(255,   0,   0));
const D3DXCOLOR D3DConstants::GREEN		(D3DCOLOR_XRGB(  0, 255,   0));
const D3DXCOLOR D3DConstants::BLUE		(D3DCOLOR_XRGB(  0,   0, 255));
const D3DXCOLOR D3DConstants::CYAN		(D3DCOLOR_XRGB(  0, 255, 255));
const D3DXCOLOR D3DConstants::MAGENTA	(D3DCOLOR_XRGB(255,   0, 255));
const D3DXCOLOR D3DConstants::YELLOW	(D3DCOLOR_XRGB(255, 255,   0));
const D3DXCOLOR D3DConstants::BLACK		(D3DCOLOR_XRGB(  0,   0,   0));


extern const D3DMATERIAL9 D3DConstants::WHITE_MTRL = InitMaterial (WHITE, WHITE, WHITE, BLACK, 8.0f);
extern const D3DMATERIAL9 D3DConstants::RED_MTRL = InitMaterial (RED, RED, RED, BLACK, 8.0f);
extern const D3DMATERIAL9 D3DConstants::GREEN_MTRL = InitMaterial (GREEN, GREEN, GREEN, BLACK, 8.0f);
extern const D3DMATERIAL9 D3DConstants::BLUE_MTRL = InitMaterial (BLUE, BLUE, BLUE, BLACK, 8.0f);
extern const D3DMATERIAL9 D3DConstants::CYAN_MTRL = InitMaterial (CYAN, CYAN, CYAN, BLACK, 8.0f);
extern const D3DMATERIAL9 D3DConstants::MAGENTA_MTRL = InitMaterial (MAGENTA, MAGENTA, MAGENTA, BLACK, 8.0f);
extern const D3DMATERIAL9 D3DConstants::YELLOW_MTRL = InitMaterial (YELLOW, YELLOW, YELLOW, BLACK, 8.0f);
extern const D3DMATERIAL9 D3DConstants::BLACK_MTRL = InitMaterial (BLACK, BLACK, BLACK, BLACK, 8.0f);

extern const D3DMATERIAL9 D3DConstants::SHINY_RED_MTRL = InitMaterial (RED, BLACK, WHITE, BLACK, 8.0f);
extern const D3DMATERIAL9 D3DConstants::SHINY_BLUE_MTRL = InitMaterial (WHITE, WHITE, WHITE, BLUE, 8.0f);
