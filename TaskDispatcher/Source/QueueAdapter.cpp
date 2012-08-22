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

	UserTLSQueues& QueueAdapter::EnqueueAsyncTask(const TaskFunc& func)
	{
		auto ptrTLS = m_processor.GetTLSValue().get();
		ptrTLS->AddQueue();
		TaskPtr task(new Task(func), GetAsyncTaskDeleter(ptrTLS->GetCurrentQueue()));
		m_queue->EnqueueAsync(std::move(task));
		return *ptrTLS;
	}

	void QueueAdapter::EnqueueSyncTask(const TaskFunc& func)
	{
		SynchronizationForTask sync;
		TaskPtr task(new Task(func), GetSyncTaskDeleter(sync));
		m_queue->EnqueueSync(std::move(task), sync);
	}

	UserTLSQueues& QueueAdapter::EnqueueAsyncBarrier(const TaskFunc& func)
	{
		auto ptrTLS = m_processor.GetTLSValue().get();
		ptrTLS->AddQueue();
		TaskPtr task(new Barrier(func), GetAsyncTaskDeleter(ptrTLS->GetCurrentQueue()));
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

	TaskDeleter QueueAdapter::GetAsyncTaskDeleter(BaseTLSQueuePtr TLSqueue)
	{
		auto queue = m_queue;
		return [queue, TLSqueue](Task* p)
		{
			delete p;
			queue->Decrease();
			TLSqueue->TaskComplete();
		};
	}
}