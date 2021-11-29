/*
       File: WeLightBatch.h
	   Author: Vislavski Nebojsa 
	   Date: 14.7.2006.
	   Version: 1.0
	   Platform: Windows
*/




#ifndef _WeLightBatch
#define _WeLightBatch


#include <SmartList.h>


class WeBatchItem;
class WeLight;



class WeLightBatch
{
public:
	~WeLightBatch();

    WeLight *Light;
	CSmartList<WeBatchItem*> BatchItems;	
};



#endif