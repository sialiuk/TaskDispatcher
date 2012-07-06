#pragma once

#include "Task.h"
#include "Typedefs.h"
#include <queue>

namespace mtd
{
	class TaskQueue: private boost::noncopyable
	{
	public:
		TaskQueue();
		void Enqueue(TaskPtr);
		TaskPtr Dequeue();
		bool Empty() const;
		bool HasTasksToProcess() const;
		void Increase();
		void Decrease();
		size_t NumberOfRunningTask() const;

	private:
		std::queue<TaskPtr>	m_tasks;
		mutable Mutex m_mutex;
		size_t m_count;
	};
	typedef std::shared_ptr<TaskQueue> QueuePtr;
}