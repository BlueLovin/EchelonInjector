#include "Drawing.h"

namespace Drawing {
	static void RegisterWindowClass()
	{
		WNDCLASS wc = { 0 };
		wc.lpfnWndProc = DefWindowProc;
		wc.hInstance = m_hInstance;
		wc.hCursor = LoadCursor(nullptr, IDC_HAND);
		wc.lpszClassName = szWindowClass;
		RegisterClass(&wc);
	}

	static void UnregisterWindowClass()
	{
		UnregisterClass(szWindowClass, m_hInstance);
	}

	static void Init(HINSTANCE hInstance)
	{
		hwnd = NULL;
		m_ptZero = { 0 };

		m_hInstance = hInstance;

		m_blend.BlendOp = AC_SRC_OVER;
		m_blend.AlphaFormat = AC_SRC_ALPHA;
		m_blend.SourceConstantAlpha = 0xff;
		m_blend.BlendFlags = NULL;

		CoInitialize(nullptr);
		RegisterWindowClass();
		hwnd = CreateWindowEx(WS_EX_LAYERED, szWindowClass, szTitle, WS_POPUP | WS_VISIBLE, 0, 0, 0, 0, nullptr, nullptr, m_hInstance, nullptr);
	}

	static RECT GetMainMonitorRect()
	{
		HMONITOR hmonitor = MonitorFromPoint(m_ptZero, MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO monitorinfo = { 0 };
		monitorinfo.cbSize = sizeof(monitorinfo);
		GetMonitorInfo(hmonitor, &monitorinfo);
		return monitorinfo.rcWork;
	}

	static HBITMAP CreateAlphaTextBitmap(std::wstring text, HFONT font, COLORREF color)
	{
		int TextLength = text.length();
		if (TextLength <= 0) return NULL;
		HDC hTextDC = CreateCompatibleDC(NULL);
		HFONT hOldFont = (HFONT)SelectObject(hTextDC, font);
		HBITMAP result = NULL;
		RECT text_area = { 0, 0, 0, 0 };
		DrawText(hTextDC, text.c_str(), TextLength, &text_area, DT_CALCRECT);
		if ((text_area.right > text_area.left) && (text_area.bottom > text_area.top))
		{
			BITMAPINFOHEADER bitmapinfoheader;
			memset(&bitmapinfoheader, 0x0, sizeof(BITMAPINFOHEADER));
			void *pvBits = NULL;

			bitmapinfoheader.biSize = sizeof(bitmapinfoheader);
			bitmapinfoheader.biWidth = text_area.right - text_area.left;
			bitmapinfoheader.biHeight = text_area.bottom - text_area.top;
			bitmapinfoheader.biPlanes = 1;
			bitmapinfoheader.biBitCount = 32;
			bitmapinfoheader.biCompression = BI_RGB;

			result = CreateDIBSection(hTextDC, (LPBITMAPINFO)&bitmapinfoheader, 0, (LPVOID *)&pvBits, NULL, 0);
			HBITMAP hOldBMP = (HBITMAP)SelectObject(hTextDC, result);
			if (hOldBMP != NULL)
			{
				SetTextColor(hTextDC, 0x00FFFFFF);
				SetBkColor(hTextDC, 0x00000000);
				SetBkMode(hTextDC, OPAQUE);

				DrawText(hTextDC, text.c_str(), TextLength, &text_area, DT_NOCLIP);
				BYTE *data_ptr = (BYTE *)pvBits;
				BYTE fill_r = GetRValue(color);
				BYTE fill_g = GetGValue(color);
				BYTE fill_b = GetBValue(color);
				for (int y = 0; y < bitmapinfoheader.biHeight; y++) {
					for (int x = 0; x < bitmapinfoheader.biWidth; x++) {
						BYTE this_a = *data_ptr;
						*data_ptr++ = (fill_b * this_a) >> 8;
						*data_ptr++ = (fill_g * this_a) >> 8;
						*data_ptr++ = (fill_r * this_a) >> 8;
						*data_ptr++ = this_a; // Set Alpha
					}
				}

				SelectObject(hTextDC, hOldBMP);
			}
		}

		SelectObject(hTextDC, hOldFont);
		DeleteDC(hTextDC);

		return result;
	}

	static void SetTransparentImageAndShowWindow(HWND hwnd, HBITMAP hbmp)
	{
		HDC hdcScreen = GetDC(nullptr);
		HDC hdcMem = CreateCompatibleDC(hdcScreen);
		HBITMAP hbmpOld = HBITMAP(SelectObject(hdcMem, hbmp));
		//----------------------------------Calculate top left position----------------------------------
		BITMAP bm;
		GetObject(hbmp, sizeof(bm), &bm);
		bmpSize = { bm.bmWidth, bm.bmHeight };
		//======= UnComment below if you want to show text at center of desktop==========================
		RECT rect = GetMainMonitorRect();
		position.x = rect.left + (rect.right - rect.left - bmpSize.cx) / 2;
		position.y = rect.bottom * 0.9;
		//----------------------------------Update window transparency-----------------------------------
		UpdateLayeredWindow(hwnd, hdcScreen, &position, &bmpSize, hdcMem, &m_ptZero, NULL, &m_blend, ULW_ALPHA);
		SetWindowPos(hwnd, HWND_TOPMOST, position.x, position.y, bmpSize.cx, bmpSize.cy, SWP_SHOWWINDOW);
		//----------------------------------delete temporary objects----------------------------------
		SelectObject(hdcMem, hbmpOld);
		DeleteDC(hdcMem);
		ReleaseDC(nullptr, hdcScreen);
	}

	static void Show(std::wstring text, HFONT font, COLORREF color)
	{
		HBITMAP MyBMP = CreateAlphaTextBitmap(text, font, color);
		if (MyBMP != nullptr)
		{
			SetTransparentImageAndShowWindow(hwnd, MyBMP);
		}
	}

	static void Hide()
	{
		SetWindowPos(hwnd, HWND_NOTOPMOST, position.x, position.y, bmpSize.cx, bmpSize.cy, SWP_HIDEWINDOW | SWP_NOACTIVATE);
	}

	static void Wait_ms(DWORD timeout)
	{
		UINT interval = 100;
		const DWORD dwStartTickCount = ::GetTickCount64();
		const UINT_PTR uniqueTimerId = 1234;
		SetTimer(hwnd, uniqueTimerId, interval, nullptr);
		while (GetTickCount64() - dwStartTickCount < timeout)
		{
			MSG msg;
			BOOL bRet;

			while ((bRet = GetMessage(&msg, hwnd, 0, 0)) != 0)
			{
				if (bRet == -1)
				{
					// handle the error and possibly exit
					return;
				}
				else
				{
					if (msg.message == WM_TIMER)
					{
						SetWindowPos(hwnd, HWND_TOPMOST, position.x, position.y, bmpSize.cx, bmpSize.cy, SWP_SHOWWINDOW);
						break;
					}
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		KillTimer(hwnd, uniqueTimerId);
	}
}