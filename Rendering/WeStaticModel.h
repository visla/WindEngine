/*
       File: WeStatixModel.h
	   Author: Vislavski Nebojsa
	   Date: 12.09.2005.
	   Version: 1.0
*/



#ifndef _WESTATICMODEL
#define _WESTATICMODEL


#include <WeWorld.h>
#include <WeMatrix.h>
#include <CString.h>
#include <WindEngine.h>



class WeMesh;
class WeCamera;
class WeActor;



class WINDENGINE_API WeStaticModel
{
protected:
	int Version;

	

public:
	WeStaticModel(void);
	virtual ~WeStaticModel(void){}
	virtual void Clone(WeStaticModel *Original, bool DeepCopy);
	
	bool	IsModel; // for normal animated model


	WeMesh* Mesh; // Mesh it is goint to use. You can have many meshes here
	//WeMesh* ShadowMesh;

	// this is mesh used for quering. It should be same as original in materials, ... but 
	// should have very very low number of polygons
	WeMesh		*QueryMesh;

	WeMatrix	TM;
	WeMatrix	PrevTM;

	String		Name;
	String		MeshName;
	String		QueryMeshName;
	//String		ShadowMeshName;


	// Owner dont have to be supplied. Some children classes uses owner but
	// static model dont use this owner at all
	virtual bool Load(StringParam Name, bool ForceReload = false);  // returns false if not loaded
	virtual bool Save(StringParam HoleName, bool Replace = false);
	virtual void LoadMaterials(WeActor *Owner);
	virtual void LoadMaterials(int MaterialPreset = 0);
	virtual void Draw(WeWorld* World, WeCamera* Cam, WeActor* Owner);
	virtual void UpdateTransformations(WeMatrix &parentTM, WeMatrix &prevTM); 
	virtual	void ComputeBoundingBox(WeActor *Owner);
	virtual void SetMaterial(StringParam MaterialName, int forMaterialPreset);
    virtual	String GetCurrentMaterialName(WeActor *Owner);	
};



#endif