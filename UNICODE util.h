#pragma once

#ifdef _UNICODE
#ifndef _CHAR_
#define _CHAR_ wchar_t
#else
#error is_defined!
#endif
#ifndef _TEXT_
#define _TEXT_(x) L ## x
#else
#error is_defined!
#endif
#ifndef _STR_
#define _STR_ ::std::wstring
#else
#error is_defined!
#endif
#ifndef _COUT_
#define _COUT_ ::std::wcout
#else
#error is_defined!
#endif
#ifndef _CB_FORMAT_
#define _CB_FORMAT_ CF_UNICODETEXT
#else
#error is_defined!
#endif
#ifndef _main
#define _main wmain
#else
#error is_defined!
#endif
#else
#ifndef _CHAR_
#define _CHAR_ char
#else
#error is_defined!
#endif
#ifndef _TEXT_
#define _TEXT_(x) x
#else
#error is_defined!
#endif
#ifndef _STR_
#define _STR_ ::std::string
#else
#error is_defined!
#endif
#ifndef _COUT_
#define _COUT_ ::std::cout
#else
#error is_defined!
#endif
#ifndef _CB_FORMAT_
#define _CB_FORMAT_ CF_TEXT
#else
#error is_defined!
#endif
#ifndef _main
#define _main main
#else
#error is_defined!
#endif
#endif