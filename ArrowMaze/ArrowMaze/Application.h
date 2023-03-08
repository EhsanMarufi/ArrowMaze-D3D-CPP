// In the name of God
// Ehsan Marufi Azar, 2012 (c)
// v1.1 Jan 2013

#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <Windows.h>

#include "Assertion.h"
#include "Direct3D.h"

namespace Application
{

	class CApplication
	{
	public:
		CApplication( const HINSTANCE& hInstance, const int& nCmdShow );
		~CApplication( void );

		int Run();

		static inline LRESULT CALLBACK WndProcProxy ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
		{
			ASSERT( s_pApplication, "The 's_pApplication' refers to an invalid address.");
			return s_pApplication->WndProc( hWnd, uMsg, wParam, lParam );
		}


		// member-datas made public because of performace considerations
		static HWND MainWindowHandle;
		static CApplication* s_pApplication;

		Direct3D::CDirect3D* m_pDirect3D;
		bool bMinimized, bRestored, bActive;

	private:
		bool initApplication( const HINSTANCE&, const int& );
		LRESULT CALLBACK WndProc ( HWND, UINT, WPARAM, LPARAM );

	}; // end of class 'CApplication'
} // end of namespace 'Application'
#endif // APPLICATION_H