// WindEngine.cpp : Defines the entry point for the DLL application.
//

// Visual Leak Detector
#define VLD_MAX_DATA_DUMP 16
#define VLD_AGGREGATE_DUPLICATES
//#include <vld.h>

// Windows Header Files:
#include <windows.h>
#include "WindEngine.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}


