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
	long result;
	auto queue = TaskDispatcher::Instance().GetQueue(NORMAL);
	queue.Enqueue(
		[&]()
		{
			result = Foo(1, 6);
		}
	);

	std::cout << result << std::endl;

	return 0;
}

