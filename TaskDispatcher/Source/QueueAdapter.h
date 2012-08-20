#pragma once

#include "TaskQueue.h"

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
		TaskDeleter GetSyncTaskDeleter(SynchronizationForTask&);
		TaskDeleter GetAsyncTaskDeleter();
	private:
		QueuePtr m_queue; 
	};
}