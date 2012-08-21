#include "QueueAdapter.h"
#include "TaskDispatcher.h"

namespace mtd
{
	QueueAdapter::QueueAdapter(QueuePtr queue, QueueProcessor& proc)
		: m_queue(queue)
		, m_processor(proc)
	{
	}

	QueueAdapter::QueueAdapter(const QueueAdapter& other)
		: m_queue(other.m_queue)
		, m_processor(other.m_processor)
	{
	}

	HolderTLSQueues& QueueAdapter::EnqueueAsyncTask(const TaskFunc& func)
	{
		auto ptrTLS = m_processor.GetTLSValue().get();
		ptrTLS->AddQueue();
		auto queue = ptrTLS->GetCurrentQueue();
		TaskPtr task(new Task(func), GetAsyncTaskDeleter(queue));
		m_queue->EnqueueAsync(std::move(task));
		return *ptrTLS;
	}

	void QueueAdapter::EnqueueSyncTask(const TaskFunc& func)
	{
		SynchronizationForTask sync;
		TaskPtr task(new Task(func), GetSyncTaskDeleter(sync));
		m_queue->EnqueueSync(std::move(task), sync);
	}

	HolderTLSQueues& QueueAdapter::EnqueueAsyncBarrier(const TaskFunc& func)
	{
		auto ptrTLS = m_processor.GetTLSValue().get();
		ptrTLS->AddQueue();
		auto queue = ptrTLS->GetCurrentQueue();
		TaskPtr task(new Barrier(func), GetAsyncTaskDeleter(queue));
		m_queue->EnqueueAsync(std::move(task));
		return *ptrTLS;
	}

	void QueueAdapter::EnqueueSyncBarrier(const TaskFunc& func)
	{
		SynchronizationForTask sync;
		TaskPtr task(new Barrier(func), GetSyncTaskDeleter(sync));
		m_queue->EnqueueSync(std::move(task), sync);
	}

	TaskDeleter QueueAdapter::GetSyncTaskDeleter(SynchronizationForTask& sync)
	{
		auto queue = m_queue;
		return [queue, &sync](Task* p)
		{
			delete p;
			queue->Decrease();
			sync.NotifySyncFinished();
		};
	}

	TaskDeleter QueueAdapter::GetAsyncTaskDeleter(BaseQueueTLSPtr queueTLS)
	{
		auto queue = m_queue;
		return [queue, queueTLS](Task* p)
		{
			queueTLS->TaskComplete();
			delete p;
			queue->Decrease();
		};
	}
}