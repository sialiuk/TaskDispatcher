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
		TaskPtr task(new Task(func), GetSyncTaskDeleter());
		m_queue->EnqueueSync(std::move(task));
	}

	void QueueAdapter::EnqueueAsyncBarrier(const TaskFunc& func)
	{
		TaskPtr task(new Barrier(func), GetAsyncTaskDeleter());
		m_queue->EnqueueAsync(std::move(task));
	}

	void QueueAdapter::EnqueueSyncBarrier(const TaskFunc& func)
	{
		TaskPtr task(new Barrier(func), GetSyncTaskDeleter());
		m_queue->EnqueueSync(std::move(task));
	}

	TaskDeleter QueueAdapter::GetSyncTaskDeleter()
	{
		return [this](Task* p)
		{
			delete p;
			m_queue->Decrease();
			m_queue->NotifySyncFinished();
		};
	}

	TaskDeleter QueueAdapter::GetAsyncTaskDeleter()
	{
		return [this](Task* p)
		{
			delete p;
			m_queue->Decrease();
		};
	}
}