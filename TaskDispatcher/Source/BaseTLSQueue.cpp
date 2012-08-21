#include "BaseTLSQueue.h"

namespace mtd
{

	bool BaseTLSQueue::Empty() const
	{
		return m_queue.empty();
	}

	TLSTaskPtr BaseTLSQueue::Dequeue()
	{
		TLSTaskPtr task(m_queue.front());
		m_queue.pop();
		return task;
	}

	void BaseTLSQueue::Enqueue(TLSTaskPtr task)
	{
		m_queue.push(task);
	}
}