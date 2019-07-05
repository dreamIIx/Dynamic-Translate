#pragma once

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
				pbuf = reinterpret_cast<_CHAR_*>(GlobalLock(hData));

				if (pbuf != nullptr)
				{
					buf_ = pbuf;
					//buftemp = buf;
					//buf_ = buf;
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