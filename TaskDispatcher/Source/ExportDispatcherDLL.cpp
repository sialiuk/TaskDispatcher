#include "..\Include\ExportDispatcherDLL.h"
#include "TaskDispatcher.h"

	void AddSyncTask(mtd::Priority p, const mtd::TaskFunc& f)
	{
		mtd::TaskDispatcher::Instance().GetQueue(p).EnqueueSyncTask(f);
	}

	void AddAsyncTask(mtd::Priority p, const mtd::TaskFunc& f)
	{
		mtd::TaskDispatcher::Instance().GetQueue(p).EnqueueAsyncTask(f);
	}

	void AddSyncBarrier(mtd::Priority p, const mtd::TaskFunc& f)
	{
		mtd::TaskDispatcher::Instance().GetQueue(p).EnqueueSyncBarrier(f);
	}

	void AddAsyncBarrier(mtd::Priority p, const mtd::TaskFunc& f)
	{
		mtd::TaskDispatcher::Instance().GetQueue(p).EnqueueAsyncBarrier(f);
	}

	void AddSyncTaskMainThread(const mtd::TaskFunc& f)
	{
		mtd::TaskDispatcher::Instance().GetMainThreadQueue().EnqueueSyncTask(f);
	}

	void AddAsyncTaskMainThread(const mtd::TaskFunc& f)
	{
		mtd::TaskDispatcher::Instance().GetMainThreadQueue().EnqueueAsyncTask(f);
	}