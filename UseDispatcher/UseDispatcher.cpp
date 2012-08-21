// UseDispatcher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Dispatcher.h>
#include <Windows.h>

#include <iostream>

using namespace mtd;

namespace
{
	long Foo(int left, int right)
	{
		long result = 1;
		while(left != right)
		{
			result *= left++;
		}
		return result;
	}

	int Summation(int left, int right)
	{
		int result = 0;
		while(left != right)
		{
			result += left++;	
		}
		return result;
	}

	struct Check
	{
		Check() { ++m_count; }
		static size_t m_count;
	};
	size_t Check::m_count = 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//boost::thread_group pool;
	//for(int i = 0; i != 200; ++i)
	//{
	//	pool.create_thread([]() { Singleton<Check>::Instance(); });
	//}


	
	auto queue = TaskDispatcher::Instance().GetQueue(HIGH);
	queue.EnqueueSyncTask([&]()
		{
			std::cout << "Enqueue Async Task: " << GetCurrentThreadId() << std::endl;
			queue.EnqueueAsyncTask([]()
				{
					std::cout << "Async Task execute: " << GetCurrentThreadId() << std::endl;
			}).Then([]()
				{
					std::cout << "Task execute: " << GetCurrentThreadId() << std::endl;
				});
		}
	);
	/*std::cout << "Enqueue sync task main thread: "<< GetCurrentThreadId() << std::endl;
	queue.EnqueueSyncTask(
		[&]()
		{
			std::cout << "Task execute: " << GetCurrentThreadId() << std::endl;
			std::cout << "Enqueue sync task thread: " << GetCurrentThreadId() << std::endl;
			queue.EnqueueSyncTask(
			[]()
			{
				std::cout << "Task execute: " << GetCurrentThreadId() << std::endl;
				std::cout << "Enqueue sync task thread: " << GetCurrentThreadId() << std::endl;
				auto queue = TaskDispatcher::Instance().GetQueue(HIGH);
				queue.EnqueueSyncTask(
				[]()
				{
					std::cout << "Task execute: " << GetCurrentThreadId() << std::endl;

				});
				std::cout << "Wakes thread: " << GetCurrentThreadId() << std::endl;
			});
			std::cout << "Wakes thread: " << GetCurrentThreadId() << std::endl;
		});
	std::cout << "Wakes thread: " << GetCurrentThreadId() << std::endl;
*/


	/*Mutex mutex;
	long result1 = 0, result2 = 0;
	long r1 = 0, r2 = 0;
	auto queue = TaskDispatcher::Instance().GetQueue(HIGH);
	queue.EnqueueAsyncTask(
		[&]()
		{
			auto temp = Foo(1, 6);
			{
				Lock lock(mutex);
				r1 = temp;
			}
		}
	);

	queue.EnqueueAsyncTask(
		[&]()
		{
			auto temp = Foo(6, 11);
			{
				Lock lock(mutex);
				r2 = temp;
			}
		}
	);

	queue.EnqueueSyncBarrier(
		[&]()
		{
			Lock lock(mutex);
			result1 = r1 + r2;
		}
	);

	result2 = Foo(1, 6) + Foo(6, 11);

	std::cout << result1 << std::endl;
	std::cout << result2 << std::endl;*/
	return 0;
}

