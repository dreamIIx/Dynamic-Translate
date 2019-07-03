#pragma once

class _ClipBoard_
{
public:
	HANDLE hData;
	bool isopen;
#ifndef _UNICODE
	char* buf;
	char* buftemp;
#else
	wchar_t* buf;
	wchar_t* buftemp;
#endif

	_ClipBoard_()
	{
		isopen = false;
#ifndef _UNICODE
		buf = (char*)"";
		buftemp = (char*)"";
#else
		buf = (wchar_t*)"";
		buftemp = (wchar_t*)"";
#endif
	}

#ifndef _UNICODE
	void getClipboardData__(::std::string& buf_)
#else
	void getClipboardData__(::std::wstring& buf_)
#endif
	{
		isopen = OpenClipboard(NULL);

		if (isopen)
		{
			hData = GetClipboardData(CF_TEXT);
			if (hData)
			{
#ifndef _UNICODE
				buf = reinterpret_cast<char*>(GlobalLock(hData));
#else
				buf = reinterpret_cast<wchar_t*>(GlobalLock(hData));
#endif

#ifndef _UNICODE
				if ((::std::strcmp(buftemp, buf) != 0) && (buf != "") && (buf != NULL) && (buf != nullptr))
#else
				if ((buftemp != buf) && (buf != L"") && (buf != NULL) && (buf != nullptr)) // ???
#endif
				{
					buftemp = buf;
					buf_ = buf;
					system("cls");
#ifndef _UNICODE
					::std::cout << buf << ::std::endl;
#else
					::std::wcout << buf << ::std::endl;
#endif
				}
				GlobalUnlock(hData);
			}

			this->~_ClipBoard_();
			Sleep(1000);
		}
	}

	~_ClipBoard_(void)
	{
		if (isopen)
		{
			CloseClipboard();
			isopen = false;
		}
	}
};