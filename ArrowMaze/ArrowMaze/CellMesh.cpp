#include "CellMesh.h"
#include "ConstantValues.h"

using namespace ArrowMaze;
using namespace Direct3D;


CMeshFile* CCellMesh::s_pCellMesh = 0;
CRenderStateManagement* CCellMesh::s_pRenderStates = 0;
DirectionalLight* CCellMesh::s_pDirectionLight = 0;
D3DMATERIAL9* CCellMesh::s_pMaterial = 0;


void CCellMesh::createCellMeshSharedObject()
{
	releaseCellMeshSharedObject();

	ASSERT_MEMORYALLOCATION( s_pCellMesh = new CMeshFile( "Media/X files/Cell.x" ) );

	// shared Cell mesh's settings
	ASSERT_MEMORYALLOCATION( s_pRenderStates = new CRenderStateManagement() );
	Render_State arrRenderStates[] = {
		{ D3DRS_LIGHTING, TRUE },
		{ D3DRS_SPECULARENABLE, TRUE }
	};
	Texture_Stage_State arrTextureStates[] = { // For stage 0
		{ D3DTSS_COLORARG1, D3DTA_SPECULAR },
		{ D3DTSS_COLORARG2, D3DTA_TEXTURE },
		//{ D3DTSS_COLORARG2, D3DTA_DIFFUSE },
		{ D3DTSS_COLOROP, D3DTOP_SUBTRACT }
	};
	Texture_Stage_State arrTextureStates2[] = { // For stage 1
		{ D3DTSS_COLORARG1, D3DTA_CURRENT },
		{ D3DTSS_COLORARG2, D3DTA_TEXTURE },
		//{ D3DTSS_COLORARG2, D3DTA_DIFFUSE },
		{ D3DTSS_COLOROP, D3DTOP_ADDSIGNED2X }
	};
	s_pRenderStates->setStates( arrRenderStates, NUM_OF( arrRenderStates ) );
	s_pRenderStates->setStates( 0, arrTextureStates, NUM_OF( arrTextureStates ) );
	s_pRenderStates->setStates( 1, arrTextureStates2, NUM_OF( arrTextureStates2 ) );
	s_pRenderStates->createStateBlock();

	// setting up the directional light
	D3DXVECTOR3 dir( 0.0f, 0.0f, -1.0f );
	D3DXCOLOR lightColor = D3DConstants::WHITE;
	ASSERT_MEMORYALLOCATION( s_pDirectionLight = new DirectionalLight( lightColor, dir ) );
	//s_pDirectionLight->m_AmbientColor = D3DConstants::WHITE;
	s_pDirectionLight->m_SpecularColor = D3DConstants::WHITE;
	s_pDirectionLight->m_DiffuseColor = D3DConstants::WHITE;

	// setting up the material
	ASSERT_MEMORYALLOCATION( s_pMaterial = new D3DMATERIAL9() );
	*s_pMaterial = D3DConstants::WHITE_MTRL;
	s_pMaterial->Specular = D3DConstants::WHITE;
	s_pMaterial->Power = 3.0f;

}


void CCellMesh::releaseCellMeshSharedObject()
{
	SAFE_DELETE( s_pCellMesh );
	SAFE_DELETE( s_pRenderStates );

	SAFE_DELETE( s_pDirectionLight );
	SAFE_DELETE( s_pMaterial );
}


void CCellMesh::Draw()
{
	ASSERT_D3DDEVICE( s_pD3DDevice );

	
	s_pRenderStates->startUsingStateBlock();
	
	THR( s_pD3DDevice->LightEnable( 0, TRUE ) );
	D3DLIGHT9 lightToSetOnDevice;
	lightToSetOnDevice = ( (D3DLIGHT9)*s_pDirectionLight );
	THR( s_pD3DDevice->SetLight( 0, &lightToSetOnDevice ) );
	THR( s_pD3DDevice->SetMaterial( s_pMaterial ) );

	s_pCellMesh->Draw();

	s_pRenderStates->endUsingStateBlock();

}