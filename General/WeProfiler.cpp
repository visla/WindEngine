/*
    WeProfiler.cpp

	Autor: Nebojsa Vislavski
	Date: 1.11.2005.
	Version: 1.0


    This profiler is used to measure different things in rendering engine

*/

#include <WeProfiler.h>
#include <WeTimer.h>
#include <math.h>



struct BlockInfo {
	LARGE_INTEGER EnterTime;
	LARGE_INTEGER TotalTime;
	int Inside;
};

BlockInfo BlocksInfo[MAX_BLOCKS];

WeProfiler *WeProfiler::_instance = 0;

WeProfiler *WeProfiler::Instance()
{
	if (_instance) return _instance;
	_instance = new WeProfiler;
	return _instance;
}


WeProfiler::WeProfiler()
{
	Reset();
	ObjectsInWorld = 0;
	// Networking
	SecCounter = 0;
	ServerSendBytesPerSec = 0;
	ServerRecvBytesPerSec = 0;
	ServerSendBytesCounter = 0;
	ServerRecvBytesCounter = 0;
	ClientSendBytesPerSec = 0;
	ClientRecvBytesPerSec = 0;
	ClientSendBytesCounter = 0;
	ClientRecvBytesCounter = 0;

	for (int i = 0; i < MAX_BLOCKS; ++i)
		BlocksInfo[i].Inside = 0;

	ResetBlocks();
}

void WeProfiler::Reset()
{
	RenderTreeNodesVisible = 0;
	ObjectsRendered = 0;
	BatchItemsQueried = 0;
	BatchItemsRendered = 0;
	BatchItemsSkipped = 0;	
	BatchItemsQueued = 0;
	ParticlesCount = 0;

	AlphaBatchItemsRendered = 0;
	AlphaBatchItemsQueried = 0;
	AlphaBatchItemsSkipped = 0;
	AlphaBatchItemsQueued = 0;
	ZonesVisible = 0;
	Zones.Clear();
	tmp = 0;

	ObjectsCastingShadows = 0;
	LightsCastingShadows = 0;
	PolygonsRendered = 0;
	ReflectiveObjectsDrawn = 0;
	ShaderBegins = 0;	
	ShadersActive.Clear();

	SecCounter += WeTimer::Instance()->GetDeltaTimeF();
	if (SecCounter > 1.0f)
	{
		ClientSendBytesPerSec = ClientSendBytesCounter;
		ClientRecvBytesPerSec = ClientRecvBytesCounter;
		ServerSendBytesPerSec = ServerSendBytesCounter;
		ServerRecvBytesPerSec = ServerRecvBytesCounter;

		ClientSendBytesCounter = 0;
	    ClientRecvBytesCounter = 0;
		ServerSendBytesCounter = 0;
		ServerRecvBytesCounter = 0;
		SecCounter = 0;
	}
}



void WeProfiler::ResetBlocks()
{
	for (int i = 0; i < MAX_BLOCKS; ++i)
		BlocksInfo[i].TotalTime.QuadPart = 0;
}


bool IsValidBlock(int BlockID)
{
	return (BlockID >= 0) && (BlockID < MAX_BLOCKS);
}



void WeProfiler::EnterBlock(int BlockID)
{
	if (IsValidBlock(BlockID))
	{
		if (BlocksInfo[BlockID].Inside == 0)
			QueryPerformanceCounter(&BlocksInfo[BlockID].EnterTime);
		++BlocksInfo[BlockID].Inside;
	}
}



void WeProfiler::LeaveBlock(int BlockID)
{
	if (IsValidBlock(BlockID))
	{
		--BlocksInfo[BlockID].Inside;
		if (BlocksInfo[BlockID].Inside == 0)
		{
			LARGE_INTEGER t;
			QueryPerformanceCounter(&t);
			BlocksInfo[BlockID].TotalTime.QuadPart += (t.QuadPart - BlocksInfo[BlockID].EnterTime.QuadPart);
		}
	}
}



float WeProfiler::GetBlockPercent(int BlockID)
{
	LARGE_INTEGER TotalTime;
	TotalTime = BlocksInfo[0].TotalTime;
	if ((TotalTime.QuadPart == 0) || (!IsValidBlock(BlockID)))
		return 0.0f;
	else
		return (float)((double)BlocksInfo[BlockID].TotalTime.QuadPart / (double)TotalTime.QuadPart);
}
