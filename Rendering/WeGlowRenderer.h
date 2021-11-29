/*
WeGlowRenderer.h

Autor: Nebojsa Vislavski
Date: 17.7.2006.
Version: 1.0

*/


#ifndef _WEGLOW_RENDERER_H
#define _WEGLOW_RENDERER_H


#include <d3dx9.h>
#include <d3d9.h>
#include <WeBaseRenderer.h>
#include <WindEngine.h>
#include <SmartList.h>
#include <WeBatchItem.h>


class WeWorld;
class WeParticleSystem;
class WeParticleVertex;
class WeShader;
class WeCamera;



class WINDENGINE_API WeGlowRenderer: public WeBaseRenderer
{
private:
	static WeGlowRenderer *_instance;
protected:

	bool			bInitialized;
	CSmartList<WeBatchItem *> BatchList;

public:
	static WeGlowRenderer *Instance();
	WeGlowRenderer();
	virtual ~WeGlowRenderer() { ShutDown(); }


	virtual bool	Initialize();
	virtual void	ShutDown();

	void			ClearAll();
	void			OnLostDevice();
	void			OnResetDevice();
//	void			QueueBatchItem(WeBatchItem *Item);

	void			RenderQueuedBatches(WeWorld* World, WeCamera *Cam);

};





#endif
