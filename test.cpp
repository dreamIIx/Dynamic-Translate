#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <Windows.h>

#include "json.hpp"

#include "clipboard.h"

#define CURL_STATICLIB

#include "curl/curl.h"

#ifdef _DEBUG
#pragma comment(lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment(lib, "curl/libcurl_a.lib")
#endif

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

#ifdef _UNICODE
#define _CHAR_ wchar_t
#define _TEXT_(x) L ## x
#define _STR_ ::std::wstring

#define _main wmain
#else
#define _CHAR_ char
#define _TEXT_(x) x
#define _STR_ ::std::string

#define _main main
#endif

bool send_req(CURL*, _STR_&, _STR_&);
_STR_ create_request(_STR_, _STR_);
static int writer(_CHAR_*, size_t, size_t, _STR_*);
::std::string reparse(const ::std::string&);

int _main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	CURL* hcurl;

	curl_global_init(CURL_GLOBAL_ALL);
	hcurl = curl_easy_init();

	if (hcurl)
	{
		//_setmode(_fileno(stdout), _O_U16TEXT);
		//_setmode(_fileno(stdin), _O_U16TEXT);
		//_setmode(_fileno(stderr), _O_U16TEXT);

		_ClipBoard_ cb;
		_STR_ req;

		while (true)
		{
			_STR_ response;

			req = _TEXT_("");

			while (req == _TEXT_(""))
			{
				cb.getClipboardData__(req);
			}

			if (req == _TEXT_("close")) break;
			req = create_request(req, _TEXT_("ru"));

			if (!send_req(hcurl, req, response))
			{
				ERROR_
					return 1;
			}

			json sparsed = json::parse(response);
			::std::string sres = reparse(sparsed["text"].dump());
			::std::cout << sres << ::std::endl;
		}
	}
	else
	{
		curl_global_cleanup();
		ERROR_
			return 1;
	}

	curl_global_cleanup();

	system("pause");
	return 0;
}

bool send_req(CURL* hcurl, _STR_& req, _STR_& resp)
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
	res = curl_easy_setopt(hcurl, CURLOPT_WRITEDATA, &resp);
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

_STR_ create_request(_STR_ text, _STR_ target)
{
	_STR_ res = _TEXT_("key=trnsl.1.1.20190701T074727Z.305a7c00849d49b1.ba6e2f35026a74f023b638f65813428b936644e9&text=");

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

	res += text;
	res += _TEXT_("&lang=");
	res += target;
	res += _TEXT_("&format=plain&options=1");

	return res;
}

static int writer(_CHAR_* data, size_t size, size_t nmemb, _STR_* writerData)
{
	if (writerData == NULL)
		return 0;

	writerData->append(data, size*nmemb);

	return size * nmemb;
}

::std::string reparse(const ::std::string& temp)
{
	::std::string res = temp;
	res.erase(res.begin(), res.begin() + 2);
	res.erase(res.end() - 2, res.end());
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
			tempchar = static_cast<int>('\n');
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
	return res;
}