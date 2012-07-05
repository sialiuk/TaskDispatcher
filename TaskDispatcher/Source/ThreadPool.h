#pragma once

#include <vector>
#include "Typedefs.h"


namespace mtd
{
	typedef boost::function<void ()> ThreadRoutine;
	class ThreadPool : private boost::noncopyable
	{
		typedef std::shared_ptr<Thread> ThreadPtr;
		typedef std::vector<ThreadPtr> Threads;

	public:
		ThreadPool(ThreadRoutine r, size_t poolSize = 4);
		~ThreadPool();
	private:
		Threads m_pool;
	};
}

