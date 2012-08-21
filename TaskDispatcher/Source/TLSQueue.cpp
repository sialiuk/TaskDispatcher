#include "TLSQueue.h"

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

	TLSMainQueue::TLSMainQueue(MainQueueWindowPtr window)
		: m_window(window)
	{
	}

	void TLSMainQueue::TaskComplete()
	{
		m_window->PostMainMessage(std::make_shared<TLSMainQueue>(*this));
	}


	void HolderTLSQueues::AddQueue()
	{
		BaseQueueTLSPtr queue(new TLSQueue());
		m_currentQueue = queue;
		m_queues.push_back(queue);
	}

	HolderTLSQueues& HolderTLSQueues::Then(const TaskFunc& func)
	{
		m_currentQueue->Enqueue(std::make_shared<Task>(func));
		return *this;
	}

	BaseQueueTLSPtr HolderTLSQueues::GetCurrentQueue() const
	{
		return m_currentQueue;
	}

	void HolderTLSQueues::ExecuteTasks()
	{
		for(size_t i = 0; i != m_queues.size(); ++i)
		{
			auto queue = m_queues[i];
			while(!queue->Empty())
			{
				auto task = queue->Dequeue();
				try
				{
					task->Execute();
				}
				catch(...)
				{
					
				}
			}
		}
	}

	HolderTLSMainQueues::HolderTLSMainQueues(MainQueueWindowPtr window)
		: m_window(window)
	{
	}

	void HolderTLSMainQueues::AddQueue()
	{
		BaseQueueTLSPtr queue(new TLSMainQueue(m_window));
		m_currentQueue = queue;
		m_queues.push_back(queue);
	}

	HolderTLSMainQueues& HolderTLSMainQueues::Then(const TaskFunc& func)
	{
		m_currentQueue->Enqueue(std::make_shared<Task>(func));
		return *this;
	}
	
}