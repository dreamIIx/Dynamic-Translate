#pragma once

/*
	Autor - -dreamIIx
GitHub - https://github.com/dreamIIx
Release [v0.1] on GitHub 03.07.2019
Actual version 3.0
This project is an "Dynamic Translate" programm, than supports to translate *any* text *fast*.
															  !!!This project uses a Yandex.Translate API!!!
*/


class _ClipBoard_
{
public:
	HANDLE hData;
	bool isopen;
	_CHAR_* pbuf;

	_ClipBoard_()
	{
		isopen = false;
		pbuf = nullptr;
	}

	void getClipboardData__(_STR_& buf_)
	{
		isopen = false;
		pbuf = nullptr;

		isopen = OpenClipboard(NULL);

		if (isopen)
		{
			hData = GetClipboardData(_CB_FORMAT_);
			if (hData)
			{
				pbuf = (_CHAR_*)GlobalLock(hData);

				if (pbuf != nullptr)
				{
					buf_ = pbuf;
				}
				
				GlobalUnlock(hData);
			}
			CloseClipboard();
			isopen = false;
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