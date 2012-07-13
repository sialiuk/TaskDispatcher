#include "MainQueueAdapter.h"

namespace mtd
{
	MainQueueAdapter::MainQueueAdapter(QueuePtr queue, MainQueueWindowPtr window)
		: m_queue(queue)
		, m_window(window)
	{
	}

	void MainQueueAdapter::EnqueueAsyncTask(const TaskFunc& func)
	{
		TaskPtr task(new Task(func));
		m_queue->Enqueue(std::move(task));
		m_window->PostMainMessage();
	}

	void MainQueueAdapter::EnqueueSyncTask(const TaskFunc& func)
	{
		TaskPtr task(new Task(func));
		m_queue->Enqueue(std::move(task));
		m_window->SendMainMessage();
	}
}