// In the name of God
// Ehsan Marufi Azar, 2013 (c)
// All rights reserved.

#pragma once
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "Grid.h"
#include <d3d9.h>
#include "hr_Exception.h"
#include "BaseCamera.h"
#include "Widget.h"
#include "MeshWidget.h"
#include "Lights.h"
#include "ConstantValues.h"
#include "DeviceAccessor.h"
#include "Cell.h"


#define REGENERATION_MAX (3)


class CGameLogic
	: public Direct3D::CDeviceAccessor
{
public:
	CGameLogic( void );
	~CGameLogic( void );

	HRESULT onD3DDeviceCreate( LPDIRECT3DDEVICE9 lpD3DDevice );
	void onKeyDown( const WPARAM& wParam );

	void Draw();

private:
	Direct3D::CBaseCamera* m_pCamera;
	ArrowMaze::CGrid* m_pGrid;
	void initializeGridObject();
	const ArrowMaze::CCell* m_cpCurrentCell;
};

#endif // GAMELOGIC_H