#include "ThreadPool.h"

namespace mtd
{
	ThreadPool::ThreadPool(ThreadRoutine r, size_t poolSize)
	{
		for(size_t i = 0; i != poolSize; ++i)
		{
			m_pool.push_back(ThreadPtr(new Thread(r)));
		}
	}

	ThreadPool::~ThreadPool()
	{
		std::for_each(m_pool.begin(), m_pool.end(),
			[](ThreadPtr t)
			{
				t->join();
			}
		);
	}
}