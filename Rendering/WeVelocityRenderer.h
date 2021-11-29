/*
    WeVelocityRenderer.h

    Autor: Nebojsa Vislavski
	Date: 17.7.2006.
	Version: 1.0

*/


#ifndef _WEVELOCITY_RENDERER_H
#define _WEVELOCITY_RENDERER_H


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



class WINDENGINE_API WeVelocityRenderer: public WeBaseRenderer
{
private:
	static WeVelocityRenderer *_instance;
protected:

	bool			bInitialized;

public:
	static WeVelocityRenderer *Instance();
	WeVelocityRenderer();
	virtual ~WeVelocityRenderer() { ShutDown(); }


	virtual bool	Initialize();
	virtual void	ShutDown();

    void			ClearAll();
	void			OnLostDevice();
	void			OnResetDevice();

	void			RenderQueuedBatches(WeWorld* World, WeCamera *Cam);

};





#endif
