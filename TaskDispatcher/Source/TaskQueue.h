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

	private:
		std::queue<TaskPtr>	m_tasks;
		Mutex				m_mutex;
	};

	typedef std::shared_ptr<TaskQueue> QueuePtr;
}