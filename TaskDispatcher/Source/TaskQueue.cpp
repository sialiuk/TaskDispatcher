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
}