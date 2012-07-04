#pragma once

#include "Typedefs.h"
#include "TaskQueue.h"
#include "ThreadPool.h"
#include "QueueMediator.h"

namespace mtd
{
	enum Priority
	{
		HIGH,
		NORMAL,
		LOW
	};


	class QueueProcessor : private boost::noncopyable
	{
	public:
		QueueProcessor();
		ThreadRoutine GetThreadRoutine();
		QueueMediator GetQueue(Priority); //const
		QueueMediator CreateQueue();
	private:
		void ProcessQueues();

	private:
		std::map<Priority, QueuePtr> m_queues;
	};


	class TaskDispatcher : public QueueProcessor
	{
	public:
		static TaskDispatcher& Instance();
	private:
		TaskDispatcher();
		~TaskDispatcher();
	private:
		ThreadPool m_pool; 
	};
}
