#pragma once

#include "TaskQueue.h"
#include "MainQueueWindow.h"

namespace mtd
{
	class MainQueueAdapter 
	{
		friend class QueueProcessor;
	public:
		void EnqueueAsyncTask(const TaskFunc& func);
		void EnqueueSyncTask(const TaskFunc& func);
		MainQueueAdapter(const MainQueueAdapter&);
	private:
		MainQueueAdapter(QueuePtr, MainQueueWindowPtr);
	private:
		QueuePtr m_queue;
		MainQueueWindowPtr m_window;
	};
}