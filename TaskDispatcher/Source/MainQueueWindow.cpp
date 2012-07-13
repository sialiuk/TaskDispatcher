#include "MainQueueWindow.h"

namespace mtd
{
	namespace
	{
		const LPCSTR CLASS_NAME = "MainQueueWindow";
		const UINT WM_PROCESS = ::RegisterWindowMessage("WM_PROCESS");
		

		LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			if(message == WM_PROCESS)
				return 0;
			return 0;
		}
	}

	MainQueueWindow::MainQueueWindow()
		: m_window(NULL)
	{
		RegisterMainWindow();
		CreateMainWindow();
	}

	MainQueueWindow::~MainQueueWindow()
	{
		DestroyWindow(m_window);
	}

	void MainQueueWindow::SendMainMessage()
	{
		SendMessage(m_window, WM_PROCESS, 0, 0);
	}

	void MainQueueWindow::PostMainMessage()
	{
		PostMessage(m_window, WM_PROCESS, 0, 0);
	}

	void MainQueueWindow::RegisterMainWindow()
	{
		WNDCLASSEX wcex = {0};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.lpszClassName = CLASS_NAME;
		wcex.lpfnWndProc = WindowProc;
		RegisterClassEx(&wcex);//обработка ошибок, исключение
	}

	void MainQueueWindow::CreateMainWindow()
	{
		m_window = CreateWindow(CLASS_NAME, "", 0, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
								nullptr, nullptr, nullptr, nullptr); //обработка ошибок, исключение
	}

	
}