#pragma once

#include "Typedefs.h"


namespace mtd
{	
	class TaskQueue;

	class Task : private boost::noncopyable
	{
	public:
		Task(const TaskFunc&, const FuncExcept& callback = CallbackCaseException);
		virtual ~Task(){ }
		void Execute();
		bool CanProcess(const TaskQueue& queue) const;
	private:
		virtual bool CanProcessImpl(const TaskQueue& queue) const;
	protected:
		static void CallbackCaseException(std::exception_ptr);
	private:
		TaskFunc m_func;
		FuncExcept m_callbackEx;
	};

	class Barrier 
		: public Task
	{
	public:
		Barrier(const TaskFunc&, const FuncExcept& callback = CallbackCaseException);
	private:
		virtual bool CanProcessImpl(const TaskQueue& queue) const;
	};
	
	typedef boost::function<void (Task* p)> TaskDeleter;
	typedef std::unique_ptr<Task, TaskDeleter> TaskPtr;
}
