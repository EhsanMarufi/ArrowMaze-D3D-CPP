#include "Widget.h"

using namespace Direct3D;


CWidget::CWidget()
{
	m_Position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Rotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Scale    = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

	m_vecTransformationCenter = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	
	m_pmatRotation = 0;

	m_pmatCapturedWorldTransformation = 0;
	m_pmatCapturedViewTransformation = 0;
	m_pmatCapturedProjectionTransformation = 0;

	m_pRenderStatesManager = 0;
	ASSERT_MEMORYALLOCATION( m_pRenderStatesManager = new CRenderStateManagement() );
}


CWidget::CWidget( const CWidget& copy )
{
	m_Position	= copy.getPosition();
	m_Rotation	= copy.getRotation();
	m_Scale		= copy.getScale();

	m_vecTransformationCenter = copy.getTransformationCenter();

	m_pmatRotation = 0;

	m_pmatCapturedWorldTransformation = 0;
	m_pmatCapturedViewTransformation = 0;
	m_pmatCapturedProjectionTransformation = 0;

	LPD3DXMATRIX pTempMatrix = 0;
	if ( pTempMatrix = copy.getRotationMatrix() )
		ASSERT_MEMORYALLOCATION( m_pmatRotation = new D3DXMATRIX(*pTempMatrix) );
	
	if ( pTempMatrix = copy.getCaptruedWordTransformation() )
		ASSERT_MEMORYALLOCATION(m_pmatCapturedWorldTransformation = new D3DXMATRIX(*pTempMatrix));
	
	if ( pTempMatrix = copy.getCaptruedViewTransformation() )
		ASSERT_MEMORYALLOCATION( m_pmatCapturedViewTransformation = new D3DXMATRIX(*pTempMatrix) );
	
	if ( pTempMatrix = copy.getCaptruedProjectionTransformation() )
		ASSERT_MEMORYALLOCATION( m_pmatCapturedProjectionTransformation = new D3DXMATRIX(*pTempMatrix) );

	m_pRenderStatesManager = 0;
	ASSERT_MEMORYALLOCATION( m_pRenderStatesManager = new CRenderStateManagement( *copy.getStateBlockManager() ) );
}


CWidget::~CWidget( void )
{
	SAFE_DELETE( m_pmatCapturedWorldTransformation );
	SAFE_DELETE( m_pmatCapturedViewTransformation );
	SAFE_DELETE( m_pmatCapturedProjectionTransformation );
	SAFE_DELETE( m_pmatRotation );
	SAFE_DELETE( m_pRenderStatesManager );
}


void CWidget::captureDeviceTransformations( const DWORD& dwCaptureTransform )
{
	ASSERT_D3DDEVICE( s_pD3DDevice );

	if ( dwCaptureTransform & CT_WORLD )
	{
		ASSERT_MEMORYALLOCATION( m_pmatCapturedWorldTransformation = new D3DXMATRIX() );
		THR( s_pD3DDevice->GetTransform(D3DTS_WORLD, m_pmatCapturedWorldTransformation) );
	}
	
	if ( dwCaptureTransform & CT_VIEW )
	{
		ASSERT_MEMORYALLOCATION( m_pmatCapturedViewTransformation = new D3DXMATRIX() );
		THR( s_pD3DDevice->GetTransform(D3DTS_VIEW, m_pmatCapturedViewTransformation) );
	}
	
	if ( dwCaptureTransform & CT_PROJECTION )
	{
		ASSERT_MEMORYALLOCATION( m_pmatCapturedProjectionTransformation = new D3DXMATRIX() );
		THR( s_pD3DDevice->GetTransform(D3DTS_PROJECTION, m_pmatCapturedProjectionTransformation) );
	}
}


void CWidget::retrieveDeviceTransformations( const DWORD& dwRetrieveTransform )
{
	ASSERT_D3DDEVICE( s_pD3DDevice );

	if ( dwRetrieveTransform & CT_WORLD && !m_pmatCapturedWorldTransformation )
		THR( s_pD3DDevice->SetTransform(D3DTS_WORLD, m_pmatCapturedWorldTransformation) );
	
	if ( dwRetrieveTransform & CT_VIEW && !m_pmatCapturedViewTransformation )
		THR( s_pD3DDevice->SetTransform(D3DTS_VIEW, m_pmatCapturedViewTransformation) );
	
	if ( dwRetrieveTransform & CT_PROJECTION && !m_pmatCapturedWorldTransformation )
		THR( s_pD3DDevice->SetTransform(D3DTS_PROJECTION, m_pmatCapturedProjectionTransformation) );
}


void CWidget::applyTransformations( const bool& bCaptureDeviceTransformations )
{
	// Save current device properites
	if ( bCaptureDeviceTransformations )
		captureDeviceTransformations();

	// Apply this widget's transformation properties to the device
	D3DXMATRIX widgetRelativeTransformation;
	D3DXMATRIX widgetTranslationMatrix, widgetRotationMatrix, widgetScaleMatrix;
	D3DXMATRIX widgetRotationXMatrix, widgetRotationYMatrix, widgetRotationZMatrix;
	D3DXMATRIX widgetCombinedWorldMatrix;

	::D3DXMatrixTranslation( &widgetRelativeTransformation, m_vecTransformationCenter.x, m_vecTransformationCenter.y, m_vecTransformationCenter.z );
	
	::D3DXMatrixTranslation( &widgetTranslationMatrix, m_Position.x, m_Position.y, m_Position.z );
	
	if ( m_pmatRotation )
		widgetRotationMatrix = *m_pmatRotation;
	else
	{
		::D3DXMatrixRotationX( &widgetRotationXMatrix, m_Rotation.x );
		::D3DXMatrixRotationY( &widgetRotationYMatrix, m_Rotation.y );
		::D3DXMatrixRotationZ( &widgetRotationZMatrix, m_Rotation.z );
		widgetRotationMatrix = widgetRotationXMatrix * widgetRotationYMatrix * widgetRotationZMatrix;
	}

	::D3DXMatrixScaling( &widgetScaleMatrix, m_Scale.x, m_Scale.y, m_Scale.z );

	widgetCombinedWorldMatrix = widgetRelativeTransformation * widgetScaleMatrix * widgetRotationMatrix * widgetTranslationMatrix;
	THR( s_pD3DDevice->SetTransform(D3DTS_WORLD, &widgetCombinedWorldMatrix) );
}

