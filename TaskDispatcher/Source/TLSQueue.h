#pragma once

#include "Task.h"
#include "Typedefs.h"
#include "BaseTLSQueue.h"
#include "MainQueueWindow.h"
#include <queue>

namespace mtd
{
	class TLSQueue
		: public BaseTLSQueue
	{	
	public:
		TLSQueue(IQueueListener& listener);
		virtual void TaskComplete();
		virtual bool IsComplete() const;
	private:
		bool m_result;
		mutable Mutex m_mutex;
		IQueueListener& m_listener;
	};
	
	class MainTLSQueue
		: public BaseTLSQueue
	{
	public:
		MainTLSQueue(MainQueueWindowPtr window);
		virtual void TaskComplete();
	private:
		MainQueueWindowPtr m_window;
	};


	class UserTLSQueues
		: private boost::noncopyable
	{
	public:
		virtual UserTLSQueues& Then(const TaskFunc&) = 0;
	};

	class BaseTLSQueues
		: public UserTLSQueues
	{
	public:
		virtual ~BaseTLSQueues(){ }
		virtual void AddQueue() = 0;
		void ExecuteTasks();
		BaseTLSQueuePtr GetCurrentQueue() const;
	protected:
		BaseTLSQueuePtr m_currentQueue;
		std::vector<BaseTLSQueuePtr> m_queues;
	};

	class HolderTLSQueues
		: public BaseTLSQueues
	{
	public:
		HolderTLSQueues(IQueueListener& listener);
		virtual HolderTLSQueues& Then(const TaskFunc&);
		virtual void AddQueue();
	private:
		IQueueListener& m_listener;
	};
	
	class HolderMainTLSQueues
		: public BaseTLSQueues
	{
	public:
		HolderMainTLSQueues(MainQueueWindowPtr window);
		virtual HolderMainTLSQueues& Then(const TaskFunc&);
		virtual void AddQueue();
	private:
		MainQueueWindowPtr m_window;
	};
}