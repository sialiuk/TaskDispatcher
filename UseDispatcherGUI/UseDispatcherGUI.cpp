// UseDispatcherGUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "UseDispatcherGUI.h"
#include <math.h>
#include <Dispatcher.h>
#include <objidl.h>
#include <GdiPlus.h>
#include <string>
#include <boost\lexical_cast.hpp>
#include <sstream>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#define MAX_LOADSTRING 100

const UINT WM_PROCESS_SEND = ::RegisterWindowMessage(L"WM_PROCESS_SEND");//WM_USER + 1;
const UINT WM_PROCESS_POST = ::RegisterWindowMessage(L"WM_PROCESS_POST");//WM_USER + 2;
const UINT WM_SHOW_RESULT = ::RegisterWindowMessage(L"QWERFJKL");
const UINT WM_PROCESS_DRAW = ::RegisterWindowMessage(L"WM_PROCESS_DRAW");

namespace
{
	unsigned percent = 0;
	static size_t divider = 4;
	BOOL flag = TRUE;
	std::string buffer = "Result: ";

	template<typename Functor>
	long double Function(long max, size_t part, Functor callback)
	{
		long temp = 0;
		long double result = 0;
		int percent = 0;
		while(temp <= max)
		{
			result += sqrt(sqrt(double(rand())) + sqrt(double(rand())));
			unsigned newPercent = static_cast<unsigned>((static_cast<double>(temp) / max) * 100);
			++temp;
			newPercent /= unsigned(part);
			if (newPercent > percent)
			{
				percent = newPercent;
				mtd::TaskDispatcher::Instance().GetMainThreadQueue().EnqueueAsyncTask
				(
					[callback]()
					{
						callback();
					}
				);
			}
		}
		return result;
	}

	template<typename Functor, typename Functor2>
	long double LongFunction(long max, Functor callback, Functor2 callback2)
	{
		long temp = 0;
		long double result = 0;
		unsigned percent = 0;
		callback(percent);
		while(temp != max)
		{
			result += sqrt(double(rand()));
			++temp;
			unsigned newPercent = static_cast<unsigned>((static_cast<double>(temp) / max) * 100);
			if (newPercent > percent)
			{
				percent = newPercent;
				mtd::TaskDispatcher::Instance().GetMainThreadQueue().EnqueueAsyncTask
				(
					[percent, callback]()
					{
						callback(percent);
					}
				);
				if (percent % 10 == 0)
				{
					mtd::TaskDispatcher::Instance().GetMainThreadQueue().EnqueueSyncTask
					(
						[callback2]()
						{
							callback2();
						}
					);
				}
			
			}
		}
		return result;
	}

	template<typename Functor>
	void DividerTasks(long iteration, size_t part, Functor callback)
	{
		long taskIteration = iteration / part;
		auto queue = mtd::TaskDispatcher::Instance().GetQueue(mtd::HIGH);
		for(size_t i = 0; i != part; ++i)
		{
			queue.EnqueueAsyncTask(
				[taskIteration, part, callback]()
			{
				Function(taskIteration, part, callback);
			});
		}
	}
}

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
   
	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_USEDISPATCHERGUI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_USEDISPATCHERGUI));
	
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	GdiplusShutdown(gdiplusToken);
	return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_USEDISPATCHERGUI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_USEDISPATCHERGUI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	auto AsyncFunc = [hWnd]()
	{
		++percent;
		RECT pos = {20, 20, 321, 62};
		InvalidateRect(hWnd, &pos, TRUE);
	};

	//auto a = About;
	auto SyncFunc = [hWnd]()
	{
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
	};

	auto CallBack = [hWnd](unsigned perc)
	{
		PostMessage(hWnd, WM_PROCESS_DRAW, static_cast<WPARAM>(perc), 0);
	};

	auto DrawProgress = [&](HDC hdc, unsigned perc, size_t divider)
	{
		Graphics graphics(hdc);
		Rect rectBack(20, 20, 3 * 100, 40);
		Rect rect(20, 20, 3 * perc, 40);
		Pen pen(Color(255, 0, 0, 0), 1);
		SolidBrush brushBack(Color(200, 200, 200));
		SolidBrush brush(Color(180, 200, 180));
		graphics.FillRectangle(&brushBack, rectBack);
		graphics.DrawRectangle(&pen, rectBack);
		graphics.FillRectangle(&brush, rect);
		graphics.DrawRectangle(&pen, rect);
		Font myFont(L"Arial", 16);
		RectF layoutRect(20, 30, 300, 40);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		SolidBrush blackBrush(Color(255, 0, 0, 0));
		std::wstringstream ss;
		//perc /= unsigned(divider);
		ss << perc << L" %";
		auto s = ss.str();
		graphics.DrawString(s.c_str(), s.size(), &myFont, layoutRect, &format, &blackBrush);
	};

	if (message == WM_SHOW_RESULT)
	{
		flag = FALSE;
		buffer += boost::lexical_cast<std::string, double>(static_cast<double>(wParam));
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	}

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_USINGSENDMESSAGE:
			SendMessage(hWnd, WM_PROCESS_SEND, 0, 0);
			break;
		case ID_USINGPOSTMESSAGE:
			PostMessage(hWnd, WM_PROCESS_POST, 0, 0);
			break;
		case ID_USINGTASKDISPATCHER:
			{
				percent = 0;
				DividerTasks(190322478, divider, AsyncFunc);
			}
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		if(flag)
		{
			DrawProgress(hdc, percent, divider);
		}
		else
		{
			 RECT pos = {30, 30, 200, 50};
             DrawTextA(hdc, buffer.c_str(), -1, &pos, DT_CENTER); 
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
