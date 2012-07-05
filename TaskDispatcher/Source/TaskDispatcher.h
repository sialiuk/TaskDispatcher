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
		friend class QueueMediator;
	public:
		QueueProcessor();
		ThreadRoutine GetThreadRoutine();
		QueueMediator GetQueue(Priority); //const
		QueueMediator CreateQueue();
		
	private:
		QueuePtr GetNonEmptyQueue();
		TaskPtr GetTask();
		void WaitForChanges();
		void NotifyAboutChanges();
		void ProcessQueues();
		bool ShouldShutdown() const;

	protected:
		void Shutdown();

	private:
		mutable Mutex m_mutex;
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
