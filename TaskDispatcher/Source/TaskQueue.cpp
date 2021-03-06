#include "TaskQueue.h"

namespace mtd
{
	TLSQueue::TLSQueue()
		: m_result(false)
	{
	}

	void TLSQueue::TaskComplete()
	{
		Lock lock(m_mutex);
		m_result = true;
	}

	bool TLSQueue::IsComplete() const
	{
		Lock lock(m_mutex);
		return m_result;
	}

	void TLSQueue::Enqueue(TLSTaskPtr task)
	{
		m_queue.push(task);
	}

	TLSQueue::TLSTaskPtr TLSQueue::Dequeue()
	{
		TLSTaskPtr task(m_queue.front());
		m_queue.pop();
		return task;
	}



	SynchronizationForTask::SynchronizationForTask()
		: m_flagOnExecute(false)
	{
	}

	void SynchronizationForTask::WaitForSyncFinished()
	{
		Lock lock(m_mutex);
		if(!m_flagOnExecute)
		{
			m_cond.wait(lock);
		}
	}

	void SynchronizationForTask::NotifySyncFinished()
	{
		Lock lock(m_mutex);
		m_cond.notify_one();
		m_flagOnExecute = true;
	}


	TaskQueue::TaskQueue(IQueueListener& listener)
		: m_count(0)
		, m_listener(listener)
	{
	}

	TaskQueue::~TaskQueue()
	{
		
	}
	
	void TaskQueue::EnqueueAsync(TaskPtr&& t)
	{
		Lock lock(m_mutex);
		m_tasks.push(std::move(t));
		m_listener.OnTaskAdded();
	}

	void TaskQueue::EnqueueSync(TaskPtr&& t, SynchronizationForTask& sync)
	{
		EnqueueAsync(std::move(t));
		sync.WaitForSyncFinished();
	}

	void TaskQueue::Enqueue(TaskPtr&& t)
	{
		Lock lock(m_mutex);
		m_tasks.push(std::move(t));
	}

	TaskPtr TaskQueue::Dequeue()
	{
		Lock lock(m_mutex);
		TaskPtr task;
		task.swap(m_tasks.front());
		m_tasks.pop();
		++m_count;
		return task;
	}

	bool TaskQueue::Empty() const
	{
		Lock lock(m_mutex);
		return m_tasks.empty();
	}

	bool TaskQueue::HasTasksToProcess() const
	{
		Lock lock(m_mutex);
		if (!m_tasks.empty())
		{
			auto& task = m_tasks.front();
			return task->CanProcess(*this);
		}
		return false;
	}

	void TaskQueue::Increase()
	{
		Lock lock(m_mutex);
		++m_count;
	}

	void TaskQueue::Decrease()
	{
		Lock lock(m_mutex);
		--m_count;
	}

	size_t TaskQueue::NumberOfRunningTask() const
	{
		return m_count;
	}
}