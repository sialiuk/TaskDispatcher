#pragma once

#include "Task.h"
#include "Typedefs.h"
#include <queue>

namespace mtd
{
	class TaskQueue: private boost::noncopyable
	{
	public:
		void Enqueue(TaskPtr);
		TaskPtr Dequeue();
		bool Empty() const;
	private:
		std::queue<TaskPtr>	m_tasks;
		mutable Mutex				m_mutex;
	};

	typedef std::shared_ptr<TaskQueue> QueuePtr;
}