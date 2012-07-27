#include "QueueAdapter.h"
#include "TaskDispatcher.h"

namespace mtd
{
	QueueAdapter::QueueAdapter(QueuePtr queue)
		: m_queue(queue)
	{
	}

	QueueAdapter::QueueAdapter(const QueueAdapter& other)
		: m_queue(other.m_queue)
	{
	}

	void QueueAdapter::EnqueueAsyncTask(const TaskFunc& func)
	{
		TaskPtr task(new Task(func), GetAsyncTaskDeleter());
		m_queue->EnqueueAsync(std::move(task));
	}

	void QueueAdapter::EnqueueSyncTask(const TaskFunc& func)
	{
		ConditionVariable condition;
		TaskPtr task(new Task(func), GetSyncTaskDeleter(condition));
		m_queue->EnqueueSync(std::move(task), condition);
	}

	void QueueAdapter::EnqueueAsyncBarrier(const TaskFunc& func)
	{
		TaskPtr task(new Barrier(func), GetAsyncTaskDeleter());
		m_queue->EnqueueAsync(std::move(task));
	}

	void QueueAdapter::EnqueueSyncBarrier(const TaskFunc& func)
	{
		ConditionVariable condition;
		TaskPtr task(new Barrier(func), GetSyncTaskDeleter(condition));
		m_queue->EnqueueSync(std::move(task), condition);
	}

	TaskDeleter QueueAdapter::GetSyncTaskDeleter(ConditionVariable& c)
	{
		auto queue = m_queue;
		return [queue, &c](Task* p)
		{
			delete p;
			queue->Decrease();
			c.notify_one();
			//queue->NotifySyncFinished();
		};
	}

	TaskDeleter QueueAdapter::GetAsyncTaskDeleter()
	{
		auto queue = m_queue;
		return [queue](Task* p)
		{
			delete p;
			queue->Decrease();
		};
	}
}