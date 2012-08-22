#include "TLSQueue.h"

namespace mtd
{
/****************TLSQueue*********************/
	TLSQueue::TLSQueue(IQueueListener& listener)
		: m_result(false)
		, m_listener(listener)
	{
	}

	void TLSQueue::TaskComplete()
	{
		Lock lock(m_mutex);
		m_result = true;
		m_listener.TLSQueueAdded();
	}

	bool TLSQueue::IsComplete() const
	{
		Lock lock(m_mutex);
		return m_result;
	}

/****************MainTLSQueue*****************/
	MainTLSQueue::MainTLSQueue(MainQueueWindowPtr window)
		: m_window(window)
	{
	}

	void MainTLSQueue::TaskComplete()
	{
		m_window->PostMainMessage(this);
	}

/******************BaseTLSQueues******************/
	void BaseTLSQueues::ExecuteTasks()
	{
		std::for_each(m_queues.cbegin(), m_queues.cend(),
			[](BaseTLSQueuePtr queue)
			{
				if(queue->IsComplete())
				{
					while(!queue->Empty())
					{
						auto task = queue->Dequeue();
						task->Execute();
					}
				}
			});
	}

	BaseTLSQueuePtr BaseTLSQueues::GetCurrentQueue() const
	{
		return m_currentQueue;
	}

/******************HolderTLSQueues*******************/
	HolderTLSQueues::HolderTLSQueues(IQueueListener& listener)
		: m_listener(listener)
	{
	}

	void HolderTLSQueues::AddQueue()
	{
		BaseTLSQueuePtr queue(new TLSQueue(m_listener));
		m_queues.push_back(queue);
		m_currentQueue = m_queues.back();
	}

	HolderTLSQueues& HolderTLSQueues::Then(const TaskFunc& func)
	{
		m_currentQueue->Enqueue(std::make_shared<Task>(func));
		return *this;
	}

/*****************HolderMainTLSQueues******************/
	HolderMainTLSQueues::HolderMainTLSQueues(MainQueueWindowPtr window)
		: m_window(window)
	{
	}

	void HolderMainTLSQueues::AddQueue()
	{
		BaseTLSQueuePtr queue(new MainTLSQueue(m_window));
		m_queues.push_back(queue);
		m_currentQueue = m_queues.back();
	}

	HolderMainTLSQueues& HolderMainTLSQueues::Then(const TaskFunc& func)
	{
		m_currentQueue->Enqueue(std::make_shared<Task>(func));
		return *this;
	}
}