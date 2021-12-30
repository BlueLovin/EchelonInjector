#pragma once
#include "Shlwapi.h"
#include <string>

namespace Drawing {
	static HINSTANCE m_hInstance;
	static BLENDFUNCTION m_blend;
	static POINT m_ptZero;
	static HWND hwnd;
	static SIZE bmpSize;
	static POINT position;

	static TCHAR szWindowClass[] = (L"DesktopApp");
	static TCHAR szTitle[] = (L"EchelonInjectorOverlay");

	static void RegisterWindowClass();
	static void UnregisterWindowClass();
	static void Init(HINSTANCE hInstance);
	static RECT GetMainMonitorRect();
	static HBITMAP CreateAlphaTextBitmap(std::wstring text, HFONT font, COLORREF color);
	static void SetTransparentImageAndShowWindow(HWND hwnd, HBITMAP hbmp);
	static void Show(std::wstring text, HFONT font, COLORREF color);
	static void Hide();
	static void Wait_ms(DWORD timeout);
}