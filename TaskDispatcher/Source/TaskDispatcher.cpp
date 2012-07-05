#include "TaskDispatcher.h"

namespace mtd
{
	QueueProcessor::QueueProcessor()
		: m_shouldShutdown(false)
	{
		m_queues[HIGH] = QueuePtr(new TaskQueue());
		m_queues[NORMAL] = QueuePtr(new TaskQueue());
		m_queues[LOW] = QueuePtr(new TaskQueue());
	}

	ThreadRoutine QueueProcessor::GetThreadRoutine()
	{
		return [this](){ProcessQueues();};
	}
	
	QueuePtr QueueProcessor::GetNonEmptyQueue()
	{
		QueuePtr high = m_queues[HIGH];
		QueuePtr normal = m_queues[NORMAL];
		QueuePtr low = m_queues[LOW];
		
		if(!high->Empty())
		{
			return high;
		}

		if(!normal->Empty())
		{
			return normal;
		}

		if(!low->Empty())
		{
			return low;
		}
		
		return QueuePtr();
	}

	TaskPtr QueueProcessor::GetTask()
	{
		Lock lock(m_mutex);
		auto queue = GetNonEmptyQueue();
		if (queue)
		{
			return queue->Dequeue();
		}
		return TaskPtr();
	}

	void QueueProcessor::Shutdown()
	{
		Lock lock(m_mutex);
		m_shouldShutdown = true;
		m_cond.notify_all();
	}

	bool QueueProcessor::ShouldShutdown() const
	{
		Lock lock(m_mutex);
		return m_shouldShutdown;
	}

	void QueueProcessor::WaitForChanges()
	{
		Lock lock(m_mutex);
		m_cond.wait(lock);
	}

	void QueueProcessor::NotifyAboutChanges()
	{
		Lock lock(m_mutex);
		m_cond.notify_all();
	}

	void QueueProcessor::ProcessQueues()
	{
		while(!ShouldShutdown())
		{
			TaskPtr task = GetTask();
			if (task)
			{
				task->Execute();
			}
			else
			{
				WaitForChanges();
			}
		}
	}

	QueueMediator QueueProcessor::GetQueue(Priority p)
	{
		auto it = m_queues.find(p);
		assert(it != m_queues.end());
		return QueueMediator(it->second, *this);
	}

	TaskDispatcher::TaskDispatcher()
		: m_pool(GetThreadRoutine())
	{
	
	}

	TaskDispatcher::~TaskDispatcher()
	{
		Shutdown();
	}

	TaskDispatcher& TaskDispatcher::Instance()
	{
		static TaskDispatcher td;
		return td;
	}

}