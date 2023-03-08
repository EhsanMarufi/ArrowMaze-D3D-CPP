#include "hr_Exception.h"

using namespace Exception;

LPCSTR CExceptionHR::s_szFormat =
"\n\
**EXEPTION OCCOURED**\n\
in: \"%s\",\n\
in line: %d\n\
with HRESULT code of (0x%x)\n\
with message: %s\n\
*********************\n";


CExceptionHR::CExceptionHR( const HRESULT& hr, LPCSTR strFile, const UINT& uiLine, LPCSTR strMessage )
{
	m_hr = hr;
	m_uiLine = uiLine;
	m_strFile.assign( strFile );
	m_strMessage.assign( strMessage );
}


CExceptionHR::CExceptionHR( const CExceptionHR& copy )
{
	m_hr = copy.getHRresult();
	m_uiLine = copy.getLine();
	m_strFile.assign( copy.getFile() );
	m_strMessage.assign( copy.getMessage() );
}


CExceptionHR::~CExceptionHR()
{
}


void CExceptionHR::explain() const
{
	LPSTR strContext = 0;
	ASSERT_MEMORYALLOCATION( strContext = new char[ MAX_CHARS - 1 ] );
	
	::sprintf_s( strContext, MAX_CHARS,	s_szFormat,
		m_strFile.c_str(), m_uiLine, m_hr, m_strMessage.c_str() );
	::OutputDebugStringA( strContext );
	
	SAFE_DELETE_ARRAY( strContext );
}


//
// function definitions

HRESULT Exception::throw_hr ( const HRESULT& hr, LPCSTR strFile, const UINT& uiLine, LPCSTR strMessage ) 
{
	if ( FAILED( hr ) )
	{
#ifdef NO_THROW
		LPSTR strContext = 0;
		ASSERT_MEMORYALLOCATION( strContext = new char[ 1023 ] );
		
		::sprintf_s( strContext, 1024, CExceptionHR::s_szFormat, strFile, uiLine, hr, strMessage );
		::OutputDebugStringA( out );
		
		SAFE_DELETE_ARRAY( strContext );
		::ATLASSERT( FALSE );
#else
		throw CExceptionHR( hr, strFile, uiLine, strMessage );
#endif
	}
	else // hr Succeeded
		return hr;
}