#pragma once
#include "..\Source\Function.h"
#include "..\Source\Priority.h"

void AddSyncTask(mtd::Priority, const mtd::TaskFunc&);
void AddAsyncTask(mtd::Priority, const mtd::TaskFunc&);
void AddSyncBarrier(mtd::Priority, const mtd::TaskFunc&);
void AddAsyncBarrier(mtd::Priority, const mtd::TaskFunc&);
void AddSyncTaskMainThread(const mtd::TaskFunc&);
void AddAsyncTaskMainThread(const mtd::TaskFunc&);