#include "Application.h"
using namespace Application;

HWND CApplication::MainWindowHandle = 0;
CApplication* CApplication::s_pApplication = 0;


CApplication::CApplication( const HINSTANCE& hInstance, const int& nCmdShow )
{
	CApplication::s_pApplication = this;
	bMinimized = false;
	bRestored = bActive = true;

	ASSERT_MEMORYALLOCATION( m_pDirect3D = new Direct3D::CDirect3D() );

	try
	{
		m_pDirect3D->DxUTInit( false, true );
		initApplication( hInstance, nCmdShow );
		m_pDirect3D->SetWindow( CApplication::MainWindowHandle, CApplication::MainWindowHandle, CApplication::MainWindowHandle, false );
		//TODO: CONSIDER CHANGING THE SECOND VALUE TO 'true', the application has to be able to create d3d10 either!
		m_pDirect3D->SetD3DVersionSupport( true, false );
		m_pDirect3D->CreateDevice();
	}
	catch ( Exception::CExceptionHR& exception )
	{
		exception.explain();
		throw;
	}
}


bool CApplication::initApplication( const HINSTANCE& hInstance, const int& nCmdShow )
{
	LPCTSTR szClassName = L"ArrowMazeGame";
	WNDCLASSW wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = static_cast<HBRUSH>( ::GetStockObject(BLACK_BRUSH) );
	wc.hCursor = ::LoadCursor( 0, IDC_ARROW );
	wc.hIcon = ::LoadIcon( 0, IDI_APPLICATION );
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProcProxy;
	wc.lpszClassName = szClassName;
	wc.lpszMenuName = 0;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	TWSM( ::RegisterClass(&wc), "** CLASS REGISTERATION FAILED! **" );

	CApplication::MainWindowHandle = ::CreateWindow(
		szClassName,
		L"Arrow Maze - By Ehsan Marufi Azar",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0);

	TWSM( CApplication::MainWindowHandle, "** WINDOW CREATION FAILED! **" );

	::ShowWindow( CApplication::MainWindowHandle, nCmdShow );
	::UpdateWindow( CApplication::MainWindowHandle );

	return true;
}


LRESULT CALLBACK CApplication::WndProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_DESTROY:
		::PostQuitMessage( 0 );
		return 0;

	case WM_KEYDOWN:
		m_pDirect3D->onKeyDown(wParam);
		return 0;
		if ( wParam == VK_ESCAPE )
			::DestroyWindow( CApplication::MainWindowHandle );
		return 0;
	}

	return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
}


int CApplication::Run()
{
	MSG  msg;
	msg.message = WM_NULL;

	::PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	while( WM_QUIT != msg.message  )
	{
		// Translate and dispatch the message. Since there is no menues, so just ignore the 'TranslateAccelerator()'.
		// Use PeekMessage() so we can use idle time to render the scene
		if( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 )
		{
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}
		else
		{
			// Render a frame during idle time (no messages are waiting)
			m_pDirect3D->Render3DEnvironment();
		}
	}

	return m_pDirect3D->GetExitCode();
}


CApplication::~CApplication(void)
{
	SAFE_DELETE( m_pDirect3D );
	CApplication::s_pApplication = 0;
}
