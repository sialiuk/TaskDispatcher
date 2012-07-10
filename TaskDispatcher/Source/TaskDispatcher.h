#pragma once

#include "Typedefs.h"
#include "TaskQueue.h"
#include "ThreadPool.h"
#include "QueueAdapter.h"

namespace mtd
{
	enum Priority
	{
		HIGH,
		NORMAL,
		LOW
	};

	class QueueProcessor
		: public IQueueListener
		, private boost::noncopyable
	{
		friend class QueueAdapter;
	public:
		QueueProcessor();
		ThreadRoutine GetThreadRoutine();
		QueueAdapter GetQueue(Priority); //const
		QueueAdapter CreateQueue();
		virtual void OnTaskAdded();
		
	protected:
		void Shutdown();

	private:
		QueuePtr GetNonEmptyQueue();
		TaskPtr GetTask();
		void WaitForChanges();
		void ProcessQueues();
		bool ShouldShutdown() const;

	private:
		mutable Mutex m_mutexForGetTask;
		mutable Mutex m_mutexForNotify;
		bool m_shouldShutdown;
		ConditionVariable m_cond;
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
