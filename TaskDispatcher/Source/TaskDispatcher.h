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

	class QueueProcessor
		: public IQueueListener
		, private boost::noncopyable
	{
		friend class QueueMediator;
	public:
		QueueProcessor();
		ThreadRoutine GetThreadRoutine();
		QueueMediator GetQueue(Priority); //const
		QueueMediator CreateQueue();
		virtual void OnTaskAdded();

	private:
		QueuePtr GetNonEmptyQueue();
		TaskPtr GetTask();
		void WaitForChanges();
		void ProcessQueues();
		bool ShouldShutdown() const;

	protected:
		void Shutdown();

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
