/*
    WeReflectionRenderer.h


	Autor: Nebojsa Vislavski
	Date: 23.11.2005.
	Version: 1.0
	Platform: Win32


	This is used to render env maps for batch items that needs them. 
	We need that batch item need this if material is ReflectionType = REALTIME, 
	and Batch Item queuing will give us this item if it detects that material.
	This is integral part of engine and cannot be used by client
	

*/

#ifndef _WEREFLECTIONRENDERER_H
#define _WEREFLECTIONRENDERER_H


#include <SmartList.h>
#include <WeBatchItem.h>
#include <WeBaseRenderer.h>


class WeWorld;
class WeCamera;


#define WEREFLSTATUS_CLEARED 0
#define WEREFLSTATUS_ACTIVE 1
#define WEREFLSTATUS_FINISHED 2




class WeReflectionRenderer: public WeBaseRenderer
{
private:
	static	WeReflectionRenderer	*_instance;
protected:
			int						UpdateInterval;  // each num frames render is actualy done
			int					    FrameCounter;
			// list of batch items that need reflection. Queue is responsible for filling this list

			// env maps list. When needed than is added. Released only at end
			// Can be performance problem if many reflective objects are shown
			// at the same time. (this is not usual case)
			class ReflectionBatch
			{
			public:
				WeActor *Actor;				
				int		LastUpdateFrame; // frame it was last updated. if curent - this is > 2 then it is replaceble
				WeRenderTarget *EnvMap;
				int		FrameCounter;
			};


			CSmartList <WeReflectionRenderer::ReflectionBatch *> ReflectiveBatches;
			int						ItemsAddedCount;


			// status - Cleared, Active, Finished. 
			int						Status;
			int						BatchIndex;



public:
	static WeReflectionRenderer *Instance();

	// If this is true which is default value than it means
	// that it will render other reflective batch items when making env maps
	// This can make some objects dark if pixel shader that is used cannot 
	// do empty env maps (not yet calculated)
	bool					DoDoubleReflections;
	int						EnvMapSize; // this is envmap size. All env maps have this size
	int						ReflectionUpdate; // number of objects that will be updated reflections. 
	

	WeReflectionRenderer(); 
	~WeReflectionRenderer() { ClearAll(); }



	// This is main method for rendering environment maps. It needs 
	// camera and world. It is called from World->Render method
	void  Render(WeWorld *World, WeCamera *Camera); 

	// This is queuing mechanism of batchrenderer
	// returns false if doesnt want to take care of batch item
	// returns true if wants to take care of this batch item
	bool  QueueBatchItem(WeBatchItem *BatchItem);
	void  ClearAll();

};


#endif