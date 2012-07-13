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
BOOL flag = TRUE;
std::string buffer = "Result: ";


template<typename Functor>
long double Function(long max, Functor callback)
{
	long temp = 0;
	long double result = 0;
	unsigned percent = 0;
	callback(percent);
	while(temp != max)
	{
		result += sqrt(double(rand()));
		++temp;
		unsigned newPercent = static_cast<unsigned>((static_cast<double>(temp)/ max) * 100);
		if (newPercent > percent)
		{
			percent = newPercent;
			callback(percent);
		}
	}
	return result;
}

template<typename Functor>
long double LongFunction(long max, Functor callback)
{
	long temp = 0;
	long double result = 0;
	unsigned percent = 0;
	callback(percent);
	while(temp != max)
	{
		result += sqrt(double(rand()));
		++temp;
		unsigned newPercent = static_cast<unsigned>((static_cast<double>(temp)/ max) * 100);
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
			
		}
	}
	return result;
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

//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
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

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
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


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
		
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	auto Func = [hWnd](unsigned perc)
	{
		percent = perc;
		RECT pos = {20, 20, 220, 60};
		InvalidateRect(hWnd, &pos, TRUE);
	};

	auto CallBack = [hWnd](unsigned perc)
	{
		PostMessage(hWnd, WM_PROCESS_DRAW, static_cast<WPARAM>(perc), 0);
	};

	auto DrawProgress = [&](HDC hdc, unsigned perc)
	{
		 Graphics graphics(hdc);
		 Pen blackPen(Color(255, 0, 0, 0), 2);
		 graphics.DrawRectangle(&blackPen, 20, 20, 2 * perc, 40);
	};

	if(message == WM_PROCESS_DRAW)
	{
		Func(static_cast<unsigned>(wParam));
	}

	if (message == WM_PROCESS_SEND)
	{
		long double r = Function(100322478, Func);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		return 0;
	}

	if (message == WM_PROCESS_POST)
	{
		long double r = Function(100322478, Func);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		return 0;
	}

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
				mtd::TaskDispatcher::Instance().GetQueue(mtd::NORMAL)
					.EnqueueAsyncTask(
						[Func]()
						{
							LongFunction(190322478,	Func);
						}
					);
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
			DrawProgress(hdc, percent);
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
