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

	MainTLSQueue::MainTLSQueue(MainQueueWindowPtr window)
		: m_window(window)
	{
	}

	void MainTLSQueue::TaskComplete()
	{
		m_window->PostMainMessage(this);
	}


	void HolderTLSQueues::AddQueue()
	{
		BaseTLSQueuePtr queue(new TLSQueue());
		m_queues.push_back(queue);
		m_currentQueue = m_queues.back();
	}

	HolderTLSQueues& HolderTLSQueues::Then(const TaskFunc& func)
	{
		m_currentQueue->Enqueue(std::make_shared<Task>(func));
		return *this;
	}

	BaseTLSQueuePtr HolderTLSQueues::GetCurrentQueue() const
	{
		return m_currentQueue;
	}

	void HolderTLSQueues::ExecuteTasks()
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
	
	UserTLSQueues& UserTLSQueues::Then(const TaskFunc& func)
	{
		m_currentQueue->Enqueue(std::make_shared<Task>(func));
		return *this;
	}
}