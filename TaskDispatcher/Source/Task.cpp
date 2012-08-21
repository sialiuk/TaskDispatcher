#include "Task.h"
#include "TaskQueue.h"

namespace mtd
{
	void Task::CallbackCaseException(std::exception_ptr)
	{
	}

	Task::Task(const TaskFunc& func, const FuncExcept& callback)
		: m_func(func)
		, m_callbackEx(callback)
	{
	}

	void Task::Execute()
	{
		try
		{
			m_func();
		}
		catch(...)
		{
			m_callbackEx(std::current_exception());
		}
	}

	bool Task::CanProcess(const TaskQueue& queue) const
	{
		return CanProcessImpl(queue);
	}

	bool Task::CanProcessImpl(const TaskQueue& queue) const
	{
		return true;
	}

	Barrier::Barrier(const TaskFunc& func, const FuncExcept& callback)
		: Task(func, callback)
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