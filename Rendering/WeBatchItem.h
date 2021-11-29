/*
      File: WeBathcItem.h
	  Author: Tomislav Kukic
	  Date: 12.09.2005.
	  Version: 1.0
*/



#ifndef _WEBATCHITEM
#define _WEBATCHITEM



#include <WeQuery.h>
#include <WeMatrix.h>
#include <WeAttribRange.h>
#include <WindEngine.h>
#include <SmartList.h>
#include <WeColor.h>
#include <WeSphere.h>

class WeMesh;
class WeMaterial;
class WeStaticModel;
class WeParticleSystem;
class WeRenderTarget;
class WeActor;
class WeLight;

class WINDENGINE_API WeBatchItem
{
friend class WeQuery;
friend class WeBatchRenderer;
friend class WeProjectorRenderer;
friend class WeVelocityRenderer;
friend class WeGlowRenderer;

protected:
	bool			SkipItem; // if this is set then batchrenderer will skip this item in rendering

public:
	WeBatchItem(): Alpha(false), Query(NULL), Actor(NULL), EnvMap(NULL), SkipItem(false), QueryMesh(NULL) { }	
	~WeBatchItem();

    // lights affecting this batch item. Can be empty or can hold as many lights 
	// as shader wants
	CSmartList <WeLight *> Lights;    
	WeColor			SumLightColor; // if using summed light color
	WeAttribRange	AttribRange;
	WeActor			*Actor;
	WeStaticModel*	Model;
	WeMesh*			Mesh;
	WeMesh*			ShadowMesh;
	WeMaterial*		Material;
	WeMatrix		WorldMatrix;
	WeMatrix		PrevWorldMatrix;
	bool			Alpha;
	WeSphere		BSphere;

	// Query stuff
	WeQuery			*Query;  // query used to modify this mesh
	WeMesh*			QueryMesh; // this is low polygon mesh used only for quering

	WeRenderTarget	 *EnvMap; // environment map for real time reflections

	float ZDist;    
};



#endif
