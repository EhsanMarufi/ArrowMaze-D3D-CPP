// In the name of God
// Class for maintaining the Direct3D application's data.
// This class is a encapsulator for DXUT's functions and even more for application specific needs.
// Ehsan Marufi Azar, 2012 (c)
// All rights reserved.

#include "Direct3D.h"

using namespace Direct3D;

bool CDirect3D::s_bDXUTInitted = false;
CDirect3D* CDirect3D::s_D3DObj = 0;


CDirect3D::CDirect3D( void )
{
	CDirect3D::s_D3DObj = this;
	m_pTextHelper = 0;
	m_pFont = 0;
	ASSERT_MEMORYALLOCATION( m_pGameLogic = new CGameLogic() );
}


CDirect3D::~CDirect3D( void )
{
	//CDirect3D::s_D3DObj = 0; //TODO: LOCATE SOME APPROPRIATE PLACE TO DO THIS.
								//We don't have to do it here! Coz of DeviceLost or DeviceDestroy 
								//callbacks being called while exiting.
	
	SAFE_DELETE( m_pGameLogic );
	
	::DXUTCleanup3DEnvironment( true );	//Modified to be global; Coz whitout this the destructor frees all the 
										//resources ungently and then unexpected errors would have occoured,
										//and we couldn't have called the 'DXUTShutdown()'; 
										//Coz it terminates the app. (which we dont want to!)
}


HRESULT CDirect3D::DxUTInit( const bool& bParseCommandLine, const bool& bShowMsgBoxOnError,
							WCHAR* strExtraCommandLineParams, const bool& bThreadSafeDXUT )
{
	SetCallBacks();
	HRESULT hr = THRM( ::DXUTInit( bParseCommandLine, bShowMsgBoxOnError, strExtraCommandLineParams, bThreadSafeDXUT ),
						"DXUTInit() - FAILED." );
	CDirect3D::s_bDXUTInitted = SUCCEEDED( hr );
	return hr;
}


void CDirect3D::RenderGuideText()
{
	if ( !m_pTextHelper )
		return;

	m_pTextHelper->Begin();
	m_pTextHelper->SetForegroundColor( 0xffffff00 );
	m_pTextHelper->SetInsertionPos( 5, 5 );
	m_pTextHelper->DrawTextLine( L"Arrow Maze Game - By Ehsan Marufi Azar" );
	m_pTextHelper->DrawTextLine( GetDeviceStats() );
	m_pTextHelper->DrawTextLine( GetFrameStats() );
	m_pTextHelper->DrawFormattedTextLine( L"________________________________________________" );

	//m_pTextHelper->DrawFormattedTextLine(L"Score: %d", pGameLogic->getScoreSystem().getScore());
	//m_pTextHelper->DrawFormattedTextLine(L"You've Entered: %d", pGameLogic->getEnteredNumber());
	//m_pTextHelper->DrawFormattedTextLine(_T("Status: %s"), pGameLogic->getStatus().c_str());
	m_pTextHelper->End();
}


void CDirect3D::SetCallBacks()
{
	// Set the callback functions
	::DXUTSetCallbackD3D9DeviceAcceptable( CDirect3D::IsD3D9DeviceAcceptableProxy );
	::DXUTSetCallbackD3D9DeviceCreated( CDirect3D::OnD3D9CreateDeviceProxy );
	::DXUTSetCallbackD3D9DeviceReset( CDirect3D::OnD3D9ResetDeviceProxy );
	::DXUTSetCallbackD3D9FrameRender( CDirect3D::OnD3D9FrameRenderProxy );
	::DXUTSetCallbackD3D9DeviceLost( CDirect3D::OnD3D9LostDeviceProxy );
	::DXUTSetCallbackD3D9DeviceDestroyed( CDirect3D::OnD3D9DestroyDeviceProxy );
	::DXUTSetCallbackDeviceChanging( CDirect3D::ModifyDeviceSettingsProxy );
	::DXUTSetCallbackMsgProc( CDirect3D::MsgProcProxy );
	::DXUTSetCallbackFrameMove( CDirect3D::OnFrameMoveProxy );
	::DXUTSetCallbackDeviceRemoved( CDirect3D::OnDeviceRemovedProxy );
	
	::DXUTSetCallbackKeyboard( CDirect3D::OnKeyBoardProxy );
	::DXUTSetCallbackMouse( CDirect3D::OnMouseProxy );

	//Direct3D 10 support:
	::DXUTSetCallbackD3D10DeviceAcceptable( CDirect3D::IsD3D10DeviceAcceptableProxy );
	::DXUTSetCallbackD3D10DeviceCreated( CDirect3D::OnD3D10DeviceCreatedProxy );
	::DXUTSetCallbackD3D10DeviceDestroyed( CDirect3D::OnD3D10DeviceDestroyedProxy );
	::DXUTSetCallbackD3D10FrameRender( CDirect3D::OnD3D10FrameRenderProxy );
	::DXUTSetCallbackD3D10SwapChainReleasing( CDirect3D::OnD3D10SwapChainReleasingProxy );
	::DXUTSetCallbackD3D10SwapChainResized( CDirect3D::OnD3D10SwapChainResizedProxy );
}


// Rejects any D3D9 devices that aren't acceptable to the app by returning false
bool CALLBACK CDirect3D::IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	//::MessageBoxW(0, L"IsD3D9DeviceAcceptable",L"Notify", MB_OK);
    // Typically want to skip back buffer formats that don't support alpha blending
    //IDirect3D9* pD3D = DXUTGetD3D9Object(); 
    //if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
    //                AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
    //                D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
    //    return false;

    return true;
}


// Before a device is created, modify the device settings as needed
bool CALLBACK CDirect3D::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	pDeviceSettings->d3d9.pp.AutoDepthStencilFormat = D3DFMT_D24S8;
    return true;
}


// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
HRESULT CALLBACK CDirect3D::OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	m_pGameLogic->onD3DDeviceCreate( pd3dDevice );

	// Create the font to display data on screen
	THRM( D3DXCreateFont( GetD3D9Device(), 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                              OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                              L"Arial", &m_pFont ), "D3DXCreateFont() - FAILED." );

    return S_OK;
}


// Release D3D9 resources created in the 'OnD3D9CreateDevice' callback 
void CALLBACK CDirect3D::OnD3D9DestroyDevice( void* pUserContext )
{
	SAFE_RELEASE( m_pFont );
}


// Release D3D9 resources created in the 'OnD3D9ResetDevice' callback 
void CALLBACK CDirect3D::OnD3D9LostDevice( void* pUserContext )
{
	if ( m_pFont )
		THRM( m_pFont->OnLostDevice(), "ID3DXFont::OnLostDevice() - FAILED." );

	SAFE_DELETE( m_pTextHelper );
}


// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
HRESULT CALLBACK CDirect3D::OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	if ( m_pFont )
		THRM( m_pFont->OnResetDevice(), "ID3DXFont::OnResetDevice() - FAILED." );

	m_pTextHelper = new CDXUTTextHelper( m_pFont );
    return S_OK;
}


// Handle updates to the scene.  This is called regardless of which D3D API is used
void CALLBACK CDirect3D::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{

}


// Render the scene using the D3D9 device
void CALLBACK CDirect3D::OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;

	// Clear the render target and the zbuffer 
	D3DCOLOR BLACK = D3DCOLOR_ARGB( 0, 0, 0, 0 );
	D3DCOLOR WHITE = D3DCOLOR_ARGB( 0, 255, 255, 255 );
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, BLACK, 1.0f, 0) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		RenderGuideText();
		m_pGameLogic->Draw();

        V( pd3dDevice->EndScene() );
    }
}


// Handle messages to the application 
LRESULT CALLBACK CDirect3D::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
    return 0;
}


// If the Direct3D device is removed such as when a laptop undocks from a docking station, DXUT 
// notifies that application with the OnDeviceRemoved callback. This allows the application
// to respond to this event. If the application returns false from within the callback, DXUT will not 
// try to find a new device and will shutdown.
bool CALLBACK CDirect3D::OnDeviceRemoved(void* pUserContext)
{
	return true;
}


// These callback mechanisms are provided to simplify handling keyboard and mouse messages through the windows 
// message pump, but does not preclude the application from handling those messages directly through 
// the MsgProc callback.
void CALLBACK CDirect3D::OnKeyBoard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
	::MessageBoxW( 0, L"OnKeyBoard", L"notify", MB_OK );
}

void CALLBACK CDirect3D::OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
	bool bSideButton1Down, bool bSideButton2Down, INT nMouseWheelDelta, INT xPos, INT yPos, void* pUserContext )
{

}



//Direct3D10 support.

// This callback function is the appropriate location for the application to create resources in 
// D3DPOOL_MANAGED or D3DPOOL_SYSTEMMEM memory, since these resources do not need to be reloaded 
// whenever the device is reset. Resources created in this callback function should be deleted in 
// the onD3D10DeviceDestroyed callback function.
HRESULT CALLBACK CDirect3D::OnD3D10DeviceCreated( ID3D10Device * pd3dDevice,
	CONST DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) 
{
	return S_OK;
}


// After the application rejects the unwanted device settings combinations, DXUT picks the best of 
// the remaining combinations and uses that best combination to create the device. Before creating the 
// device, DXUT calls LPDXUTCALLBACKMODIFYDEVICESETTINGS to allow the application to change any of the 
// device creation settings.
bool CALLBACK CDirect3D::IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType,
	DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{ 
	return true;
}


// The onD3D10DeviceDestroyed callback function is the appropriate location for the 
// application release Direct3D 10 device resources that were created in the 
// onD3D10DeviceCreated callback function.
void CALLBACK CDirect3D::OnD3D10DeviceDestroyed( void* pUserContext )
{

}


// The onD3D10FrameRender callback function is the appropriate location for the 
// application to render the current scene. The OnFrameMove callback function will be 
// called once per frame, while onD3D10FrameRender will be called when the scene needs 
// to be rendered, which might be more than once per frame.
// DXUT will call this function after the OnFrameMove callback function.
void CALLBACK CDirect3D::OnD3D10FrameRender( ID3D10Device * pd3dDevice, DOUBLE fTime, FLOAT fElapsedTime, void* pUserContext )
{

}


// The onD3D10SwapChainReleasing callback function is the appropriate location for the 
// application release Direct3D 10 device resources that were created in the 
// onD3D10SwapChainReleasing callback function.
void CALLBACK CDirect3D::OnD3D10SwapChainReleasing( void* pUserContext )
{

}


// The onD3D10SwapChainResized callback function is the appropriate location for the 
// application to create Direct3D 10 device resources that depend on the back buffer such as a render 
// target that needs to match the size of the back buffer. Resources created in the 
// onD3D10SwapChainResized callback function should be released in the 
// onD3D10SwapChainReleasing callback function.
HRESULT CALLBACK CDirect3D::OnD3D10SwapChainResized(ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, 
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	return S_OK;
}