// UnitTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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

//BOOST_AUTO_TEST_CASE(TestBarrierAsync)
//{
//	long result1 = 0, result2 = 0;
//	long r1 = 0, r2 = 0, r3 = 0;
//	Mutex mutex;
//	auto queue = TaskDispatcher::Instance().GetQueue(HIGH);
//	queue.EnqueueAsyncTask(
//		[&]()
//		{
//			Lock lock(mutex);
//			r1 += Foo(1, 6);
//		}
//	);
//
//	queue.EnqueueAsyncTask(
//		[&]()
//		{
//			Lock lock(mutex);
//			r2 += Foo(6, 11);
//		}
//	);
//
//	queue.EnqueueAsyncTask(
//		[&]()
//		{
//			Lock lock(mutex);
//			r3 += Foo(11, 15);
//		}
//	);
//
//	queue.EnqueueSyncBarrier(
//		[&]()
//		{
//			Lock lock(mutex);
//			result1 = r1 + r2 + r3;
//		}
//	);
//
//	result2 = Foo(1, 6) + Foo(6, 11) + Foo(11, 15);
//
//	BOOST_CHECK(result1 == result2);
//}


BOOST_AUTO_TEST_CASE(TestTaskCanProcess)
{
	Task task([](){ });
	TaskQueue queue;
	BOOST_CHECK(task.CanProcess(queue));
	queue.Increase();
	BOOST_CHECK(task.CanProcess(queue));
}

BOOST_AUTO_TEST_CASE(TestTaskExecute)
{
	int a = 0;
	Task task([&a](){ a += 5; });
	task.Execute();
	BOOST_CHECK(a == 5);
}

BOOST_AUTO_TEST_CASE(TestBarrierExecute)
{
	int a = 0;
	Barrier taskBarrier([&a](){ a += 5; });
	taskBarrier.Execute();
	BOOST_CHECK(a == 5);
}

BOOST_AUTO_TEST_CASE(TestBarrierCanProcess)
{
	Barrier taskBarrier([](){ });
	TaskQueue queue;
	BOOST_CHECK(taskBarrier.CanProcess(queue));
	queue.Increase();
	BOOST_CHECK(!taskBarrier.CanProcess(queue));
}

BOOST_AUTO_TEST_CASE(TestEnqueueTaskQueue)
{
	TaskQueue queue;
	queue.Enqueue(std::make_shared<Task> ([](){ }));
	BOOST_CHECK(!queue.Empty());
}

BOOST_AUTO_TEST_CASE(TestDequeueTaskQueue)
{
	TaskQueue queue;
	queue.Enqueue(std::make_shared<Task> ([](){ }));
	queue.Dequeue();
	BOOST_CHECK(queue.Empty());
}

BOOST_AUTO_TEST_CASE(TestIncreaseDecreaseCountQueue)
{
	TaskQueue queue;
	queue.Increase();
	BOOST_CHECK(queue.NumberOfRunningTask() == 1);
	queue.Decrease();
	BOOST_CHECK(queue.NumberOfRunningTask() == 0);
}

BOOST_AUTO_TEST_CASE(TestEmptyTaskQueue)
{
	TaskQueue queue;
	BOOST_CHECK(queue.Empty());
}

BOOST_AUTO_TEST_CASE(TestHasTasksToProcessTaskQueue)
{
	TaskQueue queue;
	queue.Enqueue(std::make_shared<Task> ([](){ }));
	BOOST_CHECK(queue.HasTasksToProcess());
	queue.Enqueue(std::make_shared<Barrier> ([](){ }));
	queue.Dequeue();
	BOOST_CHECK(!queue.HasTasksToProcess());
	queue.Decrease();
	BOOST_CHECK(queue.HasTasksToProcess());
}