#pragma once

#include "../Source/TaskQueue.h"

namespace mtd
{
	class QueueMediator 
	{
		friend class QueueProcessor;
	public:
		void EnqueueAsyncTask(const TaskFunc& func);
		void EnqueueSyncTask(const TaskFunc& func);
		void EnqueueAsyncBarrier(const TaskFunc& func);
		void EnqueueSyncBarrier(const TaskFunc& func);
		QueueMediator(const QueueMediator&);
	private:
		QueueMediator(QueuePtr);
		void WaitForResult();
		TaskDeleter GetSyncTaskDeleter();
		TaskDeleter GetAsyncTaskDeleter();
	private:
		QueuePtr m_queue; 
	};
}