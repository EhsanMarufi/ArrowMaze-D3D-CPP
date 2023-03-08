// In the name of God
// Ehsan Marufi Azar, 2012 (c)

/*
 * Note: To make the Assertion work, Goto 
 * Project > [YourProgram] Properties > Configuration Properties > C/C++ > Preprocessor
 * and in the right pane add the 'ASSERTION_ENABLED' to the 'Preprocessor Definitions'.
 */

#pragma once
#include <Windows.h>
#include <string>


#define MAX_ASSERT_EXPLANATION_CHARS (_CVTBUFSIZE)
#define HALT() __debugbreak()

namespace Assertion
{
	class CAssertion
	{
	public:
		CAssertion() {}
		~CAssertion() {}

		static void explain(LPCSTR strCriterion, LPCSTR strFile, const DWORD& dwLine, LPCSTR strMsg);
		
	protected:
		static LPCSTR s_strMessageFormat;

	}; // end of class 'CAssertion'


#ifdef ASSERTION_ENABLED  
    #define ASSERT(expression, msg) \
	do { \
		if (!(expression)) \
		{ \
			Assertion::CAssertion::explain(#expression, __FILE__, __LINE__, msg); \
			HALT(); \
		} \
	} while(0)  
#else  
    #define ASSERT(expression, msg) \
        do \
		{ \
			(void)sizeof(expression); \
		} while(0)  
#endif  

#define ASSERT_MEMORYALLOCATION(expression) ASSERT(expression, "Memory Allocation FAILED!")
#define ASSERT_POINTER(expression) ASSERT(expression, "Invalid Pointer.")
#define ASSERT_POINTER_M(expression, msg) ASSERT(expression, msg)
#define ASSERT_D3DDEVICE(expression) ASSERT(expression, "Invalid Device.")

};//end of namespace Assertion