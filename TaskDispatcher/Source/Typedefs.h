#pragma once

#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <memory>
#include "Priority.h"
#include "Function.h"

namespace mtd
{
	typedef boost::thread Thread;
	typedef boost::mutex Mutex;
	typedef boost::unique_lock<Mutex> Lock;
	typedef boost::condition_variable ConditionVariable;
}
