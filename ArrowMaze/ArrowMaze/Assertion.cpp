#include "Assertion.h"

LPCSTR Assertion::CAssertion::s_strMessageFormat =
	"\n*-=[ Assertion FAILED. ]=-\n"
	"\n* Criterion: %s"
	"\n* In: %s"
	"\n* Line: %d"
	"\n* With message: %s"
	"\n*-=[ End of Assertion Message. ]=-"
	"\n";

void Assertion::CAssertion::explain(LPCSTR strCriterion, LPCSTR strFile, const DWORD& dwLine, LPCSTR strMsg)
{
	LPSTR strContexMessage = 0;
	strContexMessage = new char[MAX_ASSERT_EXPLANATION_CHARS];
	if (!strContexMessage)
		HALT();

	::sprintf_s(strContexMessage, MAX_ASSERT_EXPLANATION_CHARS, s_strMessageFormat, strCriterion, strFile, dwLine, strMsg);
	::OutputDebugStringA(strContexMessage);
	delete [] strContexMessage;
}