#pragma once

#include "../Source/TaskQueue.h"

namespace mtd
{
	class QueueAdapter 
	{
		friend class QueueProcessor;
	public:
		void EnqueueAsyncTask(const TaskFunc& func);
		void EnqueueSyncTask(const TaskFunc& func);
		void EnqueueAsyncBarrier(const TaskFunc& func);
		void EnqueueSyncBarrier(const TaskFunc& func);
		QueueAdapter(const QueueAdapter&);
	private:
		QueueAdapter(QueuePtr);
		//void WaitForResult();
		TaskDeleter GetSyncTaskDeleter(ConditionVariable&);
		TaskDeleter GetAsyncTaskDeleter();
	private:
		QueuePtr m_queue; 
	};
}