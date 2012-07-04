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


long result;
void Func()
{
	result = Foo(1, 100);
}

int _tmain(int argc, _TCHAR* argv[])
{

	boost::function<void ()>F(Func);;
	boost::noncopyable;
	auto queue = TaskDispatcher::Instance().GetQueue(NORMAL);
	queue.Enqueue(
		[]()
		{
			result = Foo(0, 1000);
		}
	);

	Task task(F);
	std::cin.get();

	return 0;
}

