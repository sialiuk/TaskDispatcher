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
		TLSQueue();
		virtual void TaskComplete();
		virtual bool IsComplete() const;
	private:
		bool m_result;
		mutable Mutex m_mutex;
	};
	

	class MainTLSQueue
		: public BaseTLSQueue
	{
	public:
		MainTLSQueue(MainQueueWindowPtr window);
		virtual void TaskComplete();
		virtual bool IsComplete() const {return true;}
	private:
		MainQueueWindowPtr m_window;
	};

	class UserTLSQueues
		: public boost::noncopyable
	{
	public:
		virtual UserTLSQueues& Then(const TaskFunc&);
		virtual ~UserTLSQueues(){ }
	protected:
		BaseTLSQueuePtr m_currentQueue;
	};

	class HolderTLSQueues
		: public UserTLSQueues
	{
	public:
		virtual HolderTLSQueues& Then(const TaskFunc&);
		virtual void AddQueue();
		BaseTLSQueuePtr GetCurrentQueue() const;
		void ExecuteTasks();
	protected:
		std::vector<BaseTLSQueuePtr> m_queues;
	};
	

	class HolderMainTLSQueues
		: public HolderTLSQueues
	{
	public:
		HolderMainTLSQueues(MainQueueWindowPtr window);
		virtual HolderMainTLSQueues& Then(const TaskFunc&);
		virtual void AddQueue();
	private:
		MainQueueWindowPtr m_window;
	};
	
}