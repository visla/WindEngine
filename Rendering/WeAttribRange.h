/*
     File: WeAttributRange.h
	 Author: Tomislav Kukic
	 Date: 14.09.2005.
	 Version: 1.0
*/


#ifndef _WEATTRIBRANGE
#define _WEATTRIBRANGE



typedef struct _WeAttribRange {
    DWORD AttribId;
    DWORD FaceStart;
    DWORD FaceCount;
    DWORD VertexStart;
    DWORD VertexCount;
} WeAttribRange;


#endif
