//====================================================================================================
//Unfortunatly it has to be implemented in 'C_D3DSpacialEntity.h'! Coz of inability of the compiler to find the class
//and instanciate the template class outside the scope of file!
#include "C_D3DSpacialEntity.h"

SpacialEntity::_DEFAULTS Defaults;

const DWORD SpacialEntity::SimpleVertex::FVF = D3DFVF_XYZ /*| D3DFVF_DIFFUSE | D3DFVF_NORMAL*/ | D3DFVF_TEX1;