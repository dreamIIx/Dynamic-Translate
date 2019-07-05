#pragma once

class _ClipBoard_
{
public:
	HANDLE hData;
	bool isopen;
	_CHAR_* pbuf;
	_STR_ buf;
	//_STR_ buftemp;

	_ClipBoard_()
	{
		isopen = false;
		pbuf = nullptr;
		buf = _TEXT_("");
		//buftemp = _TEXT_("");
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
				pbuf = reinterpret_cast<_CHAR_*>(GlobalLock(hData));

				if (pbuf != nullptr)
				{
					buf = pbuf;
					//buftemp = buf;
					buf_ = buf;
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