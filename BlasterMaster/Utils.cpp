#include <Windows.h>

#include "Utils.h"

void DebugOut(const wchar_t *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	wchar_t dbg_out[4096];
	vswprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugString(dbg_out);
}

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

/*
char * string to wchar_t* string.
*/
wstring ToWSTR(string st)
{
	const char *str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring wstr(wcstring);

	// delete wcstring   // << can I ? 
	return wstr;
}

/*
	Convert char* string to wchar_t* string.
*/
LPCWSTR ToLPCWSTR(string st)
{
	const char *str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring *w = new wstring(wcstring);

	// delete wcstring   // << can I ? 
	return w->c_str();
}

int displayMessage(std::string s)
{
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();

	int msgboxID = MessageBox(
		NULL,
		(LPCWSTR)sw,
		(LPCWSTR)L"Show infos",
		MB_ICONWARNING
	);

	switch (msgboxID)
	{
	case IDCANCEL:
		// TODO: add code
		break;

		return msgboxID;
	}
}

int displayMessage(int number)
{
	int i = number;
	wchar_t istr[32];
	_itow_s(i, istr, 10);

	int msgboxID = MessageBox(
		NULL,
		(LPCWSTR)istr,
		(LPCWSTR)L"Show info",
		MB_ICONWARNING
	);

	switch (msgboxID)
	{
	case IDCANCEL:
		// TODO: add code
		break;

		return msgboxID;
	}
}