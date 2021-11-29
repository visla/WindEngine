/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		NxuPhysicsExport.cpp

		This is the common exporter implementation that contains format independent export logic.

		Changelist
		----------

*/
#include <stdio.h>

#include "NxuPhysicsExport.h"

NxuPhysicsExport::NxuPhysicsExport()
{
    mStream = 0;
    mCollection = new NxuPhysicsCollection();
    mCurrentModel = 0;
    mCurrentActor = 0;
}

NxuPhysicsExport::~NxuPhysicsExport()
{
    mStream = 0;
    if (mCollection) delete mCollection;
}

NxuPhysicsModel* NxuPhysicsExport::getCurrentModel()
{
    NxuPhysicsModel* model = mCurrentModel;
    if (!model)
    {
        model = &(mCollection->defaultModel);
    }
    return model;
}

NxuActorDesc* NxuPhysicsExport::getCurrentActor()
{
    NxuActorDesc* actor = mCurrentActor;
    if (!actor)
    {
        actor = &(mCollection->defaultActor);
    }
    return actor;
}

NX_BOOL NxuPhysicsExport::setStream(NxuOutputStream* userStream)
{
    mStream = userStream;
    return true;
}

NxuOutputStream* NxuPhysicsExport::getStream()
{
    return mStream;
}

NX_BOOL NxuPhysicsExport::flush()
{
    NX_BOOL status = false;
    //
    NxuOutputStream* s = getStream();
    NxuSectionTypes section;
    if (s)
    {
        section = NxuTypeUnknown;
        section = mCollection->store(*(s), "Physics Library");
        s->flush();
    }
	
    return section != NxuTypeUnknown;
}


void NxuPhysicsExport::clear() //Clears collection
{
    if (mCollection)
    {
        delete mCollection;
    }
    mCollection = new NxuPhysicsCollection();

    mCurrentModel = 0;
    mCurrentActor = 0;

    mStreamedModels.clear();
}

#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
NxuCCDSkeletonDesc* NxuPhysicsExport::AddSkeleton( NxCCDSkeleton* skeleton)
{
    int num = mCollection->skeletons.size();
    for (int i=0; i<num; ++i)
    {
        if (skeleton == (mCollection->skeletons[i])->mInstance) {
            return mCollection->skeletons[i];
        }
    }
    //Note: Currently no way to get triangle mesh data back from a skeleton
    // only cooked version is accessable.
    NxuCCDSkeletonDesc* skel = new NxuCCDSkeletonDesc();
    skel->setToDefault();
    skel->mCookedDataSize = skeleton->getDataSize();
    if (skel->mCookedDataSize)
    {
        skel->mCookedData = new NxU8[skel->mCookedDataSize];
        skeleton->save(skel->mCookedData, skel->mCookedDataSize);
    }
    else
    {
        skel->mCookedData = 0;
    }
    skel->mInstance = skeleton;
    mCollection->skeletons.pushBack(skel);
    return skel;
}
#endif

NxuConvexMeshDesc* NxuPhysicsExport::AddConvex(NxConvexMesh* mesh)
{
    int numConvex = mCollection->convexMeshes.size();
    for (int i=0; i<numConvex; ++i)
    {
        if (mesh == (mCollection->convexMeshes[i])->mInstance) {
            return mCollection->convexMeshes[i];
        }
    }
    NxuConvexMeshDesc* convexMesh = new NxuConvexMeshDesc();
    //TODO: Add option to store cooked data
    convexMesh->mCookedData = 0;
    convexMesh->mCookedDataSize = 0;
    convexMesh->mInstance = mesh;
    mesh->saveToDesc(*convexMesh);
    mCollection->convexMeshes.pushBack(convexMesh);

    return convexMesh;
}

NxuTriangleMeshDesc* NxuPhysicsExport::AddTriangleMesh( NxTriangleMesh* mesh)
{
    int numTrimesh = mCollection->triangleMeshes.size();
    for (int i=0; i<numTrimesh; ++i)
    {
        if (mesh == mCollection->triangleMeshes[i]->mInstance) {
            return mCollection->triangleMeshes[i];
        }
    }
    NxuTriangleMeshDesc* triMesh = new NxuTriangleMeshDesc();
    //TODO: Add option to store cooked data
    triMesh->mCookedData = 0;
    triMesh->mCookedDataSize = 0;
    triMesh->mInstance = mesh;
    mesh->saveToDesc(*triMesh);
	if(mesh->getPageCount()) 
		triMesh->flags |= NX_MF_HARDWARE_MESH;
	mCollection->triangleMeshes.pushBack(triMesh);

    return triMesh;
}

#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
NxuHeightFieldDesc* NxuPhysicsExport::AddHeightField( NxHeightField* heightfield)
{
    int num = mCollection->heightFields.size();
    for (int i=0; i<num; ++i)
    {
        if (heightfield == mCollection->heightFields[i]->mInstance) {
            return mCollection->heightFields[i];
        }
    }
    NxuHeightFieldDesc* hf = new NxuHeightFieldDesc();
    hf->mInstance = heightfield;
    heightfield->saveToDesc(*hf);
    NxU32 size = heightfield->getNbRows()*heightfield->getNbColumns()*heightfield->getSampleStride();
    hf->samples = new NxU8[size];
    heightfield->saveCells(hf->samples, size);
    mCollection->heightFields.pushBack(hf);

    return hf;
}
#endif

NxuActorDesc* NxuPhysicsExport::AddActor(NxActor* actor)
{
    if (!actor) return NULL;
    NxuPhysicsModel* model = getCurrentModel();

    NxU32 count = model->mActors.size();
    for (NxU32 i=0;i<count;++i)
    {
        NxuActorDesc* desc = model->mActors[i];
        if (desc->mInstance == actor)
        {
            return desc;
        }
    }

    NxuActorDesc* aDesc = new NxuActorDesc();
    mCurrentActor = aDesc;
    aDesc->mInstance = actor;
    actor->saveToDesc(*aDesc);
    if (!actor->saveBodyToDesc(aDesc->mBody))
    {
        //Actor has no body
        aDesc->mBody.mass = -1;
    }
    NxU32 nbActorShapes	  =	actor->getNbShapes();
	NxShape	*const*	actorShapes	= actor->getShapes();

	for	(NxU32 k=0;	k<nbActorShapes; k++)
    {
       Write( actorShapes[k]);
    }
    mCurrentActor = NULL;
    model->mActors.pushBack(aDesc);
    return aDesc;
}

NxU32 NxuPhysicsExport::GetShapeIndex(NxuActorDesc* actor, NxShape* shape)
{
    NxU32 match = (NxU32)-1;
    for (NxU32 i=0;i<actor->shapes.size();++i)
    {
        if (((NxuShapeDesc*)actor->shapes[i])->mInstance == shape) 
        {
            match = i;
            break;
        }
    }

    return match;
}

    //SDK Streaming
void NxuPhysicsExport::WriteSDK(NxPhysicsSDK* sdk)
{
    Write(sdk); //Record parameters

	NxU32 scount = sdk->getNbScenes();
	for	(NxU32 i=0;	i<scount; i++)
	{
		NxScene	*scene = sdk->getScene(i);
        Write(scene);
    }
}

    //Model definition
void NxuPhysicsExport::BeginModel(const char* modelName)
{
    if (mCurrentModel)
    {
        EndModel();
    }

    NxuPhysicsModel* model = new NxuPhysicsModel();
    strncpy(model->name, modelName, NXU_STREAM_MAX_STRING_LENGTH-2);
    model->name[NXU_STREAM_MAX_STRING_LENGTH-1] = 0; //make sure its terminated
    mCollection->models.pushBack(model);
    mCurrentModel = model;
}

void NxuPhysicsExport::EndModel()
{
    mCurrentModel = 0;
    mCurrentActor = 0;
}

void NxuPhysicsExport::InstanceModel(const char* modelName, const NxMat34& pose)
{
    NxuPhysicsModel* model = getCurrentModel();
    NX_ASSERT(model); //Cant instance models to the global space

    NxuPhysicsModel* instancedModel = 0;
    for (NxU32 i =0; i<mCollection->models.size(); ++i)
    {
        if (!strcmp(mCollection->models[i]->name, modelName))
        {
            instancedModel = mCollection->models[i];
            break;
        }
    }

    NX_ASSERT(instancedModel != 0); //Can't instance an undefined model
    if (instancedModel == 0) return;
    NxuPhysicsModelInstance* instance = new NxuPhysicsModelInstance();
    instance->mModel = instancedModel;
    instance->mPose = pose;
    model->mModels.pushBack(instance);
}

    //Individual object streaming
void NxuPhysicsExport::Write(  const NxPhysicsSDK* physicsSDK)
{
    NX_ASSERT(physicsSDK);
    if (!physicsSDK) return;

    for	(int i=NX_PENALTY_FORCE; i<	NX_PARAMS_NUM_VALUES; i++)
	{
		NxParameter	p =	(NxParameter) i;
		NxReal v = physicsSDK->getParameter( p	);
		Write(	p, v );
	}
}

void NxuPhysicsExport::Write( NxParameter p,	NxReal v )
{
    NxuParameterData* param = new NxuParameterData();
    param->param = p;
    param->value = v;
    mCollection->sdkParams.pushBack(param);
}

void NxuPhysicsExport::Write( NxScene *scene )
{
    NxuSceneDesc* newScene = new NxuSceneDesc();
    newScene->mInstance = scene;
    scene->saveToDesc(*newScene);
    mCurrentModel = newScene->mModel = new NxuPhysicsModel();
    mCollection->scenes.pushBack(newScene);

    //Then add the contents of the scene to the model
    NxU32 acount = scene->getNbActors();

	if ( acount	)
	{
		NxActor	**actors = scene->getActors();
		for	(NxU32 j=0;	j<acount; j++)
		{
            Write(actors[j]);
        }
    }

    const NxU32 mcount = scene->getNbMaterials();

	if ( mcount )
	{
		NxMaterial **materials = new NxMaterial *[mcount];
		memset(materials, 0, sizeof(NxMaterial *)*mcount );
		NxU32 iterator = 0;
		NxU32 gcount = scene->getMaterialArray(	&materials[0], mcount, iterator );

		for	( NxU32 i	= 0; i < mcount;	++i	)
		{
            Write(materials[i]);
		}

		if(materials)
		{
			delete[] materials;
			materials=NULL;
		}
	}

	NxU32 pairCount	= scene->getNbPairs();
	if ( pairCount )
	{
		NxPairFlag *array = new NxPairFlag[pairCount];
		memset(array, 0, pairCount*sizeof(NxPairFlag));
		scene->getPairFlagArray( array,	pairCount );
		for	( NxU32 i	= 0; i < pairCount;	++i	)
		{
            Write(&array[i]);
		}

		if(array!=NULL)
		{
			delete[] array;
			array=NULL;
		}
	}

	// write joints
	NxU32 jointCount = scene->getNbJoints();
	if ( jointCount	)
	{
		scene->resetJointIterator();
		for	( NxU32 i	= 0; i < jointCount; ++i )
		{
			NxJoint	*j = scene->getNextJoint();
            Write(j);
		}
	}

	// now write effectors
	NxU32 effectorCount = scene->getNbEffectors();
	if ( effectorCount )
	{
		scene->resetEffectorIterator();
		for ( NxU32 i = 0; i < effectorCount; ++i )
		{
			NxEffector *e = scene->getNextEffector();
			Write(e);
		}
	}

#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
    //Fluids
    NxU32 fluidCount = scene->getNbFluids();
    if (fluidCount)
    {
        NxFluid** fluidList = scene->getFluids();
        for (NxU32 i=0;i <fluidCount;++i)
        {
            Write(fluidList[i]);
        }
    }
#endif
}

void NxuPhysicsExport::Write( const NxSceneDesc *scene	)
{
    //now create the scene containing that model
    NxuSceneDesc* newScene = new NxuSceneDesc();
    newScene->mInstance = 0;
    *((NxSceneDesc*)newScene) = *scene; //TODO: Explicit copy
    mCurrentModel = newScene->mModel = new NxuPhysicsModel();
    mCollection->scenes.pushBack(newScene);
}

void NxuPhysicsExport::Write(NxActor *a )
{
    AddActor(a);
}


void NxuPhysicsExport::Write( NxActorDesc *a, NxBodyDesc *b )
{
    NxuPhysicsModel* model = getCurrentModel();

    NxuActorDesc* aDesc = new NxuActorDesc();
    mCurrentActor = aDesc;
    aDesc->mInstance = NULL;
    //TODO: Implement assignement
    memcpy(aDesc, a, sizeof(NxActorDesc));
    memcpy(&(aDesc->mBody), b, sizeof(NxBodyDesc));

    model->mActors.pushBack(aDesc);    
}

void NxuPhysicsExport::Write( NxU32 pairflag, NxuActorDesc* actor0,  NxuActorDesc* actor1, NxU32 shapeIndex0, NxU32 shapeIndex1)
{
    NxuPhysicsModel* model = getCurrentModel();

    NxuPairFlagDesc* capsule = new NxuPairFlagDesc();
    capsule->mFlag = pairflag;
    capsule->mActor0 = actor0;
    capsule->mActor1 = actor1;
    capsule->mShapeIndex0 = shapeIndex0;
    capsule->mShapeIndex1 = shapeIndex1;

    model->mPairFlags.pushBack(capsule);
}

void NxuPhysicsExport::Write( NxPairFlag *a	)
{
    NxuActorDesc* actor0;
    NxuActorDesc* actor1;
    NxU32 shapeIndex0 = NxU32(-1);
    NxU32 shapeIndex1 = NxU32(-1);

	if ( a->isActorPair() )
	{
		NxActor *a0 = ( NxActor *) a->objects[0];
		NxActor *a1 = ( NxActor *) a->objects[1];
		actor0 = AddActor(a0);
		actor1 = AddActor(a1);
		assert(	actor0 );
		assert(	actor1 );
	}
	else
	{
		NxShape *s0 = ( NxShape *) a->objects[0];
		NxShape *s1 = ( NxShape *) a->objects[1];

		NxActor *a0 = &s0->getActor();
		NxActor *a1 = &s1->getActor();

		actor0 = AddActor(a0);
		actor1 = AddActor(a1);

  	    shapeIndex0 = GetShapeIndex(actor0,s0);
		shapeIndex1 = GetShapeIndex(actor1,s1);

		assert(actor0);	
		assert(actor1);
		assert(shapeIndex0>=0);
		assert(shapeIndex1>=0);
	}
    
    Write( NxU32(a->flags), actor0, actor1, shapeIndex0, shapeIndex1);
}

void NxuPhysicsExport::Write( NxMaterial *a)
{
    if(!a) return;
    NxuPhysicsModel* model = getCurrentModel();
    NxuMaterialDesc* desc = new NxuMaterialDesc;
    a->saveToDesc(*desc);
    desc->index = a->getMaterialIndex();
    model->mMaterials.pushBack(desc);
}


void NxuPhysicsExport::Write( NxMaterialDesc *a, NxMaterialIndex index	)
{
    NxuPhysicsModel* model = getCurrentModel();
    NxuMaterialDesc* desc = new NxuMaterialDesc;
    //TODO: write assignment
    memcpy(desc, a, sizeof(NxMaterialDesc));
    desc->index = index;
    model->mMaterials.pushBack(desc);
}

void NxuPhysicsExport::Write( NxJoint *j )
{
    NxuJointDesc* jDesc;

	NxJointType	type = j->getType();

	switch ( type )
	{
		case NX_JOINT_PRISMATIC:
		{
			NxPrismaticJoint *jt = j->isPrismaticJoint();
			assert(jt);
			NxuPrismaticJointDesc* d = new NxuPrismaticJointDesc();
			jt->saveToDesc(*d);
            jDesc = d;
		}
		break;

		case NX_JOINT_REVOLUTE:
		{
			NxRevoluteJoint	*jt	= j->isRevoluteJoint();
			assert(jt);
			NxuRevoluteJointDesc *d = new NxuRevoluteJointDesc();
			jt->saveToDesc(*d);
            jDesc = d;
		}
		break;

		case NX_JOINT_CYLINDRICAL:
		{
			NxCylindricalJoint *jt = j->isCylindricalJoint();
			assert(jt);
			NxuCylindricalJointDesc *d = new NxuCylindricalJointDesc();
			jt->saveToDesc(*d);
            jDesc = d;
		}
		break;

		case NX_JOINT_SPHERICAL:
		{
			NxSphericalJoint *jt = j->isSphericalJoint();
			assert(jt);
			NxuSphericalJointDesc *d = new NxuSphericalJointDesc();
			jt->saveToDesc(*d);
            jDesc = d;
		}
		break;

		case NX_JOINT_POINT_ON_LINE:
		{
			NxPointOnLineJoint *jt = j->isPointOnLineJoint();
			assert(jt);
			NxuPointOnLineJointDesc *d = new NxuPointOnLineJointDesc();
			jt->saveToDesc(*d);
            jDesc = d;
		}
		break;

		case NX_JOINT_POINT_IN_PLANE:
		{
			NxPointInPlaneJoint	*jt	= j->isPointInPlaneJoint();
			assert(jt);
			NxuPointInPlaneJointDesc *d = new NxuPointInPlaneJointDesc();
			jt->saveToDesc(*d);
            jDesc = d;
		}
		break;

		case NX_JOINT_DISTANCE:
		{
			NxDistanceJoint	*jt	= j->isDistanceJoint();
			assert(jt);
			NxuDistanceJointDesc *d = new NxuDistanceJointDesc();
			jt->saveToDesc(*d);
            jDesc = d;
		}
		break;

		case NX_JOINT_PULLEY:
		{
			NxPulleyJoint *jt =	j->isPulleyJoint();
			assert(jt);
			NxuPulleyJointDesc *d = new NxuPulleyJointDesc();
			jt->saveToDesc(*d);
            jDesc = d;
		}
		break;

		case NX_JOINT_FIXED:
		{
			NxFixedJoint *jt = j->isFixedJoint();
			assert(jt);
			NxuFixedJointDesc *d = new NxuFixedJointDesc();
			jt->saveToDesc(*d);
            jDesc = d;
		}
		break;

		case NX_JOINT_D6:
		{
			NxD6Joint *jt =	j->isD6Joint();
			assert(jt);
			NxuD6JointDesc *d = new NxuD6JointDesc();
			jt->saveToDesc(*d);
            jDesc = d;   
		}
		break;

		default:
			assert(	false );
		break;
	}		 

    //Add Limits
    	// in addition, we also have to write out its limit planes!
	j->resetLimitPlaneIterator();
	if ( j->hasMoreLimitPlanes() )
	{
		// write limit point
		jDesc->mOnActor2 = j->getLimitPoint( jDesc->mPlaneLimitPoint );

		// write the plane normals
		while ( j->hasMoreLimitPlanes() )
		{
            NxuPlaneInfo* pInfo = new NxuPlaneInfo();
			j->getNextLimitPlane( pInfo->mPlaneNormal, pInfo->mPlaneD );

			// now determine the world limit point
			// note that planeD = -(planeNormal) DOT (worldLimitPt)
			// try choosing x,y = 0 for world limit pt, which makes it
			// worldLimitPt.z = planeD / -planeNormal.z
			if ( fabs(pInfo->mPlaneNormal.z) > 0.001f )
			{
				pInfo->mWorldLimitPt.x = jDesc->mPlaneLimitPoint.x;
				pInfo->mWorldLimitPt.y = jDesc->mPlaneLimitPoint.y;
				pInfo->mWorldLimitPt.z = -pInfo->mPlaneD / pInfo->mPlaneNormal.z;
			}
			// k, that didn't work - try x,z = 0
			else if ( fabs(pInfo->mPlaneNormal.y) > 0.001f )
			{
				pInfo->mWorldLimitPt.x = jDesc->mPlaneLimitPoint.x;
				pInfo->mWorldLimitPt.z = jDesc->mPlaneLimitPoint.z;
				pInfo->mWorldLimitPt.y = -pInfo->mPlaneD / pInfo->mPlaneNormal.y;
			}
			else if ( fabs(pInfo->mPlaneNormal.x) > 0.001f )
			{
				pInfo->mWorldLimitPt.y = jDesc->mPlaneLimitPoint.y;
				pInfo->mWorldLimitPt.z = jDesc->mPlaneLimitPoint.z;
				pInfo->mWorldLimitPt.x = -pInfo->mPlaneD / pInfo->mPlaneNormal.x;
			}
			else
			{
				assert( false );
			}
            jDesc->mNxuPlaneInfo.pushBack(pInfo);
		}
	}

    NxActor* actor1;
    NxActor* actor2;
    j->getActors(&actor1, &actor2);
    jDesc->mRefAttachActorDesc = AddActor(actor1);
    jDesc->mAttachActorDesc = AddActor(actor2);

    NxuPhysicsModel* model = getCurrentModel();

    jDesc->mInstance = j;
    model->mJoints.pushBack(jDesc);
}


void NxuPhysicsExport::Write(	NxShape	*shape )
{
    NxuShapeDesc* sDesc = NULL;
    NxShapeType type = shape->getType();
	switch ( type )
	{
		case NX_SHAPE_PLANE:
		{
			const NxPlaneShape *current = shape->isPlane();
			assert(current);
			NxuPlaneShapeDesc* Desc = new NxuPlaneShapeDesc();
			current->saveToDesc(*Desc);
            Desc->ccdSkeleton = current->getCCDSkeleton();
            sDesc = Desc;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
			if ( Desc->ccdSkeleton )	
            {
				 sDesc->mCCDSkeleton = AddSkeleton(Desc->ccdSkeleton );
            }
#endif
		}
		break;
		case NX_SHAPE_SPHERE:
		{
			const NxSphereShape *current = shape->isSphere();
			NxuSphereShapeDesc* Desc = new NxuSphereShapeDesc();
			assert(	current	);
			current->saveToDesc(*Desc);
            Desc->ccdSkeleton = current->getCCDSkeleton();
            sDesc = Desc;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
			if ( Desc->ccdSkeleton )	
            {
				 sDesc->mCCDSkeleton = AddSkeleton(Desc->ccdSkeleton );
            }
#endif
		}
			break;
		case NX_SHAPE_BOX:
		{
			const NxBoxShape *current = shape->isBox();
			NxuBoxShapeDesc* Desc = new NxuBoxShapeDesc();
			assert(	current	);
			current->saveToDesc(*Desc);
            Desc->ccdSkeleton = current->getCCDSkeleton();
            sDesc = Desc;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
			if ( Desc->ccdSkeleton )	
            {
				 sDesc->mCCDSkeleton = AddSkeleton(Desc->ccdSkeleton );
            }
#endif
		}
			break;
		case NX_SHAPE_CAPSULE:
		{
			const NxCapsuleShape *current = shape->isCapsule();
			NxuCapsuleShapeDesc* Desc = new NxuCapsuleShapeDesc();
			assert(	current	);
			current->saveToDesc(*Desc);
            Desc->ccdSkeleton = current->getCCDSkeleton();
            sDesc = Desc;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
			if ( Desc->ccdSkeleton )	
            {
				 sDesc->mCCDSkeleton = AddSkeleton(Desc->ccdSkeleton );
            }
#endif
		}
			break;
		case NX_SHAPE_CONVEX:
		{
			const NxConvexShape *current = shape->isConvexMesh();
			NxuConvexShapeDesc* Desc = new NxuConvexShapeDesc();
			assert(	current	);
			current->saveToDesc(*Desc);
            Desc->ccdSkeleton = current->getCCDSkeleton();
            sDesc = Desc;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
			if ( Desc->ccdSkeleton )	
            {
				 sDesc->mCCDSkeleton = AddSkeleton(Desc->ccdSkeleton );
            }
#endif
			if ( Desc->meshData )	
            {
				Desc->mConvexMeshDesc =  AddConvex(Desc->meshData);
            }
		}
		break;

#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
		// note that in the earlier versions of the SDK, NX_SHAPE_WHEEL did not exist
		case NX_SHAPE_WHEEL:
		{
			const NxWheelShape *current = (const NxWheelShape *)shape->is( NX_SHAPE_WHEEL );
			NxuWheelShapeDesc* Desc = new NxuWheelShapeDesc();
			assert( current );
			current->saveToDesc(*Desc);
            sDesc = Desc;
		}
		break;
#endif
		case NX_SHAPE_MESH:
		{
			NxTriangleMeshShape *current	 = shape->isTriangleMesh();
			NxuTriangleMeshShapeDesc*	Desc = new NxuTriangleMeshShapeDesc();
			assert(	current	);
			current->saveToDesc(*Desc);
            Desc->ccdSkeleton = current->getCCDSkeleton();
            NxTriangleMesh & trimesh = current->getTriangleMesh();
            Desc->meshData = &trimesh;
            sDesc = Desc;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
			if ( Desc->ccdSkeleton )	
            {
				 sDesc->mCCDSkeleton = AddSkeleton(Desc->ccdSkeleton );
            }
#endif
			if ( Desc->meshData )	
            {
				Desc->mTriangleMeshDesc = AddTriangleMesh(Desc->meshData);
            }
		}
        break;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
		case NX_SHAPE_HEIGHTFIELD:
		{
			//const NxHeightFieldShape *current	 = shape->isHeightField();
            const NxHeightFieldShape *current	 = (NxHeightFieldShape*) shape->is(NX_SHAPE_HEIGHTFIELD);
			NxuHeightFieldShapeDesc*	Desc = new NxuHeightFieldShapeDesc();
			assert(	current	);
			current->saveToDesc(*Desc);
            Desc->ccdSkeleton = current->getCCDSkeleton();
            sDesc = Desc;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
			if ( Desc->ccdSkeleton )	
            {
				 sDesc->mCCDSkeleton = AddSkeleton(Desc->ccdSkeleton );
            }
#endif
			if ( Desc->heightField )	
            {
				Desc->mHeightFieldDesc = AddHeightField(Desc->heightField);
            }
		}
        break;
#endif
        default:
            NX_ASSERT(0); //Unknown shape type
            return;
    }

    sDesc->mInstance = shape;
    getCurrentActor()->shapes.pushBack(sDesc->getShapeDesc());
}

void NxuPhysicsExport::Write( const	NxShapeDesc	*d	)
{
    NxuShapeDesc* sDesc;

	switch ( d->getType() )
	{
		case NX_SHAPE_PLANE:
		{
			NxuPlaneShapeDesc* Desc = new NxuPlaneShapeDesc();
            sDesc = Desc;
            NxPlaneShapeDesc* pDesc = (NxPlaneShapeDesc*)(d);
            //TODO: Implement assignment
            //*((NxPlaneShapeDesc*)Desc) = *pDesc; //Shallow copy since we handle pointer values separately anyway
		    memcpy(Desc, pDesc, sizeof(NxPlaneShapeDesc));
        
        }
		break;
		case NX_SHAPE_SPHERE:
		{
			NxuSphereShapeDesc* Desc = new NxuSphereShapeDesc();
            sDesc = Desc;
            NxSphereShapeDesc* pDesc = (NxSphereShapeDesc*)(d);
           // *((NxSphereShapeDesc*)Desc) = *pDesc; //Shallow copy since we handle pointer values separately anyway
		    memcpy(Desc, pDesc, sizeof(NxSphereShapeDesc));
        }
			break;
		case NX_SHAPE_BOX:
		{
			NxuBoxShapeDesc* Desc = new NxuBoxShapeDesc();
            sDesc = Desc;
            NxBoxShapeDesc* pDesc = (NxBoxShapeDesc*)(d);
           // *((NxBoxShapeDesc*)Desc) = *pDesc; //Shallow copy since we handle pointer values separately anyway
            memcpy(Desc, pDesc, sizeof(NxBoxShapeDesc));
		}
			break;
		case NX_SHAPE_CAPSULE:
		{
			NxuCapsuleShapeDesc* Desc = new NxuCapsuleShapeDesc();
            sDesc = Desc;
            NxCapsuleShapeDesc* pDesc = (NxCapsuleShapeDesc*)(&d);
            //*((NxCapsuleShapeDesc*)Desc) = *pDesc; //Shallow copy since we handle pointer values separately anyway
            memcpy(Desc, pDesc, sizeof(NxCapsuleShapeDesc));
		}
			break;
		case NX_SHAPE_CONVEX:
		{
			NxuConvexShapeDesc* Desc = new NxuConvexShapeDesc();
            sDesc = Desc;
            NxConvexShapeDesc* pDesc = (NxConvexShapeDesc*)(&d);
            //*((NxConvexShapeDesc*)Desc) = *pDesc; //Shallow copy since we handle pointer values separately anyway
            memcpy(Desc, pDesc, sizeof(NxConvexShapeDesc));

			if ( pDesc->meshData )	
            {
				Desc->mConvexMeshDesc = (NxuConvexMeshDesc*)AddConvex(pDesc->meshData);
            }
		}
		break;

#if NXU_SDK_FEATURES & NXU_SDK_HAS_WHEEL
		// note that in the earlier versions of the SDK, NX_SHAPE_WHEEL did not exist
		case NX_SHAPE_WHEEL:
		{
			NxuWheelShapeDesc* Desc = new NxuWheelShapeDesc();
            sDesc = Desc;
            NxWheelShapeDesc* pDesc = (NxWheelShapeDesc*)(&d);
            //*((NxWheelShapeDesc*)Desc) = *pDesc; //Shallow copy since we handle pointer values separately anyway
            memcpy(Desc, pDesc, sizeof(NxWheelShapeDesc));
		}
		break;
#endif
		case NX_SHAPE_MESH:
		{
			NxuTriangleMeshShapeDesc*	Desc = new NxuTriangleMeshShapeDesc();
            sDesc = Desc;
            NxTriangleMeshShapeDesc* pDesc = (NxTriangleMeshShapeDesc*)(&d);
            //*((NxTriangleMeshShapeDesc*)Desc) = *pDesc; //Shallow copy since we handle pointer values separately anyway
            memcpy(Desc, pDesc, sizeof(NxTriangleMeshShapeDesc));

			if ( pDesc->meshData )	
            {
				Desc->mTriangleMeshDesc = (NxuTriangleMeshDesc*) AddTriangleMesh(pDesc->meshData);
            }
		}
        break;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
		case NX_SHAPE_HEIGHTFIELD:
		{
			NxuHeightFieldShapeDesc*	Desc = new NxuHeightFieldShapeDesc();
            sDesc = Desc;
            NxHeightFieldShapeDesc* pDesc = (NxHeightFieldShapeDesc*)(&d);
            //*((NxTriangleMeshShapeDesc*)Desc) = *pDesc; //Shallow copy since we handle pointer values separately anyway
            memcpy(Desc, pDesc, sizeof(NxHeightFieldShapeDesc));

			if ( pDesc->heightField )	
            {
				Desc->mHeightFieldDesc = (NxuHeightFieldDesc*) AddHeightField(pDesc->heightField);
            }
		}
        break;
#endif
    }
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
			if ( d->ccdSkeleton )	
            {
				 sDesc->mCCDSkeleton = (NxuCCDSkeletonDesc*) AddSkeleton(d->ccdSkeleton );
            }
#endif
    sDesc->mInstance = 0;
    getCurrentActor()->shapes.pushBack(sDesc->getShapeDesc());
}

void NxuPhysicsExport::Write(	NxTriangleMesh *mesh )
{
    AddTriangleMesh(mesh);
}

void NxuPhysicsExport::Write(	NxConvexMesh *mesh )
{
    AddConvex(mesh);
}

#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
void NxuPhysicsExport::Write(	NxHeightField *hf )
{
    AddHeightField(hf);
}
#endif

#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS

void NxuPhysicsExport::Write( 	NxCCDSkeleton *skeleton	)
{
    AddSkeleton(skeleton);
}

#endif

void NxuPhysicsExport::Write( NxEffector *e )
{
	NxSpringAndDamperEffector *sade = e->isSpringAndDamperEffector();
    if (sade)
    {
        NxuSpringAndDamperEffectorDesc* effector = new NxuSpringAndDamperEffectorDesc();

		effector->setToDefault();
		// get the effector description
#if NXU_SDK_FEATURES & NXU_SDK_HAS_EFFECTOR_DESC
		sade->saveToDesc( *effector );
#else
#pragma message("There was not saveToDesc method in 2.2, we need to do this manually")
#endif

		// get the actors from the pntrs
		int actorIndex1 = -1, actorIndex2 = -1;
		if ( effector->body1 )
		{
			effector->mRefAttachActorDesc = AddActor( effector->body1 );
            effector->pos1 = effector->body1->getGlobalPosition();
		}
		if ( effector->body2 )
		{
			effector->mAttachActorDesc = AddActor( effector->body2 );
            effector->pos2 = effector->body2->getGlobalPosition();
		}
        effector->mInstance = sade;
        NxuPhysicsModel* model = getCurrentModel();

        model->mEffectors.pushBack(effector);
    }
    else
    {
        assert(false); //No other effectors supported
    }
}

#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
void NxuPhysicsExport::Write( NxFluid *fluid)
{
    NxuFluidDesc* desc = new NxuFluidDesc();
    fluid->saveToDesc(*desc);
    //Save the emitters
    NxU32 count = fluid->getNbEmitters();
    NxFluidEmitter** emitters = fluid->getEmitters();
    for (NxU32 i=0;i<count;++i)
    {
		NxuFluidEmitterDesc* emitterDesc = new NxuFluidEmitterDesc();
		emitters[i]->saveToDesc(*emitterDesc);
        emitterDesc->mInstance = emitters[i];
        emitterDesc->mFrameActor = AddActor(emitters[i]->getFrameActor());
		desc->mEmitters.pushBack(emitterDesc);
    }
    //TODO: Does this need a deep copy? 
    desc->initialParticleData = fluid->getParticlesWriteData();

    getCurrentModel()->mFluids.pushBack(desc);
}
#endif

void NxuPhysicsExport::Write(NxuTriangleMeshDesc* d)
{
    NxU32 count = mCollection->triangleMeshes.size();
    for (NxU32 i=0;i<count;++i)
    {
        if (d == mCollection->triangleMeshes[i]) return;
    }
    mCollection->triangleMeshes.pushBack(d);
}

void NxuPhysicsExport::Write(NxuConvexMeshDesc* d)
{
    NxU32 count = mCollection->convexMeshes.size();
    for (NxU32 i=0;i<count;++i)
    {
        if (d == mCollection->convexMeshes[i]) return;
    }
    mCollection->convexMeshes.pushBack(d);
}

#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
void NxuPhysicsExport::Write(NxuHeightFieldDesc* d)
{
    NxU32 count = mCollection->heightFields.size();
    for (NxU32 i=0;i<count;++i)
    {
        if (d == mCollection->heightFields[i]) return;
    }
    mCollection->heightFields.pushBack(d);
}
#endif

#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
void NxuPhysicsExport::Write(NxuCCDSkeletonDesc* d)
{
    NxU32 count = mCollection->skeletons.size();
    for (NxU32 i=0;i<count;++i)
    {
        if (d == mCollection->skeletons[i]) return;
    }
    mCollection->skeletons.pushBack(d);
}
#endif
void NxuPhysicsExport::Write(NxuMaterialDesc* d)
{
    NxuPhysicsModel* model = getCurrentModel();
    model->mMaterials.pushBack(d);
}

void NxuPhysicsExport::Write(NxuShapeDesc* d)
{
    NxuActorDesc* actor = getCurrentActor();

    NxU32 count = actor->shapes.size();
    for (NxU32 i=0;i<count;++i)
    {
        if (d == (NxuShapeDesc*)(actor->shapes[i])) return;
    }

    actor->shapes.pushBack(d->getShapeDesc());
}

void NxuPhysicsExport::Write(NxuActorDesc* d)
{
    NxuPhysicsModel* model = getCurrentModel();

    NxU32 count = model->mActors.size();
    for (NxU32 i=0;i<count;++i)
    {
        if (d == model->mActors[i]) return;
    }

    count = d->shapes.size();
    for (NxU32 i=0;i<count;++i)
    {
        NxuShapeDesc* shape = (NxuShapeDesc*)d->shapes[i];
        NxShapeDesc* desc = shape->getShapeDesc();
        if (desc->getType() == NX_SHAPE_CONVEX)
        {
            NxuConvexShapeDesc* convexShape = (NxuConvexShapeDesc*)shape;
            Write(convexShape->mConvexMeshDesc);
        }
        else if (desc->getType() == NX_SHAPE_MESH)
        {
            NxuTriangleMeshShapeDesc* trimeshShape = (NxuTriangleMeshShapeDesc*)shape;
            Write(trimeshShape->mTriangleMeshDesc);
        }
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
        else if (desc->getType() == NX_SHAPE_HEIGHTFIELD)
        {
            NxuHeightFieldShapeDesc* heightFieldShape = (NxuHeightFieldShapeDesc*)shape;
            Write(heightFieldShape->mHeightFieldDesc);
        }
#endif
        Write(shape->mCCDSkeleton);
    }
    model->mActors.pushBack(d);
}

void NxuPhysicsExport::Write(NxuJointDesc* d)
{
    NxuPhysicsModel* model = getCurrentModel();

    Write(d->mRefAttachActorDesc);
    Write(d->mAttachActorDesc);

    model->mJoints.pushBack(d);
}

void NxuPhysicsExport::Write(NxuSpringAndDamperEffectorDesc* d)
{
    NxuPhysicsModel* model = getCurrentModel();

    Write(d->mRefAttachActorDesc);
    Write(d->mAttachActorDesc);

    model->mEffectors.pushBack(d);
}

void NxuPhysicsExport::Write(NxuPairFlagDesc* d)
{
    NxuPhysicsModel* model = getCurrentModel();

    Write(d->mActor0);
    Write(d->mActor1);

    model->mPairFlags.pushBack(d);
}

#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
void NxuPhysicsExport::Write(NxuFluidDesc* d)
{
    NxuPhysicsModel* model = getCurrentModel();

    NxU32 count = d->emitters.size();
    for (NxU32 i=0;i<count;++i)
    {
        NxFluidEmitterDesc & emit = d->emitters[i];
        NxuFluidEmitterDesc* desc = (NxuFluidEmitterDesc*)&emit;
        Write(desc->mFrameActor);
    }
    d->initialParticleData.numParticlesPtr = &d->mNumParticles;
    //Deprecated
    //if (d->surfaceMeshDesc)
    //{
    //   d->surfaceMeshDesc->meshData.numVerticesPtr = &d->mImplicitVerts;
    //   d->surfaceMeshDesc->meshData.numTrianglesPtr = &d->mImplicitIndexes;
    //   d->surfaceMeshDesc->meshData.numParentIndicesPtr = &d->mImplicitParents;
    //}

    model->mFluids.pushBack(d);  
}
#endif

void NxuPhysicsExport::Write(NxuPhysicsModel* d)
{
    NxU32 count = mCollection->models.size();
    for (NxU32 i=0;i<count;++i)
    {
        if (d == mCollection->models[i]) return;
    }

    count = d->mModels.size();
    for (NxU32 i=0;i<count;++i)
    {
        NxuPhysicsModel* model = d->mModels[i]->mModel;
        Write(model);
    }
    mCollection->models.pushBack(d);
}

void NxuPhysicsExport::Write(NxuSceneDesc* d)
{

    NxU32 count = d->mModel->mModels.size();
    for (NxU32 i=0;i<count;++i)
    {
        NxuPhysicsModel* model = d->mModel->mModels[i]->mModel;
        Write(model);
    }

    mCollection->scenes.pushBack(d);
}


//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND
