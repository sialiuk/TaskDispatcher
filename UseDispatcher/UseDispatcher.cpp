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


	double Calculation(long i)
	{
		double sum = 0;
		while(i >= 0)
		{
			sum += sqrt((double)rand()) / log10((double)rand());
			--i;
		}
		return sum;
	}

	class TimerCounter
	{
	public:
		void Start()
		{
			QueryPerformanceCounter(&m_start_time);
		}

		void Stop()
		{
		    QueryPerformanceCounter(&m_end_time);
		}

		LONGLONG Interval()
		{
			return m_end_time.QuadPart - m_start_time.QuadPart;
		}
	private:	
		LARGE_INTEGER  m_start_time;
		LARGE_INTEGER  m_end_time;
	};
	TimerCounter counter;
}

int _tmain(int argc, _TCHAR* argv[])
{
	auto queue = TaskDispatcher::Instance().GetQueue(HIGH);
	counter.Start();
	queue.EnqueueSyncTask([&]()
		{

			std::cout << "Enqueue async task thread: "<< GetCurrentThreadId() << std::endl;
			queue.EnqueueAsyncTask([]()
				{
					std::cout << Calculation(10000) << std::endl;
					std::cout << "Execute async task thread: "<< GetCurrentThreadId() << std::endl;
				}
			).Then([&]()
				{
					std::cout << Calculation(10000) << std::endl;
					std::cout << "Execute Then task thread: "<< GetCurrentThreadId() << std::endl;
					counter.Stop();
					std::cout << "*****Time Execute use Then*****: " << counter.Interval() << std::endl;
				});
		});

	Sleep(2000);

	counter.Start();
	queue.EnqueueSyncTask([&]()
		{
			std::cout << "Enqueue Sync task thread: "<< GetCurrentThreadId() << std::endl;
			queue.EnqueueSyncTask([]()
				{
					std::cout << Calculation(10000) << std::endl;
					std::cout << "Execute Sync task thread: "<< GetCurrentThreadId() << std::endl;
				});
			std::cout << Calculation(10000) << std::endl;
			std::cout << "Execute Then task thread: "<< GetCurrentThreadId() << std::endl;
			counter.Stop();
		});

	std::cout << "*****Time Execute not use Then*****: " << counter.Interval() << std::endl;
	system("pause");

	/*counter.Start();
	queue.EnqueueSyncTask([&]()
		{
			queue.EnqueueSyncTask([]()
				{
					std::cout << Calculation(10000) << std::endl;
				});
			std::cout << Calculation(10000) << std::endl;
		});
	counter.Stop();
	std::cout << "Time Execute not use Then: " << counter.Interval() << std::endl;*/

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

