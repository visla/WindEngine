/*
    WeProfiler.h

	Autor: Nebojsa Vislavski
	Date: 1.11.2005.
	Version: 1.0


    This profiler is used to measure different things in rendering engine

*/

#ifndef _WEPROFILER_H
#define _WEPROFILER_H

#include <WindEngine.h>
#include <CString.h>


#define MAX_BLOCKS 1000


class WINDENGINE_API WeProfiler
{
private:
	static WeProfiler *_instance;

public:
	static WeProfiler *Instance();

	WeProfiler();

	int ObjectsInWorld;  // how many objects are in world
	int ObjectsRendered; // how many objects are to be rendered

	int RenderTreeNodesVisible;	
	int tmp;

	int BatchItemsQueried; 
	int BatchItemsRendered; 
	int BatchItemsSkipped;
	int BatchItemsQueued;

	int AlphaBatchItemsRendered;
	int AlphaBatchItemsQueried;
	int AlphaBatchItemsSkipped;
	int AlphaBatchItemsQueued;

	int ForegroundBatchItemsRendered;
	int ForegroundBatchItemsQueried;
	int ForegroundBatchItemsSkipped;
	int ForegroundBatchItemsQueued;

	int PolygonsRendered;
	int ZonesVisible;
	Strings Zones;

	int ParticlesCount; // how many particles are rendered
	int ReflectiveObjectsDrawn; 
	int ShaderBegins;
	Strings ShadersActive;

	int ClientSendBytesPerSec;
	int ClientRecvBytesPerSec;
	int ClientSendBytesCounter;
	int ClientRecvBytesCounter;
	int ServerSendBytesPerSec;
	int ServerRecvBytesPerSec;
	int ServerSendBytesCounter;
	int ServerRecvBytesCounter;

	int ObjectsCastingShadows;
	int LightsCastingShadows;


	float SecCounter;

	void Reset();

	void ResetBlocks();
	void EnterBlock(int BlockID);
	void LeaveBlock(int BlockID);
	float GetBlockPercent(int BlockID);
};


#endif
