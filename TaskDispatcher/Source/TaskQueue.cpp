#include "TaskQueue.h"

namespace mtd
{
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
		return taskPtr;
	}
}