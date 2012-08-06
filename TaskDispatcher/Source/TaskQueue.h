#pragma once

#include "Task.h"
#include "Typedefs.h"
#include <queue>

namespace mtd
{

	class SynchronizationForTask
		: private boost::noncopyable
	{ 
	public:
		SynchronizationForTask();
		void WaitForSyncFinished();
		void NotifySyncFinished();
	private:
		ConditionVariable m_cond;
		Mutex m_mutex;
		bool m_flagOnExecute;
	};

	class IQueueListener
	{
	public:
		virtual void OnTaskAdded() = 0;
		virtual ~IQueueListener() {}
	};

	class TaskQueue : private boost::noncopyable
	{
	public:
		TaskQueue(IQueueListener&);
		~TaskQueue();
		void EnqueueSync(TaskPtr&&, SynchronizationForTask&);
		void EnqueueAsync(TaskPtr&&);
		void Enqueue(TaskPtr&&);
		TaskPtr Dequeue();
		bool Empty() const;
		bool HasTasksToProcess() const;
		void Increase();
		void Decrease();
		size_t NumberOfRunningTask() const;
	private:
		IQueueListener& m_listener;
		std::queue<TaskPtr> m_tasks;		
		mutable Mutex m_mutex;
		size_t m_count;
	};
	typedef std::shared_ptr<TaskQueue> QueuePtr;
}