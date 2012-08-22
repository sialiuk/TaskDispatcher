#pragma once

#include "TaskQueue.h"
#include "TLSQueue.h"

namespace mtd
{
	class QueueAdapter 
	{
		friend class QueueProcessor;
	public:
		UserTLSQueues& EnqueueAsyncTask(const TaskFunc& func);
		void EnqueueSyncTask(const TaskFunc& func);
		UserTLSQueues& EnqueueAsyncBarrier(const TaskFunc& func);
		void EnqueueSyncBarrier(const TaskFunc& func);
		QueueAdapter(const QueueAdapter&);
	private:
		QueueAdapter(QueuePtr queue, QueueProcessor& proc);
		TaskDeleter GetSyncTaskDeleter(SynchronizationForTask& sync);
		TaskDeleter GetAsyncTaskDeleter(BaseTLSQueuePtr TLSqueue);
	private:
		QueuePtr m_queue;
		QueueProcessor& m_processor;
	};
}