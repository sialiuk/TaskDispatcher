#pragma once

#include "Task.h"
#include "Typedefs.h"
#include "BaseQueueTLS.h"
#include "MainQueueWindow.h"
#include <queue>

namespace mtd
{
	class TLSQueue
		: public BaseQueueTLS
	{	
	public:
		TLSQueue();
		virtual void TaskComplete();
		bool IsComplete() const;
	private:
		bool m_result;
		mutable Mutex m_mutex;
	};
	

	class TLSMainQueue
		: public BaseQueueTLS
	{
	public:
		TLSMainQueue(MainQueueWindowPtr window);
		virtual void TaskComplete();
	private:
		MainQueueWindowPtr m_window;
	};

	class UserTLSQueues
	{
	public:
		virtual UserTLSQueues& Then(const TaskFunc&);
		virtual ~UserTLSQueues(){ }
	protected:
		BaseQueueTLSPtr m_currentQueue;
	};

	class HolderTLSQueues
		: public UserTLSQueues
	{
	public:
		virtual HolderTLSQueues& Then(const TaskFunc&);
		virtual void AddQueue();
		BaseQueueTLSPtr GetCurrentQueue() const;
		void ExecuteTasks();
	protected:
		std::vector<BaseQueueTLSPtr> m_queues;
	};
	

	class HolderTLSMainQueues
		: public HolderTLSQueues
	{
	public:
		HolderTLSMainQueues(MainQueueWindowPtr window);
		virtual HolderTLSMainQueues& Then(const TaskFunc&);
		virtual void AddQueue();
	private:
		MainQueueWindowPtr m_window;
	};
	
}