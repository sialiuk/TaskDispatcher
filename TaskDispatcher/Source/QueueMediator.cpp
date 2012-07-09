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

	TaskPtr QueueMediator::CreateAsyncTask(Task* task)
	{
		auto q = m_queue;
		TaskPtr p(task,
		[q](Task* p)
		{
			delete p;
			q->Decrease();
		});
		return p;
	}

	TaskPtr QueueMediator::CreateSyncTask(Task* task)
	{
		auto q = m_queue;
		TaskPtr p(task, //TODO: restore make_shared
		[q](Task* p)
		{
			delete p;
			q->Decrease();
			q->NotifySyncFinished();
		});
		return p;
	}

	void QueueMediator::EnqueueAsyncTask(const TaskFunc& func)
	{
		TaskPtr task = CreateAsyncTask(new Task(func));
		m_queue->EnqueueAsync(std::move(task));
	}

	void QueueMediator::EnqueueSyncTask(const TaskFunc& func)
	{
		TaskPtr task = CreateSyncTask(new Task(func));
		m_queue->EnqueueSync(std::move(task));
	}

	void QueueMediator::EnqueueAsyncBarrier(const TaskFunc& func)
	{
		TaskPtr task = CreateAsyncTask(new Barrier(func));
		m_queue->EnqueueAsync(std::move(task));
	}

	void QueueMediator::EnqueueSyncBarrier(const TaskFunc& func)
	{
		TaskPtr task = CreateSyncTask(new Barrier(func));
		m_queue->EnqueueSync(std::move(task));
	}
}