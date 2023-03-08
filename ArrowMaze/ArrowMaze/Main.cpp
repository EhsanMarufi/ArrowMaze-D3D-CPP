// In the name of God, the compassionate, the merciful
// Win32 windows creator
// Ehsan Marufi Azar, (c) 2013
// v1.1 Jan 2013


#include <Windows.h>
#include <string>
#include <tchar.h>			// for _tWinMain
#include <exception>

#include "Application.h"
#include "hr_Exception.h"
#include "Assertion.h"


INT APIENTRY _tWinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR szCommandline, int nCmdShow)
{
	int ret = 0x0;
	try
	{
		Application::CApplication App( hInstance, nCmdShow );
		return App.Run();
	}
	catch ( const std::exception& e )
	{
		::MessageBoxW( 0, reinterpret_cast<LPCWSTR>( e.what() ), L"std ERROR", MB_OK );
		ret = 0x1;
	}
	catch( const Exception::CExceptionHR& e )
	{
		e.explain();
		ret = 0x2;
	}
	catch (...)
	{
		//TODO: REPLACE THE MESSAGE WITH SOME LOGGING MECHANISM. (YOU MAY CHANGE THE EXCEPTION HANDLER
		//CLASS CONSTRUCTOR, TO RECORD THE EXCEPTIONS AROSE EVERY SINGLE TIME THE CONSTRUCTOR CALLED!)
		::MessageBox(0,
			L"Some errors have been occured and logged in the output panel.\n"
			L"We're sorry for inconveniences that may arise.\n"
			L"\n                     The developer, Ehsasn Marufi Azar, 2013.",
			 L"ERROR", MB_OK);
		ret = 0x2;
	}
	//CAUTION: NO CODE BUT RETURN HAS TO GO HERE!
	return ret;
}