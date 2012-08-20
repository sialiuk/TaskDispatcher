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
}