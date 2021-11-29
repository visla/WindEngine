/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		NxuPhysicsExport.h

		This is the common exporter implementation that contains format independent export logic.

		Changelist
		----------

*/
#ifndef NXU_PHYSICS_EXPORT_H
#define NXU_PHYSICS_EXPORT_H

#include <stdio.h>

#include "NxuStream.h"

class NxuPhysicsExport
{
protected:
    NxuPhysicsCollection* mCollection; //The collection of information in the process of being exported
    NxuOutputStream* mStream;           //The stream to which the data will be written
    NxuPhysicsModel*         mCurrentModel;  //The current context to which objects are being added
    NxuActorDesc* mCurrentActor;            //The current actor to which shapes are being added

    NxArray<NxuPhysicsModel*> mStreamedModels; //List of models that have already been streamed

    //returns the current model if in use, otherwise the default model
    NxuPhysicsModel* getCurrentModel();
    //returns the current actor if in use, otherwise the default actor
    NxuActorDesc* getCurrentActor();

    //Lookup/Cache function for shared data
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
    NxuCCDSkeletonDesc* AddSkeleton(NxCCDSkeleton* skeleton);
#endif
    NxuConvexMeshDesc* AddConvex(NxConvexMesh* mesh);
    NxuTriangleMeshDesc* AddTriangleMesh(NxTriangleMesh* mesh);
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
    NxuHeightFieldDesc* AddHeightField(NxHeightField* hf);
#endif
    NxuActorDesc* AddActor(NxActor* actor);

    NxU32 GetShapeIndex(NxuActorDesc* actor, NxShape* shape);

public:
    NxuPhysicsExport();
    virtual ~NxuPhysicsExport();

    virtual NX_BOOL setStream(NxuOutputStream* userStream);
    virtual NxuOutputStream* getStream();

    //Obtain a reference to the data stored so far, be wary of memory management issues if you modify it
    NxuPhysicsCollection* getCollection() {return mCollection;} 
    void setCollection(NxuPhysicsCollection* collection) {mCollection = collection;} 

    virtual NX_BOOL flush(); //Forces data into stream
    void clear(); //Clears collection, including deleting all descriptors contained in the collection

    //SDK Streaming
    virtual void WriteSDK(NxPhysicsSDK* physicsSDK); //Writes the entire SDK, including all scenes

    //Model definition, use to define structured groupings of objects if desired
    virtual void BeginModel(const char* modelName);
    virtual void EndModel();
    virtual void InstanceModel(const char* modelName, const NxMat34& pose); //Inserts a reference to the specified model into the current scene or model

    //Individual PhysX object streaming
    virtual void Write( const NxPhysicsSDK* physicsSDK); //Writes out all SDK parameters, but not contained scenes
	virtual void Write( NxParameter p,	NxReal v );
	virtual void Write(	 NxScene *scene );
	virtual void Write( const NxSceneDesc *scene	);
	virtual void Write(	 NxActor *a );
	virtual void Write(	 NxActorDesc *a, NxBodyDesc *b );
	virtual void Write(	 NxPairFlag *a	);
    virtual void Write( NxU32 pairflag, NxuActorDesc* actorIndex0, NxuActorDesc* actorIndex1, NxU32 shapeIndex0 = NxU32(-1), NxU32 shapeIndex1 = NxU32(-1));
	virtual void Write(	 NxMaterial *a	);
	virtual void Write(	 NxMaterialDesc *a, NxMaterialIndex index	);
	virtual void Write(  NxJoint *j );
	virtual void Write(	NxShape	*shape );
	virtual void Write(	 const	NxShapeDesc	*d);

	virtual void Write( NxTriangleMesh *mesh );
	virtual void Write( NxConvexMesh *mesh );
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
    virtual void Write( NxHeightField *heightfield );
#endif
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
	virtual void Write( NxCCDSkeleton *skeleton	);
#endif
	virtual void Write( NxEffector *e );
#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
    virtual void Write( NxFluid *fluid);
#endif

    //Extended descriptor streaming
    virtual void Write(NxuTriangleMeshDesc* d);
    virtual void Write(NxuConvexMeshDesc* d);
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
    virtual void Write(NxuHeightFieldDesc* d);
#endif
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
    virtual void Write(NxuCCDSkeletonDesc* d);
#endif
    virtual void Write(NxuMaterialDesc* d);
    virtual void Write(NxuShapeDesc* d);
    virtual void Write(NxuActorDesc* d);
    virtual void Write(NxuJointDesc* d);
    virtual void Write(NxuSpringAndDamperEffectorDesc* d);
    virtual void Write(NxuPairFlagDesc* d);
#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
    virtual void Write(NxuFluidDesc* d);
#endif
    virtual void Write(NxuPhysicsModel* d);
    virtual void Write(NxuSceneDesc* d);
};

#endif //NXU_PHYSICS_EXPORT_H


//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND

