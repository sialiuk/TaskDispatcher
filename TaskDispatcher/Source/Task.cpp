#include "Task.h"
#include "TaskQueue.h"

namespace mtd
{
	Task::Task(const TaskFunc& func)
		: m_func(func)
	{
	}

	void Task::Execute()
	{
			m_func();
	}

	bool Task::CanProcess(const TaskQueue& queue) const
	{
		return CanProcessImpl(queue);
	}

	bool Task::CanProcessImpl(const TaskQueue& queue) const
	{
		return true;
	}

	Barrier::Barrier(const TaskFunc& func)
		: Task(func)
	{
	}

	bool Barrier::CanProcessImpl(const TaskQueue& queue) const
	{
		if(queue.NumberOfRunningTask() == 0)
		{
			return true;
		}
		return false;
	}
}