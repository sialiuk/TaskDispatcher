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


	class HolderTLSQueues
	{
	public:
		virtual ~HolderTLSQueues() { }
		virtual void AddQueue();
		virtual HolderTLSQueues& Then(TaskFunc&);
		BaseQueueTLSPtr GetCurrentQueue() const;
		void ExecuteTasks();
	protected:
		BaseQueueTLSPtr m_currentQueue;
		std::vector<BaseQueueTLSPtr> m_queues;
	};
	

	class HolderTLSMainQueues
		: public HolderTLSQueues
	{
	public:
		HolderTLSMainQueues(MainQueueWindowPtr window);
		virtual void AddQueue();
		virtual HolderTLSMainQueues& Then(TaskFunc&);
	private:
		MainQueueWindowPtr m_window;
	};

	
}