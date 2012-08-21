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
		QueueAdapter(QueuePtr, QueueProcessor&);
		TaskDeleter GetSyncTaskDeleter(SynchronizationForTask&);
		TaskDeleter GetAsyncTaskDeleter(BaseQueueTLSPtr);
	private:
		QueuePtr m_queue;
		QueueProcessor& m_processor;
	};
}