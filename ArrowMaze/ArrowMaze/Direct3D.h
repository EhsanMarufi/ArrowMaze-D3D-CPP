// In the name of God
// Ehsan Marufi Azar, 2013 (c)
// Class for maintaining the Direct3D application's data.
// This class is a encapsulator for DXUT's functions and even more for application specific needs.
// All rights reserved.
// v1.2 Jan 2013

#pragma once
#ifndef DIRECT3D_H
#define DIRECT3D_H

#include <Windows.h>
#include "hr_exception.h"
#include "Assertion.h"
#include "GameLogic.h"

#include "DXUT\Core\DXUT.h"
#include "DXUT\Optional\SDKmisc.h" // for DXUTTextHelper


namespace Direct3D
{

	class CDirect3D
	{
	public:
		CDirect3D( void );
		~CDirect3D( void );


		HRESULT DxUTInit(
			const bool& bParseCommandLine = true ,
			const bool& bShowMsgBoxOnError = true,
			WCHAR* strExtraCommandLineParams = 0,
			const bool& bThreadSafeDXUT = false );


		inline static bool IsDXUTInitted() { return CDirect3D::s_bDXUTInitted; }


		inline void onKeyDown( const WPARAM& wParam )
		{
			m_pGameLogic->onKeyDown( wParam );
		}


		// COMMON TASKS; ALL INLINE.
		//---------------------------

		// handle the default hotkeys
		inline void SetHotkeyHandling(
			const bool& bAltEnterToToggleFullscreen = true,
			const bool& bEscapeToQuit = true,
			const bool& bPauseToToggleTimePause = true )
		{
			::DXUTSetHotkeyHandling( bAltEnterToToggleFullscreen, bEscapeToQuit, bPauseToToggleTimePause );
		}


		// Show the cursor and clip it when in full screen
		inline void SetCursorSettings(
			const bool& bShowCursorWhenFullScreen = false,
			const bool& bClipCursorWhenFullScreen = false )
		{
			::DXUTSetCursorSettings( bShowCursorWhenFullScreen, bClipCursorWhenFullScreen );
		}


		// If a device is successfully found, the 'ModifyDeviceSettings' callback will be called
		// to allow the application to examine or change the device settings before the device is created.
		// Smart for both of Direct3D9 and 10.
		inline HRESULT CreateDevice(
			const bool& bWindowed = true,
			const INT& nSuggestedWidth = 0,
			const INT& nSuggestedHeight = 0 ) 
		{
			return THRM( ::DXUTCreateDevice( bWindowed, nSuggestedWidth, nSuggestedHeight),
						  "DXUTCreateDevice() - FAILED." );
		}


		// To alter the settings of a current device, the application can call 'DXUTGetDeviceSettings'
		// to get the device settings structure, change it, and then call 'DXUTCreateDeviceFromSettings'.
		// The application can use 'DXUTFindValidDeviceSettings' to ensure that the device
		// settings are valid before calling DXUTCreateDeviceFromSettings.
		inline HRESULT CreateDeviceFromSettings(
			DXUTDeviceSettings* pDeviceSettings,
			const bool& bPreserveInput = FALSE,
			const bool& bClipWindowToSingleAdapter = TRUE )
		{
			return THRM(::DXUTCreateDeviceFromSettings(
						pDeviceSettings,
						bPreserveInput,
						bClipWindowToSingleAdapter ),
					"CreateDeviceFromSetting() - FAILED." );
		}


		inline HRESULT SetWindow(
			const HWND& hWndFocus,
			const HWND& hWndDeviceFullScreen,
			const HWND& hWndDeviceWindowed,
			const bool& bHandleMessages = TRUE )
		{
			if ( hWndFocus == 0 || hWndDeviceFullScreen == 0 || hWndDeviceWindowed == 0 )
				THRM( E_INVALIDARG, "SetWindow() - INVALID ARGUMENTS." ); 

		return THRM( ::DXUTSetWindow(
						hWndFocus, hWndDeviceFullScreen,
						hWndDeviceWindowed, bHandleMessages ),
						"DXUTSetWindow() - FAILED." );
		}


		// This method checks whether the device is lost. If so, the method attempts to reset the device
		// and then calls the 'LPDXUTCALLBACKFRAMEMOVE' and 'LPDXUTCALLBACKD3D10FRAMERENDER' callback functions.
		inline void Render3DEnvironment()
		{
			::DXUTRender3DEnvironment();
		}


		// For each device setting, a match option in the 'DXUTDeviceSettings structure' specifies how the
		// function makes decisions. The function works for both Direct3D 9 and Direct3D 10 device settings.
		// This function is internally by DXUT used when toggling between full screen and windowed modes,
		// when selecting between HAL and REF device types, and inside 'DXUTCreateDevice'.
		inline HRESULT FindValidDeviceSettings(
			DXUTDeviceSettings* pOut,
			DXUTDeviceSettings* pIn = NULL,
			DXUTMatchOptions* pMatchOptions = NULL )
		{
			return THRM( ::DXUTFindValidDeviceSettings( pOut, pIn, pMatchOptions ),
							"DXUTFindValidDeviceSettings() - FAILED." );
		}


		// High-Resolution TIMER
		//-----------------------
		inline HRESULT SetTimer(
			LPDXUTCALLBACKTIMER pCallbackTimer,
			const FLOAT& fTimeoutInSecs = 1.0f,
			UINT* pnIDEvent = NULL,
			void* pCallbackUserContext = NULL )
		{
			return THRM(::DXUTSetTimer(
						pCallbackTimer, fTimeoutInSecs,
						pnIDEvent, pCallbackUserContext ),
					"DXUTSetTimer() - FAILED." );
		}


		inline HRESULT KillTimer( const UINT& nIDEvent )
		{
			return THRM( ::DXUTKillTimer( nIDEvent ), "DXUTKillTimer() - FAILED." );
		}


		inline void Pause( const bool& bPauseTime, const bool& bPauseRendering )
		{
			::DXUTPause( bPauseTime, bPauseRendering );
		}


		//inline void ResetFrameworkState();{::DXUTResetFrameworkState();}

		//inline HRESULT SetConstantFrameTime( const BOOL& bEnabled, const FLOAT& fTimePerFrame = 0.0333f)
		//{
		//	return THRTM(::DXUTSetConstantFrameTime( bEnabled, fTimePerFrame ), "DXUTSetConstantFrameTime() - FAILED." );
		//}

		inline void SetD3DVersionSupport( const bool& bAppCanUseD3D9, const bool& bAppCanUseD3D10 )
		{
			::DXUTSetD3DVersionSupport( bAppCanUseD3D9, bAppCanUseD3D10 );
		}


		inline void SetMultimonSettings( const bool& bAutoChangeAdapter )
		{
			::DXUTSetMultimonSettings( bAutoChangeAdapter );
		}


		inline void SetShortcutKeySettings( const bool& bAllowWhenFullscreen = false, const bool& bAllowWhenWindowed = true )
		{
			::DXUTSetShortcutKeySettings( bAllowWhenFullscreen , bAllowWhenWindowed );
		}


		inline void Shutdown( const int& nExitCode = 0x0 )
		{
			::DXUTShutdown( nExitCode );
		}


		inline HRESULT ToggleFullScreen()
		{
			return THRM( ::DXUTToggleFullScreen(), "DXUTToggleFullscreen() - FAILED." );
		}


		inline HRESULT ToggleREF()
		{
			return THRM( ::DXUTToggleREF(), "DXUTToggleREF() - FAILED." );
		}



		// State Retrieval Functions; ALL INLINE
		//---------------------------------------
		inline bool DoesAppSupportD3D9()						{ return ::DXUTDoesAppSupportD3D9(); }
		//inline bool GetAutomation()							{ return ::DXUTGetAutomation(); }
		inline CONST D3DSURFACE_DESC* DXUTGetD3D9BackBufferSurfaceDesc() { return ::DXUTGetD3D9BackBufferSurfaceDesc(); }
		inline IDirect3DDevice9 * GetD3D9Device()				{ return ::DXUTGetD3D9Device(); }
		inline CONST D3DCAPS9 * GetD3D9DeviceCaps()				{ return ::DXUTGetD3D9DeviceCaps(); }
		inline IDirect3D9 * GetD3DObject()						{ return ::DXUTGetD3D9Object(); }
		inline D3DPRESENT_PARAMETERS GetD3D9PresentParameters()	{ return ::DXUTGetD3D9PresentParameters();}
		inline DXUTDeviceSettings GetDeviceSettings()			{ return ::DXUTGetDeviceSettings(); }
		inline LPCWSTR GetDeviceStats()							{ return ::DXUTGetDeviceStats(); }
		inline FLOAT GetElapsedTime()							{ return ::DXUTGetElapsedTime(); }
		inline INT GetExitCode()								{ return ::DXUTGetExitCode(); }
		inline FLOAT GetFPS()									{ return ::DXUTGetFPS(); }
		
		//Get a pointer to a string containing the current number of frames per second (optionally),
		//resolution, back buffer format, and depth stencil format.
		inline LPCWSTR GetFrameStats( const bool& bIncludeFPS = true ) { return ::DXUTGetFrameStats( bIncludeFPS ); }
		inline const RECT * GetFullsceenClientRectAtModeChange(){ return &::DXUTGetFullsceenClientRectAtModeChange(); }
		inline HWND GetHWNDFocus()								{ return ::DXUTGetHWNDFocus(); }
		inline HWND GetHWNDDeviceFullScreen()					{ return ::DXUTGetHWNDDeviceFullScreen(); }
		inline HWND GetHWNDDeviceWindowed()						{ return ::DXUTGetHWNDDeviceWindowed(); }
		inline BOOL GetShowMsgBoxOnError()						{ return ::DXUTGetShowMsgBoxOnError(); }
		inline DOUBLE GetTime()									{ return ::DXUTGetTime(); }
		inline const RECT* GetWindowClientRect()				{ return &::DXUTGetWindowClientRect(); }
		inline const RECT* GetWindowClientRectAtModeChange()	{ return &::DXUTGetWindowClientRectAtModeChange(); }
		inline LPCWSTR DXUTGetWindowTitle()						{ return ::DXUTGetWindowTitle(); }
		inline BOOL IsActive()									{ return ::DXUTIsActive(); }
		inline BOOL IsAppRenderingWithD3D9()					{ return ::DXUTIsAppRenderingWithD3D9(); }
		
		//For example VK_F1, VK_LSHIFT, VK_RCONTROL, VK_RMENU, and 41 (representing the 'A' key).
		inline BOOL IsKeyDown( const BYTE& vKey )				{return ::DXUTIsKeyDown(vKey);}
		
		//Allowed values are VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_XBUTTON1, and VK_XBUTTON2.
		inline BOOL IsMouseButtonDown(BYTE vButton)				{ return ::DXUTIsMouseButtonDown( vButton ); }
		inline BOOL IsRenderingPaused()							{ return ::DXUTIsRenderingPaused(); }
		inline BOOL IsTimePaused()								{ return ::DXUTIsTimePaused(); }
		inline BOOL IsWindowed()								{ return ::DXUTIsWindowed(); }

		//Direct3D10 support.
		inline bool DoesAppSupportD3D10()						{ return ::DXUTDoesAppSupportD3D10(); }
		inline BOOL IsD3D10Available()							{ return ::DXUTIsD3D10Available(); }
		inline BOOL IsAppRenderingWithD3D10()					{ return ::DXUTIsAppRenderingWithD3D10(); }
		inline CONST DXGI_SURFACE_DESC* DXUTGetDXGIBackBufferSurfaceDesc() { return ::DXUTGetDXGIBackBufferSurfaceDesc(); }
		inline IDXGIFactory* GetDXGIFactory()					{ return ::DXUTGetDXGIFactory(); }
		inline IDXGISwapChain* GetDXGISwapChain()				{ return ::DXUTGetDXGISwapChain(); }
		inline ID3D10DepthStencilView* GetD3D10DepthStencilView() { return ::DXUTGetD3D10DepthStencilView(); }
		inline ID3D10Device* GetD3D10Device()					{ return ::DXUTGetD3D10Device(); }
		inline ID3D10RenderTargetView* GetD3D10RenderTargetView() { return ::DXUTGetD3D10RenderTargetView(); }


	private:
		static CDirect3D* s_D3DObj;
		static bool s_bDXUTInitted;

		ID3DXFont* m_pFont;
		CDXUTTextHelper* m_pTextHelper;
		void RenderGuideText();

		CGameLogic* m_pGameLogic;



		void SetCallBacks();

		// CallBack functions; ALL INLINE.
		//---------------------------------
		// Rejects any D3D9 devices that aren't acceptable to the app by returning false
		bool CALLBACK IsD3D9DeviceAcceptable(
			D3DCAPS9* pCaps,
			D3DFORMAT AdapterFormat,
			D3DFORMAT BackBufferFormat,
			bool bWindowed,
			void* pUserContext );


		bool CALLBACK ModifyDeviceSettings(
			DXUTDeviceSettings* pDeviceSettings,
			void* pUserContext );


		// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
		// and aren't tied to the back buffer size
		HRESULT CALLBACK OnD3D9CreateDevice(
			IDirect3DDevice9* pd3dDevice,
			const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
			void* pUserContext );


		// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
		// or that are tied to the back buffer size 
		HRESULT CALLBACK OnD3D9ResetDevice(
			IDirect3DDevice9* pd3dDevice,
			const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
			void* pUserContext );


		// Handle updates to the scene.  This is called regardless of which D3D API is used
		void CALLBACK OnFrameMove(
			double fTime,
			float fElapsedTime,
			void* pUserContext );


		void CALLBACK OnD3D9FrameRender(
			IDirect3DDevice9* pd3dDevice,
			double fTime,
			float fElapsedTime,
			void* pUserContext );


		LRESULT CALLBACK MsgProc(
			HWND hWnd,
			UINT uMsg,
			WPARAM wParam,
			LPARAM lParam, 
			bool* pbNoFurtherProcessing,
			void* pUserContext );


		// Release D3D9 resources created in the OnD3D9ResetDevice callback 
		void CALLBACK OnD3D9LostDevice( void* pUserContext );


		// Release D3D9 resources created in the OnD3D9CreateDevice callback 
		void CALLBACK OnD3D9DestroyDevice( void* pUserContext );


		// If the Direct3D device is removed such as when a laptop undocks from a docking station, DXUT 
		// notifies that application with the LPDXUTCALLBACKDEVICEREMOVED callback. This allows the application
		// to respond to this event. If the application returns false from within the callback, DXUT will not 
		// try to find a new device and will shutdown.
		bool CALLBACK OnDeviceRemoved( void* pUserContext );


		void CALLBACK OnKeyBoard(
			UINT nChar,
			bool bKeyDown,
			bool bAltDown,
			void* pUserContext );


		void CALLBACK OnMouse(
			bool bLeftButtonDown,
			bool bRightButtonDown,
			bool bMiddleButtonDown,
			bool bSideButton1Down,
			bool bSideButton2Down,
			INT nMouseWheelDelta,
			INT xPos,
			INT yPos,
			void* pUserContext );



		// Direct3D10 support.
		//---------------------
		HRESULT CALLBACK OnD3D10DeviceCreated(
			ID3D10Device* pd3dDevice,
			CONST DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
			void* pUserContext );


		bool CALLBACK IsD3D10DeviceAcceptable(
			UINT Adapter,
			UINT Output,
			D3D10_DRIVER_TYPE DeviceType,
			DXGI_FORMAT
			BackBufferFormat,
			bool bWindowed,
			void* pUserContext );


		void CALLBACK OnD3D10DeviceDestroyed( void* pUserContext );


		void CALLBACK OnD3D10FrameRender(
			ID3D10Device* pd3dDevice,
			DOUBLE fTime,
			FLOAT fElapsedTime,
			void* pUserContext );


		void CALLBACK OnD3D10SwapChainReleasing( void* pUserContext );


		HRESULT CALLBACK OnD3D10SwapChainResized(
			ID3D10Device* pd3dDevice,
			IDXGISwapChain *pSwapChain, 
			const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
			void* pUserContext );


		// Proxies for encapsulating callBack functions, to make them able to access the class's data members. ALL INLINE.
		//-----------------------------------------------------------------------------------------------------------------
#define ASSERT_THEN_RETURN(x) ASSERT_D3DDEVICE(s_D3DObj); return s_D3DObj->x
		inline static bool CALLBACK IsD3D9DeviceAcceptableProxy(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
		{ ASSERT_THEN_RETURN(IsD3D9DeviceAcceptable(pCaps,AdapterFormat ,BackBufferFormat, bWindowed, pUserContext));}
		inline static bool CALLBACK ModifyDeviceSettingsProxy(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
		{ ASSERT_THEN_RETURN(ModifyDeviceSettings(pDeviceSettings, pUserContext));}
		inline static HRESULT CALLBACK OnD3D9CreateDeviceProxy(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
		{ ASSERT_THEN_RETURN(OnD3D9CreateDevice(pd3dDevice, pBackBufferSurfaceDesc, pUserContext));}
		inline static HRESULT CALLBACK OnD3D9ResetDeviceProxy(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
		{ ASSERT_THEN_RETURN(OnD3D9ResetDevice(pd3dDevice, pBackBufferSurfaceDesc, pUserContext));}
		inline static void CALLBACK OnFrameMoveProxy(double fTime, float fElapsedTime, void* pUserContext)
		{ ASSERT_THEN_RETURN(OnFrameMove(fTime, fElapsedTime, pUserContext));}
		inline static void CALLBACK OnD3D9FrameRenderProxy(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext)
		{ ASSERT_THEN_RETURN(OnD3D9FrameRender(pd3dDevice, fTime, fElapsedTime, pUserContext));}
		inline static LRESULT CALLBACK MsgProcProxy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
		{ ASSERT_THEN_RETURN(MsgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing, pUserContext));}
		inline static void CALLBACK OnD3D9LostDeviceProxy(void* pUserContext)
		{ ASSERT_THEN_RETURN(OnD3D9LostDevice(pUserContext));}
		inline static void CALLBACK OnD3D9DestroyDeviceProxy(void* pUserContext)
		{ ASSERT_THEN_RETURN(OnD3D9DestroyDevice(pUserContext));}
		inline static void CALLBACK OnKeyBoardProxy(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
		{ ASSERT_THEN_RETURN(OnKeyBoard(nChar, bKeyDown, bAltDown, pUserContext));}
		inline static void CALLBACK OnMouseProxy(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
			bool bSideButton1Down, bool bSideButton2Down, INT nMouseWheelDelta, INT xPos, INT yPos, void* pUserContext)
		{ ASSERT_THEN_RETURN(OnMouse(bLeftButtonDown, bRightButtonDown, bMiddleButtonDown, bSideButton1Down, bSideButton2Down, nMouseWheelDelta, xPos, yPos, pUserContext));}
		inline static bool CALLBACK OnDeviceRemovedProxy(void* pUserContext)
		{ ASSERT_THEN_RETURN(OnDeviceRemoved(pUserContext));}

		// Direct3D10 support.
		inline static HRESULT CALLBACK OnD3D10DeviceCreatedProxy(ID3D10Device * pd3dDevice, CONST DXGI_SURFACE_DESC * pBackBufferSurfaceDesc, void* pUserContext)
		{ ASSERT_THEN_RETURN(OnD3D10DeviceCreated(pd3dDevice, pBackBufferSurfaceDesc, pUserContext));}
		inline static bool CALLBACK IsD3D10DeviceAcceptableProxy(UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
		{ ASSERT_THEN_RETURN(IsD3D10DeviceAcceptable(Adapter, Output, DeviceType, BackBufferFormat, bWindowed, pUserContext));}
		inline static void CALLBACK OnD3D10DeviceDestroyedProxy(void* pUserContext)
		{ ASSERT_THEN_RETURN(OnD3D10DeviceDestroyed(pUserContext));}
		inline static void CALLBACK OnD3D10FrameRenderProxy(ID3D10Device * pd3dDevice, DOUBLE fTime, FLOAT fElapsedTime, void* pUserContext)
		{ ASSERT_THEN_RETURN(OnD3D10FrameRender(pd3dDevice, fTime, fElapsedTime, pUserContext));}
		inline static void CALLBACK OnD3D10SwapChainReleasingProxy(void* pUserContext)
		{ ASSERT_THEN_RETURN(OnD3D10SwapChainReleasing(pUserContext));}
		inline static HRESULT CALLBACK OnD3D10SwapChainResizedProxy(ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
		{ ASSERT_THEN_RETURN(OnD3D10SwapChainResized(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc, pUserContext));}
		//#undef ASSERT_THEN_RETURN //TODO: UNCOMMENT THIS, COMMENTED JUST COZ OF THE INTELISENCE!

	}; // end of class 'CDirect3D'
} // end of namespace 'Direct3D'
#endif // DIRECT3D_H

