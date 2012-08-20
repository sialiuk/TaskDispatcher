#pragma once

#include "Task.h"
#include "Typedefs.h"
#include <queue>

namespace mtd
{
	class BaseQueueTLS;
	typedef std::shared_ptr<Task> TLSTaskPtr;
	typedef std::shared_ptr<BaseQueueTLS> BaseQueueTLSPtr;

	class BaseQueueTLS
	{
	public:
		bool Empty() const;
		TLSTaskPtr Dequeue();
		void Enqueue(TLSTaskPtr);
		virtual void TaskComplete() = 0;
	private:
		std::queue<TLSTaskPtr> m_queue;
	};
}