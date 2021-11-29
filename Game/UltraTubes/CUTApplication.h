/*
       File: CApplication.h
	   Author: Tomislav Kukic
	   Date: 11.01.2006.
	   Version: 1.0
*/




#ifndef _CUTAPPLICATION_H
#define _CUTAPPLICATION_H


#include <Windows.h>
#include <CApplication.h>


class CUTApplication : public CApplication
{
public:	
	static CUTApplication *Instance;
	bool Run();
};




#endif