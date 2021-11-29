#ifndef _WEFLARE_RENDERER_H
#define _WEFLARE_RENDERER_H


#include <d3dx9.h>
#include <d3d9.h>
#include <WeBaseRenderer.h>
#include <WindEngine.h>
#include <SmartList.h>
#include <WeBatchItem.h>
#include <WeFlareBatch.h>
#include <WeWorld.h>
#include <WeCamera.h>
#include <WeLight.h>



class WINDENGINE_API WeFlareRenderer: public WeBaseRenderer
{
private:
	static WeFlareRenderer *_instance;
protected:

	bool			bInitialized;

	CSmartList <WeFlareBatch *> FlareBatches;

public:
	static WeFlareRenderer *Instance();
	WeFlareRenderer();
	virtual ~WeFlareRenderer() { ShutDown(); }


	virtual bool	Initialize();
	virtual void	ShutDown();

	void			OnLostDevice();
	void			OnResetDevice();

	void			RemoveLight(WeLight *Light);
	void			RemoveAllLights();

	void			PrepareQueuedBatches(WeWorld* World, WeCamera* Camera);
	void			RenderQueuedBatches(WeWorld* World, WeCamera* Camera);
	//void			QueueBatchItem(WeBatchItem* BatchItem);
};





#endif
