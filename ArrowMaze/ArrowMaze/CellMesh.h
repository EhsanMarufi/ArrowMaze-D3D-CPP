// In the name of God
// Ehsan Marufi Azar, 2013 (c)
// This class is just an encapsulator

#pragma once
#ifndef CELLMESH_H
#define CELLMESH_H


#include "MeshFile.h"
#include "DeviceAccessor.h"
#include "RenderStateManagement.h"
#include "Assertion.h"
#include "safe.h"

#include "Lights.h"

namespace ArrowMaze
{

	class CCellMesh
		: public Direct3D::CDeviceAccessor
	{
	public:
		CCellMesh() {}
		~CCellMesh() {}


		void Draw();


		// static section
		static void createCellMeshSharedObject();
		static void releaseCellMeshSharedObject();

		static Direct3D::CMeshFile* getCellMesh() { return s_pCellMesh; }

	protected:
		static Direct3D::CMeshFile* s_pCellMesh;
		static Direct3D::CRenderStateManagement* s_pRenderStates;
		static Direct3D::DirectionalLight* s_pDirectionLight;
		static D3DMATERIAL9* s_pMaterial;

	}; // end of Class CCellMesh
} // end of namespace 'ArrowMaze'

#endif // CELLMESH_H