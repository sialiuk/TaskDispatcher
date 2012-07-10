#include "TaskDispatcher.h"

namespace mtd
{
	void QueueProcessor::OnTaskAdded()
	{
		Lock lock(m_mutexForNotify);
		m_cond.notify_all();
	}

	QueueProcessor::QueueProcessor()
		: m_shouldShutdown(false)
	{
		m_queues[HIGH] = QueuePtr(new TaskQueue(*this));
		m_queues[NORMAL] = QueuePtr(new TaskQueue(*this));
		m_queues[LOW] = QueuePtr(new TaskQueue(*this));
	}

	ThreadRoutine QueueProcessor::GetThreadRoutine()
	{
		return [this](){ProcessQueues();};
	}
	
	QueuePtr QueueProcessor::GetNonEmptyQueue()
	{
		auto it = std::find_if(m_queues.begin(), m_queues.end(),
			[](std::pair<Priority, QueuePtr> it)
			{
				return it.second->HasTasksToProcess(); // есть задачи для выполнения?
			}
		);
		if (it != m_queues.end())
		{
			return it->second;
		}
		return QueuePtr();
	}

	TaskPtr QueueProcessor::GetTask()
	{
		Lock lock(m_mutexForGetTask);
		auto queue = GetNonEmptyQueue();
		if (queue)
		{
			return queue->Dequeue();
		}
		return TaskPtr();
	}

	void QueueProcessor::Shutdown()
	{
		Lock lock(m_mutexForNotify);
		m_shouldShutdown = true;
		m_cond.notify_all();
	}

	bool QueueProcessor::ShouldShutdown() const
	{
		Lock lock(m_mutexForNotify);
		return m_shouldShutdown;
	}

	void QueueProcessor::WaitForChanges()
	{
		Lock lock(m_mutexForNotify);
		m_cond.wait(lock);
	}

	void QueueProcessor::ProcessQueues()
	{
		while(!ShouldShutdown())
		{
			auto task = GetTask();
			if (task)
			{
				try
				{
					task->Execute();
				}
				catch(const std::exception&)
				{
					//CurrentException(ex.what());
				}
				catch(...)
				{
				
				}
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
		return QueueMediator(it->second);
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