/*
       File: WeWorld.h
	   Author: Tomislav Kukic
	   Date: 13.09.2005.
	   Version: 1.0
*/




#ifndef _WEWORLD
#define _WEWORLD


#include <SmartList.h>
#include <CString.h>
#include <WeVector.h>
#include <WindEngine.h>
#include <WeSphere.h>
#include <WeFileStream.h>
#include <WeColor.h>



class WeRenderTree;
class WeBatchRenderer;
class WeActor;
class WeCamera;
class WeLight;
class WeSkyBox;
class WePathNode;
class WeZone;
class WePortal;
class WeFrustum;



typedef void (*CALLBACK_LOADPROGRESS) (float Progrees, bool &StopLoading);
typedef void (*CALLBACK_BAKEPROGRESS) (int FacesLit, float Progrees, bool &StopLoading);
typedef void (*CALLBACK_MAPCALLBACK) ();



#define WORLD_VERSION	1
#define MAX_PATH_NODES 1000



#define RENDERPHASE_AMBIENT 0
#define RENDERPHASE_AMBIENTALPHA 1
#define RENDERPHASE_LIGHTING 2
#define RENDERPHASE_ALPHA 3
#define RENDERPHASE_PROJECTORS 4
#define RENDERPHASE_GLOW 5
#define RENDERPHASE_REFRACTIONS 6
#define RENDERPHASE_FOREGROUND 7
#define RENDERPHASE_FLARES 8
#define RENDERPHASE_SKYBOX 9

#define RENDERPHASE_COUNT 10


class WINDENGINE_API WeWorld
{
	friend class	WeBatchRenderer;
	friend class	WePhysics;
	friend class	WeRenderTree;
	friend class	WeLightMap;

protected:	

	WeVector*		MinPos;
	WeVector*		MaxPos;

	bool			isLoaded;
	bool			isLoading;
	bool			isUnloading;
	bool			isHeaderLoaded;
	bool			bHasTicked;
	int				Version;			// this is map version
	DWORD			WorldFrame;			// frame the world is in. Incremented in tick
	bool			ZonedRendering;		// Should it use zoned rendering. Default is false
	bool			ZonedRenderingPrepared;
	float			MaxFacesToLight;	// max faces to light
	float			FacesLit;			// how many faces are lit

	bool			bRenderPhaseEnabled[RENDERPHASE_COUNT];

	
	virtual void	OnLoadMapHeader(WeFileStream &FS) {};				// This will load map header
	virtual void	OnSaveMapHeader(WeFileStream &FS) {};				// This will save map header

			void	PrepareZonedRendering();	// this will rearrange objects in world into zones 
			void	ConnectZones();				// connects zones with portals using zone names

public:
	CSmartList <WeZone *>		Zones;
	CSmartList <WePortal *>		Portals;

	bool			IsEditor;

	WeSkyBox		*SkyBox;				// this is sky box that world is using
	WeRenderTree	*RenderTree;
	WeActor			*Actors;
	WeLight			*Lights;
	WePathNode		*PathNodes;
	float			PercentLoaded;

	// sorted PathNodes, updated only when world is loaded
	int				TotalPathNodes;
	int				TotalNormalPathNodes;
	WePathNode		*FirstPathNode;
	int				TotalPathLen;

#ifdef _DEBUG
	bool			ShowActorTypes[10]; // za -1, -2, -3, ..., -10
#endif

					WeWorld(void);// this is public for testing
	virtual			~WeWorld(void);

	WeActor*		AddActor(WeActor* Actor);
	bool			RemoveActor(WeActor* Actor); // removes Actor from world but doesnt destroy it. 
	
	// Sky Boxes are not added to the world. Use this to set sky box
	WeSkyBox*		GetSkyBox()  { return SkyBox; }
	
	// called before actual rendering. Override this to give it some funcionality
	// you should initialize some rendering states for some objects here if you want. Used very rarely.
	void			PreRender(WeCamera* Cam);		
	void			Render(WeCamera* Cam, bool RenderPostProcess = true);		// called when rendering
	// called after rendering. Override this to give it some funcionality. You should write 
	// you should write interface things here.  
	void			PostRender(WeCamera* Cam);  
	bool			GetZonedRendering() { return ZonedRendering; }  
	void			SetZonedRendering(bool zonedrendering) { ZonedRenderingPrepared = false;  ZonedRendering = zonedrendering; }
	// this will gather all zones that are visible through this camera. This is called during rendering
	bool			GatherCameraZones(WeCamera *Cam, CSmartList <WeZone *> &Zones); 
	void			GatherCameraZones(WeCamera *Cam, WeZone *Parent, WeFrustum *Frustum, CSmartList <WeZone *> &Zones); 

	// ----- Map Loading and Saving routines and callbacks -----
	WePathNode*		GetPathNode(int SequenceNumber);
	WePathNode*		GetNextPathNode(int CurrentNodeNumber, int aNodeType, int LastNodeNumber);
	int				ProcessPathNodes(WePathNode *FirstNode, int LastNodeNumber, WePathNode **RefLastNode, int TubeID);
						// rezultat je broj noda u nizu
	WePathNode*		NumerizePathNodes(WePathNode *FirstNode, float *Index, float Step);
	void			DoSortPathNodes();

	// this is called while loading to identify progress in percents
	// if you set StopLoading to true then it will stop loading map. 
	CALLBACK_LOADPROGRESS		OnLoadProgress;
	CALLBACK_BAKEPROGRESS		OnBakingProgress; // called while precalculating lights
	CALLBACK_MAPCALLBACK		OnLoadCanceled;
	CALLBACK_MAPCALLBACK		OnMapLoaded;
	
	// this is called when maps needs to load some unknown item
	// need to create object, initialized it, and add it to world
	virtual WeActor* OnLoadMapItem(int Type, WeFileStream &FS) { return NULL; };		

	// You should override Load and Save methods only if you need really special save and load of map.
	// NOTE! If you override load and save please call OnMapLoaded after loading and OnLoadProgress while loading
	virtual bool	Load(StringParam name);			// Calls loadmapheader and then load items. Set ForEditor to true if you wish to load map for editor
	// Calls loadmapheader and then load items. Set ForEditor to true if you wish to load map for editor. Ticked means it will load item by item. 
	// so you have to call this LoadInTick method in each tick and stop doing it when map is loaded
	// calling this method on already loaded map has no effect. If you wish to load again
	// use Unload or NewWorld method before.
	// This method doesnt call callbacks OnLoadProgress, OnLoadCanceled, OnMapLoaded
	virtual bool	LoadInTick(StringParam name, bool LoadToEditor = false, bool StopLoading = false);    
	virtual bool	Unload();						// TODO: unload map from memory. After this world is empty
	// Calls savemapheader and then serialize of each item. If name of map exists it is overwritten
	virtual bool	Save(StringParam name);

	// export all prefabs to Wavefront OBJ
	virtual bool	ExportToOBJ(StringParam name);

	// This will load only map header from map. 
	// You cant call Load or Save on map that is not yet created
	virtual bool	LoadMapHeader(StringParam name);
	virtual bool	SaveMapHeader(StringParam name);
    
	// This method fill find nodes in sphere. 
	// TODO: this method is not implemented yet
	void			FindPathNodes(CSmartList <WePathNode *> &PathNodes, WeSphere &InSphere);
	// This will find closest node that has the direction (z-axis) not opposite to vector from location to that node	
	// This method is used in racing games when you need to find closest path node in the right track direction
	WePathNode*		FindClosestDirectionPathNode(WeVector &Location);
	WePathNode*		FindClosestInvDirectionPathNode(WeVector &Location);
	WePathNode*		FindNextDirectionPathNode(WeVector &Location);
	void			GetRefPathNodes(WeVector &Location, WePathNode *&Node1, WePathNode *&Node2, float &D1, float &D2, WeVector &RefPos);


	// This will find all nearby dynamic lights. and fill the structure with maximum NumLights
	// TODO: This currently supports only one light per shader, make it support more.
	void			FindNearestLights(WeSphere &ObjectsSphere, CSmartList <WeLight *> &Lights, int NumLights, int NumDirectLights, bool SphereIsDynamicActor);
	// This will find summed light intensity at one point.  
	void			FindSumColorLights(WeVector &Location, WeColor &outSumColor);
	WeActor*		FindActor(StringParam Name);
	bool			IsLoading(){ return isLoading; }
	bool			IsUnloading(){ return isUnloading; }
	bool			IsLoaded() { return isLoaded; }
	bool			IsHeaderLoaded() { return isHeaderLoaded; }
	DWORD			GetWorldFrame() { return WorldFrame; }

	// call this when you want drawing optimizations like octree to be in effect
	// This should be called when some prefab is added to scene all group of prefabs is added
	// or after map is loaded this is automaticly called
	// TODO: Check to see if this is safe to remove
	virtual void	BuildWorld();  

	// This will calculate lightmaps for prefabs in world`s list
	void			CalculateStaticLights(bool SimpleGenerate = false);
	void			CalculateStaticLights(CSmartList <WeLight *> &Lights, bool SimpleGenerate = false);
	void			CalculateStaticLights(CSmartList <WeActor*> &Actors, bool SimpleGenerate = false);

	bool			HasTicked();
	void			Tick();    // This updates actor states

	// This clears all actors from list. This doesnt destroy them.
    // Call destroy later
	void			ClearAll(); 
	void			NewWorld();
	
	// Render Phases
	bool			RenderPhaseEnabled(int RenderPhase);
	void			EnableRenderPhase(int RenderPhase, bool enabled);

	void			SetMaxRenderDistanceToAll(float MaxRenderDistance);
	void			Editor_HideDoodads();
	void			Editor_ShowDoodads();
	void			Editor_HideAll();
	void			Editor_ShowAll(); // this will show all hidden objects
	void			Editor_ShowPathNodes();
	void			Editor_HidePathNodes();
	void			Editor_HideZones();
	void			Editor_ShowZones();
	void			Editor_HidePortals();
	void			Editor_ShowPortals();
	void			ClearAllLights();

};




#endif
