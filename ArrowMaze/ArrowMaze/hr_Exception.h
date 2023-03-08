// In the name of God
// Exception handling for Direct3D HRESULTs returned by the COM methods
// and the Win32 APIs that may have the 'error' handling techniques.
// Ehsan Marufi Azar, 2012 (c)
// v1.1 Jan 2013

#pragma once
#ifndef HR_EXCEPTION_H
#define HR_EXCEPTION_H

#include <Windows.h>
#include <string>
#include "safe.h"
#include "Assertion.h"

#ifdef NO_THROW
#include <atlbase.h>
#endif


namespace Exception
{
	class CExceptionHR
	{
	public:
		CExceptionHR( const HRESULT& hr, LPCSTR strFile, const UINT& uiLine, LPCSTR strMessage = 0 );
		CExceptionHR( const CExceptionHR& copy );
		~CExceptionHR();


		void explain() const;

		inline const HRESULT& getHRresult() const	{ return m_hr; }
		inline LPCSTR getFile() const				{ return m_strFile.c_str(); }
		inline const UINT& getLine() const			{ return m_uiLine; }
		inline LPCSTR getMessage() const			{ return m_strMessage.c_str(); }


	protected:
		static const char* s_szFormat;

		HRESULT m_hr;
		UINT m_uiLine;
		std::string m_strFile, m_strMessage;

		enum { MAX_CHARS = 1024 };	// Instead of using const unsinged int, which we would had to init
									// in constructor, we've used enum which is intrinsitcally const!

	}; // end of class 'CExceptionHR'



	HRESULT throw_hr ( const HRESULT& hr, LPCSTR strFile, const UINT& uiLine, LPCSTR strMessage = 0 );

	template <typename T>
	inline T throw_win ( LPCSTR strFile, const UINT& uiLine, const T& status,
		LPCSTR strMessage = 0, const int& error = ::GetLastError() )
	{
		if ( !status )
		{
			throw_hr( HRESULT_FROM_WIN32(error), strFile, uiLine, strMessage );
		} 
		return status;
	}



// MACROs for simple manipulation of data.
// these MACROs will automatically fill in the __FILE__ and __LINE__ and automatically for _T().
#define THR(hr) Exception::throw_hr(hr, __FILE__, __LINE__, #hr)
#define THRM(hr, msg) Exception::throw_hr(hr, __FILE__,__LINE__, msg)

#define TWS(status) Exception::throw_win(__FILE__, __LINE__, status, #status)
#define TWSM(status, msg) Exception::throw_win(__FILE__, __LINE__, status, msg)

#define TWSE(status, error) Exception::throw_win(__FILE__, __LINE__, status, 0, error)
#define TWSME(status, msg, error) Exception::throw_win(__FILE__, __LINE__, status, msg, error)


} // end of namespace 'Exception'
#endif // HR_EXCEPTION_H