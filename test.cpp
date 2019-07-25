﻿/*
	Autor - -dreamIIx
GitHub - https://github.com/dreamIIx
Release [v0.1] on GitHub 03.07.2019
Actual version 3.0
This project is an "Dynamic Translate" programm, than supports to translate *any* text *fast*.
                                                              !!!This project uses a Yandex.Translate API!!!
*/

#include <iostream>
#include <codecvt>
#include <string>

#include <Windows.h>
#include <windef.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <wchar.h>

#include "json.hpp"

#define CURL_STATICLIB

#include "curl/curl.h"

#ifdef _DEBUG
#pragma comment(lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment(lib, "curl/libcurl_a.lib")
#endif

#include "UNICODE util.h"
#include "clipboard.h"

using json = nlohmann::json;

#ifdef _DEBUG
#ifndef def_ERR_FL_
#define def_ERR_FL_
#define def_XTT_S(x) #x
#define def_XTT_S_(x) def_XTT_S(x)
#define def_XTT_S__LINE__ def_XTT_S_(__LINE__)
#define def__FILELINE (__FILE__  " line " def_XTT_S__LINE__)
#define ERROR_ ::std::cout << "Error - " << def__FILELINE << ::std::endl;
#endif
#else
#define ERROR_ ::std::cout << "error" << ::std::endl;
#endif

#define _TARGET_ADDRESS_ "https://translate.yandex.net/api/v1.5/tr.json/translate"
#define _SECURE_ACCESS_
#define __ALIGN47 112 // 96 (112)
#define __ALIGN48 48 // -16 (48)
#define WIN_X_ 750
#define WIN_Y_ 350

const TCHAR* szWindowClass = _T("Dynamic Translate v2.1");
const TCHAR* szTitle = _T("dynamic_translator(translation is done using Yandex.Translate API");
HWND button;
HINSTANCE hInst;
CURL* hcurl = nullptr;
_ClipBoard_ cb;
::std::string response = "your text to translate";
static int nxPos = 0, nxMin = 0, nxMax = 255;

#ifdef _UNICODE
std::string w2char(const std::wstring&);
std::wstring char2w(const std::string&);
#endif
bool send_req(::std::string&);
::std::string create_request(_STR_, ::std::string);
static int writer(char*, size_t, size_t, ::std::string*);
::std::string reparse(::std::string);
bool mA(CURL*, _ClipBoard_&);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);

	curl_global_init(CURL_GLOBAL_ALL);
	hcurl = curl_easy_init();

	if (hcurl)
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			MessageBox(NULL,
				_T("Call to RegisterClassEx failed!"),
				_T("dynamic_translator"),
				NULL);

			return 1;
		}

		hInst = hInstance;

		// The parameters to CreateWindow explained:
		// szWindowClass: the name of the application
		// szTitle: the text that appears in the title bar
		// WS_OVERLAPPEDWINDOW: the type of window to create
		// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
		// 500, 100: initial size (width, length)
		// NULL: the parent of this window
		// NULL: this application does not have a menu bar
		// hInstance: the first parameter from WinMain
		// NULL: not used in this application
		HWND hWnd = CreateWindow(
			szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			WIN_X_, WIN_Y_,
			NULL,
			NULL,
			hInstance,
			NULL
		);

		button = CreateWindow(
			_T("BUTTON"),
			_T("OK"),
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			WIN_X_, WIN_Y_,
			50, 50,
			hWnd,
			(HMENU)1,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL
		);

		if (!hWnd)
		{
			MessageBox(NULL,
				_T("Call to CreateWindow(hWnd) failed!"),
				_T("dynamic_translator"),
				NULL);

			return 1;
		}
		if (!button)
		{
			MessageBox(NULL,
				_T("Call to CreateWindow(button) failed!"),
				_T("dynamic_translator"),
				NULL);

			return 1;
		}

		ShowWindow(hWnd, nCmdShow);
		ShowWindow(button, nCmdShow);

		UpdateWindow(hWnd);
		UpdateWindow(button);
		//SendMessage(hWnd, DM_SETDEFID, IDOK, 0);

		//HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(122));
		MSG msg;
		BOOL bRet = 0;
		while (bRet = GetMessage(&msg, nullptr, 0, 0))
		{
			if (bRet == -1) break;
			//if (!TranslateAccelerator(hWnd, hAccel, &msg))
			//{
			if (!IsDialogMessage(hWnd, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			//}
		}

		curl_global_cleanup();
		return (int)msg.wParam;
	}
	else
	{
		ERROR_
			curl_global_cleanup();
		return 1;
	}
}

#ifdef _UNICODE
std::string w2char(const std::wstring &wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

std::wstring char2w(const std::string &str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
#endif

bool send_req(::std::string& req)
{
	CURLcode res;

	res = curl_easy_setopt(hcurl, CURLOPT_URL, _TARGET_ADDRESS_);
	if (res != CURLE_OK)
	{
		ERROR_
			return false;
	}
#ifdef _SECURE_ACCESS_
	res = curl_easy_setopt(hcurl, CURLOPT_SSL_VERIFYPEER, 0L);
	if (res != CURLE_OK)
	{
		ERROR_
			return false;
	}
	res = curl_easy_setopt(hcurl, CURLOPT_SSL_VERIFYHOST, 0L);
	if (res != CURLE_OK)
	{
		ERROR_
			return false;
	}
#endif
	res = curl_easy_setopt(hcurl, CURLOPT_POSTFIELDS, req.c_str());
	if (res != CURLE_OK)
	{
		ERROR_
			return false;
	}
	res = curl_easy_setopt(hcurl, CURLOPT_WRITEFUNCTION, writer);
	if (res != CURLE_OK)
	{
		ERROR_
			return false;
	}
	res = curl_easy_setopt(hcurl, CURLOPT_WRITEDATA, &response);
	if (res != CURLE_OK)
	{
		ERROR_
			return false;
	}

	res = curl_easy_perform(hcurl);
	if (res != CURLE_OK)
	{
		ERROR_
			return false;
	}

	return true;
}

::std::string create_request(_STR_ text, ::std::string target)
{
	::std::string res = "key=trnsl.1.1.20190701T074727Z.305a7c00849d49b1.ba6e2f35026a74f023b638f65813428b936644e9&text=";

	for(size_t x = 0; x <= text.size(); ++x)
	{
		if (text[x] == _TEXT_(' '))
		{
			text.erase(text.begin() + x);
			text.insert(text.begin() + x, _TEXT_('%'));
			text.insert(text.begin() + x + 1, _TEXT_('2'));
			text.insert(text.begin() + x + 2, _TEXT_('0'));
		}
	}

#ifdef _UNICODE
	auto temp = w2char(text);
	res += temp;
#else
	res += text;
#endif

	res += "&lang=";
	res += target;
	res += "&format=plain&options=1";

	return res;
}

static int writer(char* data, size_t size, size_t nmemb, ::std::string* writerData)
{
	if (writerData == NULL)
		return 0;

	writerData->append(data, size*nmemb);

	return size * nmemb;
}

::std::string reparse(::std::string temp)
{
	::std::string res = temp;
	res.erase(res.begin(), res.begin() + 2);
	res.erase(res.end() - 2, res.end());
	int i = 0;
	while (i < res.size() - 1)
	{
		if (static_cast<int>(res[i]) == 92 && static_cast<int>(res[i + 1]) == 110)
		{
			res.erase(res.begin() + i, res.begin() + i + 2);
			res.insert(res.begin() + i, '\r\n');
		}
		++i;
	}
#ifndef _UNICODE
	int i = 0, align = 0;
	while(true)
	{
		if (i * 2 + align > res.size() - 1) break;
		int tempchar = 0;
		if (static_cast<int>(res[i * 2 + align]) == -47) tempchar = static_cast<int>(res[i * 2 + 1 + align]) + __ALIGN47;
		else if (static_cast<int>(res[i * 2 + align]) == -48) tempchar = static_cast<int>(res[i * 2 + 1 + align]) + __ALIGN48;
		else if (static_cast<int>(res[i * 2 + align]) >= '0' && static_cast<int>(res[i * 2 + align]) <= '9')
		{
			tempchar = static_cast<int>(res[i * 2 + align]);
			--align;
		}
		else if (static_cast<int>(res[i * 2 + align]) == '\\' && static_cast<int>(res[i * 2 + 1 + align]) == 'n')
		{
			tempchar = static_cast<int>('\r\n');
		}
		else if (static_cast<int>(res[i * 2 + align]) >= 'A' && static_cast<int>(res[i * 2 + align]) <= 'z')
		{
			tempchar = static_cast<int>(res[i * 2 + align]);
			--align;
		}
		else if (ispunct(static_cast<int>(res[i * 2 + align])) || static_cast<int>(res[i * 2 + align]) == ' ')
		{
			tempchar = static_cast<int>(res[i * 2 + align]);
			--align;
		}
		else ERROR_

		res[i] = static_cast<char>(tempchar);
		++i;
	}
	res.erase(res.begin() + i, res.end());
#endif
	return res;
}

bool mA()
{
	//_setmode(_fileno(stdout), _O_U16TEXT);
	//_setmode(_fileno(stdin), _O_U16TEXT);
	//_setmode(_fileno(stderr), _O_U16TEXT);

	_STR_ clipString = _TEXT_("");
	::std::string req = "";
	response = "";

	cb.getClipboardData__(clipString);
	if (clipString == _TEXT_(""))
	{
		ERROR_
			return false;
	}

	req = create_request(clipString, "ru");

	if (!send_req(req))
	{
		ERROR_
			return false;
	}

	json sparsed = json::parse(response);
	response = reparse(sparsed["text"].dump());

	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HFONT mainFont;
	SCROLLINFO si = { 0 };
	TEXTMETRIC tm;

	static int winx_ = WIN_X_;
	static int winy_ = WIN_Y_;

	static int VScrollPos;
	static int yChar;

	switch (message)
	{
	case WM_CREATE:
	{
		hdc = GetDC(hWnd);

		GetTextMetrics(hdc, &tm);
		yChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hWnd, hdc);
		return 0;
	}
	case WM_VSCROLL:
	{
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &si);

		VScrollPos = si.nPos;
		switch (LOWORD(wParam))
		{
		case SB_TOP:
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;
		case SB_LINEUP:
			si.nPos -= 1;
			break;
		case SB_LINEDOWN:
			si.nPos += 1;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		si.fMask = SIF_POS;
		si.nPage = winy_ / yChar;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hWnd, SB_VERT, &si);

		if (si.nPos != VScrollPos)
		{
			ScrollWindowEx(hWnd, 0, yChar * (VScrollPos - si.nPos), NULL, NULL, NULL, NULL, NULL);
			//UpdateWindow(hWnd);
			//InvalidateRect(hWnd, NULL, TRUE);
		}

		return 0;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

#ifdef _UNICODE
		mainFont = CreateFont(16, 0, FW_DONTCARE, FW_DONTCARE, FW_DONTCARE,
			FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _TEXT_("Arial"));

		auto oldFont = SelectObject(hdc, mainFont);
#endif

		ps.rcPaint.left = 0;
		ps.rcPaint.top = 0;
		ps.rcPaint.right = winx_ - 85;
		ps.rcPaint.bottom = winy_;

#ifdef _UNICODE
		auto resString = char2w(response);
		DrawText(hdc, resString.c_str(), _tcslen(resString.c_str()), &ps.rcPaint, DT_WORDBREAK);

		SelectObject(hdc, oldFont);
		DeleteObject(mainFont);
#else
		DrawText(hdc, response.c_str(), _tcslen(response.c_str()), &ps.rcPaint, DT_WORDBREAK);
#endif

		EndPaint(hWnd, &ps);
		//ScrollWindowEx(hWnd, 0, -yChar * (VScrollPos - si.nPos), NULL, NULL, NULL, NULL, NULL);

		return 0;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case '0':
			if (mA())
			{
				UpdateWindow(hWnd);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		default:
			break;
		}
		return 0;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam))
		{
			if (mA())
			{
				UpdateWindow(hWnd);
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
		return 0;
	}
	case WM_SIZE:
	{
		winx_ = LOWORD(lParam);
		winy_ = HIWORD(lParam);
		SetWindowPos(button, HWND_TOP, winx_ - 50, winy_ - 50, 50, 50, NULL);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 255;
		si.nPage = winy_ / yChar;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

		return 0;
	}
	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
		return 0;
	}
	}
}