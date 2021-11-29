/*
    WeShadowVolumeRenderer.h

    Autor: Nebojsa Vislavski
	Date: 13.7.2006.
	Version: 1.0

*/


#ifndef _WESHADOWVOLUME_RENDERER_H
#define _WESHADOWVOLUME_RENDERER_H


/*
#ifndef DEBUG


#ifdef _HAS_ITERATOR_DEBUGGING
#undef _HAS_ITERATOR_DEBUGGING
#endif // _HAS_ITERATOR_DEBUGGING

#define _HAS_ITERATOR_DEBUGGING 0


#ifdef _DEBUG
#undef _DEBUG
#endif


#endif // DEBUG
*/


#include <d3dx9.h>
#include <d3d9.h>
#include <WeBaseRenderer.h>
#include <WindEngine.h>
#include <SmartList.h>
#include <WeBatchItem.h>
#include <WeLightBatch.h>
#include <SmartList.h>



class WeWorld;
class WeParticleSystem;
class WeParticleVertex;
class WeShader;
class WeCamera;
class WeLight;



class WINDENGINE_API WeShadowVolumeRenderer: public WeBaseRenderer
{
private:
	static WeShadowVolumeRenderer *_instance;
protected:

	bool						bInitialized;

	CSmartList <WeBatchItem *>	ShadowBatches;
	CSmartList <WeLight *>		Lights;
	CSmartList<WeLightBatch *>	LightBatches;

public:
	static WeShadowVolumeRenderer *Instance();
	WeShadowVolumeRenderer();
	virtual ~WeShadowVolumeRenderer() { ShutDown(); }


	virtual bool	Initialize();
	virtual void	ShutDown();

    void			ClearAll();
	void			OnLostDevice();
	void			OnResetDevice();

	bool			Preparing;

	void			RenderQueuedBatches(WeWorld* World);
	void			PrepareQueuedBatches(WeWorld* World, WeCamera* Camera);
	void			QueueBatchItem(WeBatchItem* BatchItem);

	// you should almost never call this. This is automaticly called in QueueBatchItem	
	// when material has ALPHA type
	void			QueueAlphaBatch(WeBatchItem *BatchItem); 

};





#endif
