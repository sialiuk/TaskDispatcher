#include "TaskQueue.h"

namespace mtd
{
	TaskQueue::TaskQueue()
		:m_count(0)
	{
	}
	//Enqueue/Dequeue
	void TaskQueue::Enqueue(TaskPtr t)
	{
		Lock lock(m_mutex);
		m_tasks.push(t);
	}

	TaskPtr TaskQueue::Dequeue()
	{
		Lock lock(m_mutex);
		TaskPtr taskPtr = m_tasks.front();
		m_tasks.pop();
		++m_count;
		return taskPtr;
	}

	bool TaskQueue::Empty() const
	{
		Lock lock(m_mutex);
		return m_tasks.empty();
	}

	bool TaskQueue::HasTasksToProcess() const
	{
		Lock lock(m_mutex);
		if (!m_tasks.empty())
		{
			auto& task = m_tasks.front();
			return task->CanProcess(*this);
		}
		return false;
	}

	void TaskQueue::Increase()
	{
		Lock lock(m_mutex);
		++m_count;
	}

	void TaskQueue::Decrease()
	{
		Lock lock(m_mutex);
		--m_count;
	}

	size_t TaskQueue::NumberOfRunningTask() const
	{
		return m_count;
	}
}