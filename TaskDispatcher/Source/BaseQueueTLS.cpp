#include "BaseQueueTLS.h"

namespace mtd
{

	bool BaseQueueTLS::Empty() const
	{
		return m_queue.empty();
	}

	TLSTaskPtr BaseQueueTLS::Dequeue()
	{
		TLSTaskPtr task(m_queue.front());
		m_queue.pop();
		return task;
	}

	void BaseQueueTLS::Enqueue(TLSTaskPtr task)
	{
		m_queue.push(task);
	}
}