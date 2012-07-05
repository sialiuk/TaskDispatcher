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
		for(int i = 0; i < 1000000; ++i)
		{
			result *= left;
		}
		result *= left++;
	}
	return result;
}


int _tmain(int argc, _TCHAR* argv[])
{
	long result = 0;
	Mutex mutex;
	auto queue = TaskDispatcher::Instance().GetQueue(NORMAL);
	queue.Enqueue(
		[&]()
		{
			Lock lock(mutex);
			result += Foo(1, 6);
		}
	);

	queue.Enqueue(
		[&]()
		{
			Lock lock(mutex);
			result += Foo(6, 12);
		}
	);

	queue.Enqueue(
		[&]()
		{
			Lock lock(mutex);
			result += Foo(12, 120);
		}
	);


	/*queue.EnqueueBarrier(
		[&]()
		{
			std::cout << result << std::endl;
		}	
	);*/

	std::cin.get();

	return 0;
}

