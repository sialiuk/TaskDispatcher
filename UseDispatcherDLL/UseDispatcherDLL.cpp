// UseDispatcherDLL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "..\TaskDispatcher\Include\ExportDispatcherDLL.h"

int _tmain(int argc, _TCHAR* argv[])
{
	AddSyncTask(mtd::LOW,
		[]()
		{
			std::cout << "AddSyncTask\n";
		});

	AddAsyncTask(mtd::HIGH,
		[]()
		{
			std::cout << "AddAsyncTask 1\n";
		});
	
	AddAsyncTask(mtd::LOW,
		[]()
		{
			std::cout << "AddAsyncTask 3\n";
		});

	AddAsyncTask(mtd::HIGH,
		[]()
		{
			std::cout << "AddAsyncTask 2\n";
		});
	
	AddSyncTask(mtd::LOW,
		[]()
		{
			std::cout << "AddSyncTask\n";
		});


	return 0;
}

