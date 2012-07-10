#pragma once

#include "Typedefs.h"

namespace mtd
{
	typedef boost::function<void ()> TaskFunc;
	
	class TaskQueue;

	class Task : private boost::noncopyable
	{
	public:
		Task(const TaskFunc&);
		virtual ~Task(){};
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
	
	typedef boost::function<void (Task* p)> TaskDeleter;
	typedef std::unique_ptr<Task, TaskDeleter> TaskPtr;
}
