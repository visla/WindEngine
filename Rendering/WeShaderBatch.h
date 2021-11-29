/*
       File: WeShaderBatch.h
	   Author: Tomislav Kukic
	   Date: 24.10.2005.
	   Version: 1.0
	   Platform: Windows
*/




#ifndef _WESHADERBATCH
#define _WESHADERBATCH


#include <SmartList.h>


class WeBatchItem;
class WeShader;



class WeShaderBatch
{
public:
	~WeShaderBatch();

	WeShader* Shader;
	CSmartList<WeBatchItem*> BatchItems;	
};



#endif