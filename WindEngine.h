// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WINDENGINE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WINDENGINE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//#define WINDENGINE_API 


//#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <malloc.h>
//#include <stdlib.h>
//#include <crtdbg.h>
//#endif


#ifndef __SAFE_DELETE__
#define __SAFE_DELETE__
#define SAFE_DELETE(a) if(a) { delete a; a = NULL; }
#endif


#ifndef __SAFE_RELEASE__
#define __SAFE_RELEASE__
#define SAFE_RELEASE(a) if(a) { a->Release(); a = NULL; }
#endif


#ifdef WINDENGINE_EXPORTS
#define WINDENGINE_API __declspec(dllexport)
#else
#define WINDENGINE_API __declspec(dllimport)
#endif
