#include "TaskDispatcher.h"

namespace mtd
{
	CreateQueueProcessorException::CreateQueueProcessorException()
		: std::runtime_error("Error creating QueueProcessor.")
	{
	}

	void QueueProcessor::OnTaskAdded()
	{
		Lock lock(m_mutexForNotify);
		m_cond.notify_one();
	}

	QueueProcessor::QueueProcessor()
	try
		: m_shouldShutdown(false)
		, m_window(new MainQueueWindow())
	{
		m_queues[HIGH] = std::make_shared<TaskQueue>(*this);
		m_queues[NORMAL] = std::make_shared<TaskQueue>(*this);
		m_queues[LOW] = std::make_shared<TaskQueue>(*this);
		m_mainThreadQueue = std::make_shared<TaskQueue>(*this);
		m_TLSPtr.reset(new HolderTLSMainQueues(m_window));
	}
	catch(const CreateWindowException&)
	{
		throw CreateQueueProcessorException();
	}
	catch(const RegisterWindowException&)
	{
		throw CreateQueueProcessorException();
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
		if (!m_shouldShutdown)
		{
			m_cond.wait(lock);
		}
	}

	void QueueProcessor::ProcessQueues()
	{
		m_TLSPtr.reset(new HolderTLSQueues());

		while(!ShouldShutdown())
		{
			m_TLSPtr.get()->ExecuteTasks();

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

	QueueAdapter QueueProcessor::GetQueue(Priority p)
	{
		auto it = m_queues.find(p);
		assert(it != m_queues.end());
		return QueueAdapter(it->second, *this);
	}

	MainQueueAdapter QueueProcessor::GetMainThreadQueue()
	{
		return MainQueueAdapter(m_mainThreadQueue, m_window);
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