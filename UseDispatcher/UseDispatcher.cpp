// UseDispatcher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Dispatcher.h>

#include <iostream>

using namespace mtd;

long Foo(int left, int right)
{
	long result = 1;
	while(left != right)
	{
		result *= left++;
	}
	return result;
}


int _tmain(int argc, _TCHAR* argv[])
{
	Mutex mutex;
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
			auto temp = Foo(1, 6);
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
	std::cout << result2 << std::endl;
	return 0;
}

