// UnitTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace mtd;


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