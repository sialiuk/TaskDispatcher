#pragma once

#include "Typedefs.h"

namespace mtd
{
	typedef boost::function<void ()> TaskFunc;

	class TaskQueue;

	class Task: private boost::noncopyable
	{
	public:
		Task(const TaskFunc&);
		void Execute();
		bool CanProcess(const TaskQueue& queue) const;
	private:
		virtual bool CanProcessImpl(const TaskQueue& queue) const;	
	private:
		TaskFunc m_func;
	};

	class Barrier : public Task
	{
	public:
		Barrier(const TaskFunc&);
	private:
		virtual bool CanProcessImpl(const TaskQueue& queue) const;
	};

	typedef std::shared_ptr<Task> TaskPtr;
}
