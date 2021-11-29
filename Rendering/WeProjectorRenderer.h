/*
    WeProjectorRenderer.h

    Autor: Nebojsa Vislavski
	Date: 13.7.2006.
	Version: 1.0

*/


#ifndef _WEPROJECTOR_RENDERER_H
#define _WEPROJECTOR_RENDERER_H


#include <d3dx9.h>
#include <d3d9.h>
#include <WeBaseRenderer.h>
#include <WindEngine.h>
#include <SmartList.h>
#include <WeBatchItem.h>
#include <WeLightBatch.h>


class WeWorld;
class WeParticleSystem;
class WeParticleVertex;
class WeShader;
class WeCamera;



class WINDENGINE_API WeProjectorRenderer: public WeBaseRenderer
{
private:
	static WeProjectorRenderer *_instance;
protected:

	bool			bInitialized;

    CSmartList <WeLightBatch *> LightBatchesOmni;
    CSmartList <WeLightBatch *> LightBatchesSpot;

	void			RenderOmniBatches(WeWorld *World);
	void			RenderSpotBatches(WeWorld *World);

public:
	static WeProjectorRenderer *Instance();
	WeProjectorRenderer();
	virtual ~WeProjectorRenderer() { ShutDown(); }


	virtual bool	Initialize();
	virtual void	ShutDown();

    void			ClearAll();
	void			OnLostDevice();
	void			OnResetDevice();

	void			RenderQueuedBatches(WeWorld* World);
	void			PrepareQueuedBatches(WeWorld* World, WeCamera* Camera);
	void			QueueBatchItem(WeBatchItem* BatchItem);

};





#endif
