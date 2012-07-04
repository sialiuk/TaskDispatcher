#pragma once

#include "../Source/TaskQueue.h"

namespace mtd
{
	class QueueMediator 
	{
		friend class QueueProcessor;
		
	public:
		void Enqueue(const TaskFunc& func);
		QueueMediator(const QueueMediator&);
	private:
		QueueMediator(QueuePtr);

	private:
		QueuePtr m_queue; 
	};
}