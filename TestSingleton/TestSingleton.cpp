// TestSingleton.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinBase.h>
using namespace mtd;

namespace
{
	struct Count
	{
		Count() 
		{
			++m_count; 
		}

		~Count() 
		{

		}
		static size_t m_count;
	};

	size_t Count::m_count = 0;

typedef SingletonHolder<Count, CreateStaticVariable, MultiThreadedMutexLock> SingletonStaticMutex;
typedef SingletonHolder<Count, CreateWithNew, MultiThreadedMutexLock> SingletonNewMutex;
typedef SingletonHolder<Count, CreateStaticVariable, MultiThreadedLockFree> SingletonStaticLockFree;
typedef SingletonHolder<Count, CreateWithNew, MultiThreadedLockFree> SingletonNewLockFree;
} 

BOOST_AUTO_TEST_CASE(TestSingletonHolderDefault)
{
	typedef SingletonHolder<int> Singleton;
	int& result1 = Singleton::Instance();
	int& result2 = Singleton::Instance();
	BOOST_CHECK(&result1 == &result2);
}

BOOST_AUTO_TEST_CASE(TestSingletonSingleThreadingUseStaticVariable)
{
	typedef SingletonHolder<int, CreateStaticVariable> Singleton;
	int& result1 = Singleton::Instance();
	int& result2 = Singleton::Instance();
	BOOST_CHECK(&result1 == &result2);
}

BOOST_AUTO_TEST_CASE(TestSingletonMultiThreadingUseStaticVariable)
{
	boost::thread_group pool;
	for(int i = 0; i != 20; ++i)
	{
		pool.create_thread([](){ SingletonStaticMutex::Instance(); });
	}
	pool.join_all();

	BOOST_CHECK(Count::m_count == 1);
	Count::m_count = 0;
}

BOOST_AUTO_TEST_CASE(TestSingletonMultiThreadingUseNew)
{
	boost::thread_group pool;
	for(int i = 0; i != 20; ++i)
	{
		pool.create_thread([](){ SingletonNewMutex::Instance(); });
	}
	pool.join_all();

	BOOST_CHECK(Count::m_count == 1);
	Count::m_count = 0;
}

BOOST_AUTO_TEST_CASE(TestSingletonMultiThreadingLockFreeUseStaticVariable)
{
	boost::thread_group pool;
	for(int i = 0; i != 20; ++i)
	{
		pool.create_thread([](){ SingletonStaticLockFree::Instance(); });
	}
	pool.join_all();

	BOOST_CHECK(Count::m_count == 0);
}

BOOST_AUTO_TEST_CASE(TestSingletonMultiThreadingLockFreeUseNew)
{
	boost::thread_group pool;
	for(int i = 0; i != 20; ++i)
	{
		pool.create_thread([](){ SingletonNewLockFree::Instance(); });
	}
	pool.join_all();

	BOOST_CHECK(Count::m_count == 1);
	Count::m_count = 0;
}

