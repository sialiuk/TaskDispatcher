#pragma once

#include "Typedefs.h"

namespace mtd
{
	typedef boost::function<void ()> TaskFunc;

	class Task: private boost::noncopyable
	{
	public:
		Task(const TaskFunc&);
		void Execute();
	
	private:
		TaskFunc m_func;
	};

	typedef std::shared_ptr<Task> TaskPtr;
}
