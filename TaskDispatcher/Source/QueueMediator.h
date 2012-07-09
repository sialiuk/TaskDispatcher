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
		TaskPtr CreateAsyncTask(Task* task);
		TaskPtr CreateSyncTask(Task* task);
		void WaitForResult();
	private:
		QueuePtr m_queue; 
	};
}