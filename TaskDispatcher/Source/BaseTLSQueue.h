#pragma once

#include "Task.h"
#include "Typedefs.h"
#include <queue>

namespace mtd
{
	class BaseTLSQueue;
	typedef std::shared_ptr<Task> TLSTaskPtr;
	typedef std::shared_ptr<BaseTLSQueue> BaseTLSQueuePtr;

	class BaseTLSQueue
	{
	public:
		bool Empty() const;
		TLSTaskPtr Dequeue();
		void Enqueue(TLSTaskPtr);
		virtual ~BaseTLSQueue() { }
		virtual void TaskComplete() = 0;
		virtual bool IsComplete() const { return true; }
	private:
		std::queue<TLSTaskPtr> m_queue;
	};
}