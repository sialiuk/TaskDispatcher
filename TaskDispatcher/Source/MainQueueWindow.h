#pragma once

#include "Typedefs.h"
#include "TaskQueue.h"
#include "BaseQueueTLS.h"
#include <Windows.h>

namespace mtd
{
	class CreateWindowException
	{
	};

	class RegisterWindowException
	{
	};

	class MainQueueWindow : private boost::noncopyable
	{
	public:
		MainQueueWindow();
		~MainQueueWindow();
		void SendMainMessage(QueuePtr queue);
		void PostMainMessage(QueuePtr queue);
		void PostMainMessage(BaseQueueTLS* queue);
	private:
		void CreateMainWindow();
		void RegisterMainWindow();
		HWND m_window;
	};
	typedef std::shared_ptr<MainQueueWindow> MainQueueWindowPtr;
}