// UnitTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace mtd;

namespace
{
	struct QueueListener : IQueueListener
	{
		QueueListener()
			: m_count(0)
		{
		}

		void OnTaskAdded()
		{
			++m_count;
		}

		size_t Count() const
		{
			return m_count;
		}

		~QueueListener()
		{
		}
	private:
		size_t m_count;
	};

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

	template<typename TaskType, typename F>
	TaskPtr MakeTask(F&& f)
	{
		return TaskPtr(new TaskType(std::move(f)), [](Task* p){delete p;});
	}
}

BOOST_AUTO_TEST_CASE(TestInsertedTask)
{
	int tempResult1 = 0;
	int tempResult2 = 0;
	int tempResult3 = 0;

	Mutex mutex;
	auto queue = TaskDispatcher::Instance().GetQueue(NORMAL);

	queue.EnqueueSyncTask(
		[&]()
	{
		auto temp  = Summation(1, 20);
		{
			Lock lock(mutex);
			tempResult3 += temp;
		}
		
		Mutex& tempMutex = mutex;
		int& temp1 = tempResult1;
		int& temp3 = tempResult3;
		queue.EnqueueAsyncTask(
			[&]()
		{
			{
				Lock lock(tempMutex);
				temp1 = temp3;
			}
		});
	});

	queue.EnqueueAsyncTask(
		[&]()
	{
		int temp = Summation(1, 20);
		{
			Lock lock(mutex);
			tempResult2 += temp;
		}
	});

	queue.EnqueueSyncBarrier(
		[&]()
	{
		
	});

	BOOST_CHECK(tempResult1 == tempResult2);
}

BOOST_AUTO_TEST_CASE(TestAsyncSyncBarrier)
{
	int result1 = 0;
	int result2 = 0;
	int tempResult1 = 0;
	int tempResult2 = 0;

	Mutex mutex;
	auto queue = TaskDispatcher::Instance().GetQueue(NORMAL);
	queue.EnqueueAsyncTask(
		[&]()
	{
		int temp = Summation(1, 10);

		{
			Lock lock(mutex);
			tempResult1 += temp;
		}
	});
	
	queue.EnqueueAsyncTask(
		[&]()
	{
		int temp = Summation(10, 20);

		{
			Lock lock(mutex);
			tempResult2 += temp;
		}
	});

	queue.EnqueueAsyncBarrier(
		[&]()
	{
		Lock lock(mutex);
		result1 = tempResult1 + tempResult2;
	});

	queue.EnqueueAsyncTask(
		[&]()
	{
		int temp = Summation(10, 20);

		{
			Lock lock(mutex);
			tempResult1 = temp;
		}
	});
	
	queue.EnqueueAsyncTask(
		[&]()
	{
		int temp = Summation(1, 10);

		{
			Lock lock(mutex);
			tempResult2 = temp;
		}
	});

	queue.EnqueueSyncBarrier(
		[&]()
	{
		Lock lock(mutex);
		result2 = tempResult1 + tempResult2;
	});

	BOOST_CHECK(result2 == result1);
}

BOOST_AUTO_TEST_CASE(TestGenerationTaskException)
{
	auto queue = TaskDispatcher::Instance().GetQueue(NORMAL);

	queue.EnqueueAsyncTask(
		[&]()
	{
		throw std::runtime_error("ErrorTask");
	});

	queue.EnqueueSyncBarrier(
		[&]()
	{
		
	});

}

BOOST_AUTO_TEST_CASE(TestBarrierSync)
{
	long result1 = 0, result2 = 0;
	long tempResult1 = 0, tempResult2 = 0, tempResult3 = 0; 
	Mutex mutex;
	auto queue = TaskDispatcher::Instance().GetQueue(HIGH);
	queue.EnqueueAsyncTask(
		[&]()
		{
			auto temp = Foo(1, 6);
			{
				Lock lock(mutex);
				tempResult1 += temp;
			}
		}
	);

	queue.EnqueueAsyncTask(
		[&]()
		{
			auto temp = Foo(6, 11);
			{
				Lock lock(mutex);
				tempResult2 += temp;
			}
		}
	);

	queue.EnqueueAsyncTask(
		[&]()
		{
			auto temp = Foo(11, 15);
			{
				Lock lock(mutex);
				tempResult3 += temp;
			}
		}
	);

	queue.EnqueueSyncBarrier(
		[&]()
		{
			Lock lock(mutex);
			result1 = tempResult1 + tempResult2 + tempResult3;
		}
	);

	result2 = Foo(1, 6) + Foo(6, 11) + Foo(11, 15);

	BOOST_CHECK(result1 == result2);
}


BOOST_AUTO_TEST_CASE(TestTaskExecute)
{
	int a = 0;
	Task task([&a](){ a += 5; });
	task.Execute();
	BOOST_CHECK(a == 5);
}

BOOST_AUTO_TEST_CASE(TestTaskCanProcess)
{

	QueueListener listener;
	TaskQueue queue(listener);
	Task task([] () { });
	BOOST_CHECK(task.CanProcess(queue));
}

BOOST_AUTO_TEST_CASE(TestBarrierCanProcess)
{
	QueueListener listener;
	TaskQueue queue(listener);
	Barrier barrier([] () { });
	BOOST_CHECK(barrier.CanProcess(queue));
	queue.Increase();
	BOOST_CHECK(!barrier.CanProcess(queue));
}

BOOST_AUTO_TEST_CASE(TestBarrierExecute)
{
	int a = 0;
	Barrier taskBarrier([&a](){ a += 5; });
	taskBarrier.Execute();
	BOOST_CHECK(a == 5);
}

BOOST_AUTO_TEST_CASE(TestQueueIncreaseDecrease)
{
	QueueListener listener;
	TaskQueue queue(listener);
	queue.Increase();
	queue.Increase();
	BOOST_CHECK(queue.NumberOfRunningTask() == 2);
	queue.Decrease();
	queue.Decrease();
	BOOST_CHECK(queue.NumberOfRunningTask() == 0);
}

BOOST_AUTO_TEST_CASE(TestQueueHasTasksToProcess)
{
	QueueListener listener;
	TaskQueue queue(listener);
	queue.EnqueueAsync(MakeTask<Barrier>([](){ }));
	BOOST_CHECK(queue.HasTasksToProcess());
	queue.Increase();
	BOOST_CHECK(!queue.HasTasksToProcess());
	queue.Dequeue();
	queue.EnqueueAsync(MakeTask<Task>([](){ }));
	BOOST_CHECK(queue.HasTasksToProcess());
}

BOOST_AUTO_TEST_CASE(TestQueueOnEmptiness)
{
	QueueListener listener;
	TaskQueue queue(listener);
	BOOST_CHECK(queue.Empty());
	queue.EnqueueAsync(MakeTask<Task>([](){ }));
	BOOST_CHECK(!queue.Empty());
}

BOOST_AUTO_TEST_CASE(TestQueueEnqueueAsync)
{
	QueueListener listener;
	TaskQueue queue(listener);

	queue.EnqueueAsync(MakeTask<Task>([](){ }));
	queue.EnqueueAsync(MakeTask<Task>([](){ }));
	queue.EnqueueAsync(MakeTask<Task>([](){ }));
	BOOST_CHECK(!queue.Empty() && listener.Count() == 3);
}

BOOST_AUTO_TEST_CASE(TestQueueDequeue)
{
	QueueListener listener;
	TaskQueue queue(listener);
	queue.EnqueueAsync(MakeTask<Task>([](){ }));
	queue.EnqueueAsync(MakeTask<Task>([](){ }));
	queue.EnqueueAsync(MakeTask<Task>([](){ }));
	queue.Dequeue();
	queue.Dequeue();
	queue.Dequeue();
	BOOST_CHECK(queue.Empty() && queue.NumberOfRunningTask() == 3);
}

