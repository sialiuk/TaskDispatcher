#include "QueueMediator.h"

namespace mtd
{
	QueueMediator::QueueMediator(QueuePtr queue)
		: m_queue(queue)
	{
	}

	QueueMediator::QueueMediator(const QueueMediator& other)
		: m_queue(other.m_queue)
	{
	}

	void QueueMediator::Enqueue(const TaskFunc& func)
	{
		m_queue->Enqueue(std::make_shared<Task>(func));
	}
}