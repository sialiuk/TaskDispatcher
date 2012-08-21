#include "MainQueueWindow.h"

namespace mtd
{
	namespace
	{
		const LPCSTR CLASS_NAME = "MainQueueWindow";
		const UINT WM_PROCESS = ::RegisterWindowMessage("WM_PROCESS");
		const UINT WM_TLS_PROCESS = ::RegisterWindowMessage("WM_TLS_PROCESS");

		LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			if(message == WM_PROCESS)
			{
				TaskQueue *queue = reinterpret_cast<TaskQueue*>(wParam);
				if (queue)
				{
					auto task = queue->Dequeue();
					try
					{
						task->Execute();
					}
					catch(...)
					{
					
					}
				}
				return 0;
			}

			if(message == WM_TLS_PROCESS)
			{
				BaseQueueTLS *queue = reinterpret_cast<BaseQueueTLS*>(wParam);
				if (queue)
				{
					while(!queue->Empty())
					{
						auto task = queue->Dequeue();
						if(task)
						{
							try
							{
								task->Execute();
							}
							catch(...)
							{
					
							}
						}
					}
				}
				return 0;
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
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

	void MainQueueWindow::SendMainMessage(QueuePtr queue)
	{
		SendMessage(m_window, WM_PROCESS, reinterpret_cast<WPARAM>(queue.get()), 0);
	}

	void MainQueueWindow::PostMainMessage(QueuePtr queue)
	{
		PostMessage(m_window, WM_PROCESS, reinterpret_cast<WPARAM>(queue.get()), 0);
	}

	void MainQueueWindow::PostMainMessage(BaseQueueTLS* queue)
	{
		PostMessage(m_window, WM_TLS_PROCESS, reinterpret_cast<WPARAM>(queue), 0);
	}

	void MainQueueWindow::RegisterMainWindow()
	{
		WNDCLASSEX wcex = {0};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.lpszClassName = CLASS_NAME;
		wcex.lpfnWndProc = WindowProc;
		if(RegisterClassEx(&wcex) == NULL)	//обработка ошибок, исключение
		{
			throw CreateWindowException();
		}
	}

	void MainQueueWindow::CreateMainWindow()
	{
		m_window = CreateWindow(CLASS_NAME, "", 0, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
								 HWND_MESSAGE, nullptr, nullptr, nullptr);
		
		if(m_window == NULL)	//обработка ошибок, исключение
		{
			throw CreateWindowException();
		}
	}
}