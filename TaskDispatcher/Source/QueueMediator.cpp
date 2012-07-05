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

	void QueueMediator::Enqueue(const TaskFunc& func)
	{
		auto q = m_queue;
		TaskPtr p(new Task(func), [q](Task* p){delete p;q->Decrease();});
		//m_queue->Enqueue(std::make_shared<Task>(func));
		m_queue->Enqueue(p);
		m_processor.NotifyAboutChanges();
	}
}