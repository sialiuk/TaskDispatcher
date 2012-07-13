#pragma once

#include "Typedefs.h"
#include <Windows.h>

namespace mtd
{
	class MainQueueWindow : private boost::noncopyable
	{
	public:
		MainQueueWindow();
		~MainQueueWindow();
		void SendMainMessage();
		void PostMainMessage();
//		bool GetMainMessage();
	private:
		void CreateMainWindow();
		void RegisterMainWindow();
		HWND m_window;
	};
	typedef std::shared_ptr<MainQueueWindow> MainQueueWindowPtr;
}