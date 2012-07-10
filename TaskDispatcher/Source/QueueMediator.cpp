#include "QueueMediator.h"
#include "TaskDispatcher.h"

namespace mtd
{
	QueueMediator::QueueMediator(QueuePtr queue)
		: m_queue(queue)
	{
	}

	QueueMediator::QueueMediator(const QueueMediator& other)
		: m_queue(other.m_queue)
	{
	}

	void QueueMediator::EnqueueAsyncTask(const TaskFunc& func)
	{
		TaskPtr task(new Task(func), GetAsyncTaskDeleter());
		m_queue->EnqueueAsync(std::move(task));
	}

	void QueueMediator::EnqueueSyncTask(const TaskFunc& func)
	{
		TaskPtr task(new Task(func), GetSyncTaskDeleter());
		m_queue->EnqueueSync(std::move(task));
	}

	void QueueMediator::EnqueueAsyncBarrier(const TaskFunc& func)
	{
		TaskPtr task(new Barrier(func), GetAsyncTaskDeleter());
		m_queue->EnqueueAsync(std::move(task));
	}

	void QueueMediator::EnqueueSyncBarrier(const TaskFunc& func)
	{
		TaskPtr task(new Barrier(func), GetSyncTaskDeleter());
		m_queue->EnqueueSync(std::move(task));
	}

	TaskDeleter QueueMediator::GetSyncTaskDeleter()
	{
		return [this](Task* p)
		{
			delete p;
			m_queue->Decrease();
			m_queue->NotifySyncFinished();
		};
	}

	TaskDeleter QueueMediator::GetAsyncTaskDeleter()
	{
		return [this](Task* p)
		{
			delete p;
			m_queue->Decrease();
		};
	}
}