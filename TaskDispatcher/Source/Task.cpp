#include "Task.h"

namespace mtd
{
	Task::Task(const TaskFunc& func): m_func(func)
	{
	}

	void Task::Execute()
	{
		m_func();
	}
}