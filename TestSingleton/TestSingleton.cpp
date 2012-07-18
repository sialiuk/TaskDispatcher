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
} 

BOOST_AUTO_TEST_CASE(TestSingleton)
{
	boost::thread_group pool;
	for(int i = 0; i != 20; ++i)
	{
		pool.create_thread([](){ Singleton<Count>::Instance(); });
	}
	pool.join_all();

	BOOST_CHECK(Count::m_count == 1);
	Count::m_count = 0;
}

BOOST_AUTO_TEST_CASE(TestSingletonLock)
{
	boost::thread_group pool;
	for(int i = 0; i != 20; ++i)
	{
		pool.create_thread([](){ SingletonLock<Count>::Instance(); });
	}
	pool.join_all();

	BOOST_CHECK(Count::m_count == 1);
	Count::m_count = 0;
}

BOOST_AUTO_TEST_CASE(TestSingletonHandler)
{
	boost::thread_group pool;
	for(int i = 0; i != 20; ++i)
	{
		pool.create_thread([](){ SingletonHandler<Count>::Instance(); });
	}
	pool.join_all();

	BOOST_CHECK(Count::m_count == 1);
	Count::m_count = 0;
}
