#pragma once

#include "Task.h"
#include "Typedefs.h"
#include <queue>

namespace mtd
{
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
		void EnqueueSync(TaskPtr&&);
		void EnqueueAsync(TaskPtr&&);
		void Enqueue(TaskPtr&&);
		TaskPtr Dequeue();
		bool Empty() const;
		bool HasTasksToProcess() const;
		void Increase();
		void Decrease();
		size_t NumberOfRunningTask() const;
		void NotifySyncFinished();
		void WaitForSyncFinished();

	private:
		IQueueListener& m_listener;
		std::queue<TaskPtr> m_tasks;		
		mutable Mutex m_mutex;
		size_t m_count;
		ConditionVariable m_syncFinishedCondition;
	};
	typedef std::shared_ptr<TaskQueue> QueuePtr;
}