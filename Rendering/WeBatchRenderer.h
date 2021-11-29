/*
       File: WeBatchRenderer.h
	   Author: Tomislav Kukic
	   Date: 12.09.2005.
	   Version: 1.0
*/



#ifndef _WEBATCHRENDERER
#define _WEBATCHRENDERER



#include <WeBaseRenderer.h>
#include <WeWorld.h>
#include <WindEngine.h>



class WeMaterial;
class WeBatchItem;
class WeShaderBatch;
class WeShader;



class WINDENGINE_API WeBatchRenderer: public WeBaseRenderer
{
friend class WeProjectorRenderer;  // need access to shader batches
friend class WeVelocityRenderer;
friend class WeGlowRenderer;
friend class WeRefractiveRenderer;

private:
	static WeBatchRenderer* _instance;

protected:
	WeBatchRenderer(void);
	virtual ~WeBatchRenderer(void);

	// This is called before actual rendering to mark batch items occluded.
	void	QueryQueuedBatches(WeWorld *World);	
	void	SetAlphaRef(WeMaterial *Material, bool TurnOff);

	void	PrepareZonedQueuedBatches(WeWorld* World, WeCamera* Camera); // this is used if zoned rendering is needed
	void	SetStandardParams(WeShader *Shader, WeBatchItem *BatchItem);
	bool	Preparing;



	// Batch Items
	// --------------------------------------------
	CSmartList<WeShaderBatch*> ShaderBatchList;
	CSmartList<WeShaderBatch*> RefractiveShaderBatchList;
	CSmartList<WeBatchItem *> AlphaBatchList;
	CSmartList<WeBatchItem *> ForegroundBatchList;

public:
	static WeBatchRenderer* Instance();

	// HIGH VERSION RENDERING
	void	RenderBatchesHigh(WeWorld *World);
	void	RenderAlphaBatchesHigh(WeWorld *World);	
	void	RenderZBuffer(WeWorld *World);



	// LOW VERSION RENDERING
	void	RenderAmbientBatches(WeWorld* World);
	void	RenderAmbientAlphaBatches(WeWorld *World);

	void	RenderQueuedBatches(WeWorld* World);
	void	PrepareQueuedBatches(WeWorld* World, WeCamera* Camera);
	void	QueueBatchItem(WeBatchItem* BatchItem);

	// This will do actual rendering for Batch items
	void	DrawQueuedBatches(WeWorld *World);
	void	DrawAlphaBatches(WeWorld *World);  // including particle systems
	void	DrawForegroundBatches(WeWorld *World);
	void	DrawRefractiveBatches(WeWorld *World);

	// you should almost never call this. This is automaticly called in QueueBatchItem	
	// when material has ALPHA type
	void	QueueAlphaBatch(WeBatchItem *BatchItem); 
	void	QueueForegroundBatch(WeBatchItem *BatchItem); 
	void	QueueRefractiveBatch(WeBatchItem *BatchItem); 

	virtual void ClearAll();

};



#endif