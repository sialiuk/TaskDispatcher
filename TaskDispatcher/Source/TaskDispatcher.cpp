#include "TaskDispatcher.h"

namespace mtd
{
	QueueProcessor::QueueProcessor()
	{
		m_queues[HIGH] = QueuePtr(new TaskQueue());
		m_queues[NORMAL] = QueuePtr(new TaskQueue());
		m_queues[LOW] = QueuePtr(new TaskQueue());
	}

	ThreadRoutine QueueProcessor::GetThreadRoutine()
	{
		return [this](){ProcessQueues();};
	}
	
	void QueueProcessor::ProcessQueues()
	{
		//while(!Terminated())
		//{
		//	if (HasTaskAvaliable())
		//	{
		//		auto task = GetTask();
		//		task();
		//	}
		//	else
		//	{
		//		//Sleep(100);
		//		WaitForTasks();
		//	}
		//}
	}

	QueueMediator QueueProcessor::GetQueue(Priority p)
	{
		auto it = m_queues.find(p);
		assert(it != m_queues.end());
		return QueueMediator(it->second);
	}

	TaskDispatcher::TaskDispatcher()
		: m_pool(GetThreadRoutine())
	{
	
	}

	TaskDispatcher::~TaskDispatcher()
	{
	}

	TaskDispatcher& TaskDispatcher::Instance()
	{
		static TaskDispatcher td;
		return td;
	}

}