/*
       File: WeShaderBatch.cpp
	   Author: Tomislav Kukic
	   Date: 24.10.2005.
	   Version: 1.0
	   Platform: Windows
*/




#include <WeShaderBatch.h>
#include <windows.h>
#include <WeBatchItem.h>


WeShaderBatch::~WeShaderBatch()
{
	for(BatchItems.First(); !BatchItems.End(); BatchItems.Next())
	{	
		delete BatchItems.GetItem();
	}

	BatchItems.Clear();


}