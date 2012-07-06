#include "QueueMediator.h"
#include "TaskDispatcher.h"

namespace mtd
{
	QueueMediator::QueueMediator(QueuePtr queue, QueueProcessor& processor)
		: m_queue(queue)
		, m_processor(processor)
	{
	}

	QueueMediator::QueueMediator(const QueueMediator& other)
		: m_queue(other.m_queue)
		, m_processor(other.m_processor)
	{
	}

	void QueueMediator::CreateAndEnqueueTask(Task* task)
	{
		auto q = m_queue;
		TaskPtr p(task, //TODO: restore make_shared
		[q](Task* p)
		{
			delete p;
			q->Decrease();
		});
		m_queue->Enqueue(p);
		m_processor.NotifyAboutChanges();
	}

	void QueueMediator::Enqueue(const TaskFunc& func)
	{
		Task* task = new Task(func);
		CreateAndEnqueueTask(task);
	}

	void QueueMediator::EnqueueBarrier(const TaskFunc& func)
	{
		Task* task = new Barrier(func);
		CreateAndEnqueueTask(task);
	}
}