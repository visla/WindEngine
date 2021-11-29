/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		NxuPhysicsImport.cpp

		This is the common import implementation that contains format independent import logic.

		Changelist
		----------

*/

#include <stdio.h>

#include "NxuPhysicsImport.h"
#include "NxCooking.h"

#include "NxStream.h"



NxPhysicsSDK *NxuPhysicsImport::SDK = NULL;



ImportMemoryWriteBuffer::ImportMemoryWriteBuffer() : currentSize(0), maxSize(0), data(NULL)
	{
	}

ImportMemoryWriteBuffer::~ImportMemoryWriteBuffer()
	{
		if(data!=NULL)
		{
			delete[] data;
			data=NULL;
		}
	}

void ImportMemoryWriteBuffer::clear()
	{
	currentSize = 0;
	}

NxStream& ImportMemoryWriteBuffer::storeByte(NxU8 b)
	{
	storeBuffer(&b, sizeof(NxU8));
	return *this;
	}
NxStream& ImportMemoryWriteBuffer::storeWord(NxU16 w)
	{
	storeBuffer(&w, sizeof(NxU16));
	return *this;
	}
NxStream& ImportMemoryWriteBuffer::storeDword(NxU32 d)
	{
	storeBuffer(&d, sizeof(NxU32));
	return *this;
	}
NxStream& ImportMemoryWriteBuffer::storeFloat(NxReal f)
	{
	storeBuffer(&f, sizeof(NxReal));
	return *this;
	}
NxStream& ImportMemoryWriteBuffer::storeDouble(NxF64 f)
	{
	storeBuffer(&f, sizeof(NxF64));
	return *this;
	}
NxStream& ImportMemoryWriteBuffer::storeBuffer(const void* buffer, NxU32 size)
	{
	NxU32 expectedSize = currentSize + size;
	if(expectedSize > maxSize)
		{
		maxSize = expectedSize + 4096;

		NxU8* newData = new NxU8[maxSize];
		NX_ASSERT(newData!=NULL);

		if(data)
			{
			memcpy(newData, data, currentSize);
			delete[] data;
			}
		data = newData;
		}
	memcpy(data+currentSize, buffer, size);
	currentSize += size;
	return *this;
	}


ImportMemoryReadBuffer::ImportMemoryReadBuffer(const NxU8* data) : buffer(data)
	{
	}

ImportMemoryReadBuffer::~ImportMemoryReadBuffer()
	{
	// We don't own the data => no delete
	}

NxU8 ImportMemoryReadBuffer::readByte() const
	{
	NxU8 b;
	memcpy(&b, buffer, sizeof(NxU8));
	buffer += sizeof(NxU8);
	return b;
	}

NxU16 ImportMemoryReadBuffer::readWord() const
	{
	NxU16 w;
	memcpy(&w, buffer, sizeof(NxU16));
	buffer += sizeof(NxU16);
	return w;
	}

NxU32 ImportMemoryReadBuffer::readDword() const
	{
	NxU32 d;
	memcpy(&d, buffer, sizeof(NxU32));
	buffer += sizeof(NxU32);
	return d;
	}

float ImportMemoryReadBuffer::readFloat() const
	{
	float f;
	memcpy(&f, buffer, sizeof(float));
	buffer += sizeof(float);
	return f;
	}

double ImportMemoryReadBuffer::readDouble() const
	{
	double f;
	memcpy(&f, buffer, sizeof(double));
	buffer += sizeof(double);
	return f;
	}

void ImportMemoryReadBuffer::readBuffer(void* dest, NxU32 size) const
	{
	memcpy(dest, buffer, size);
	buffer += size;
	}


NxuPhysicsImport::NxuPhysicsImport(NxPhysicsSDK *sdk)
{
	SDK = sdk;
    mCollection = new NxuPhysicsCollection();
}

NxuPhysicsImport::~NxuPhysicsImport()
{
    if (mCollection) delete mCollection;
}

void NxuPhysicsImport::instanceSkeletons(NxPhysicsSDK & sdk)
{
	// now load	up any tri meshes
	NxU32 size = mCollection->skeletons.size();
	for	(NxU32 i	= 0; i < size; ++i )
	{
		NxCCDSkeleton *skel = 0;

		NxuCCDSkeletonDesc *sd = mCollection->skeletons[i];
		if ( sd )
		{
			// k, do we need to cook the mesh?
			if ( !sd->mCookedData )
			{
				skel = sdk.createCCDSkeleton( *sd );
			}
			else
			{
				skel = sdk.createCCDSkeleton( sd->mCookedData, sd->mCookedDataSize);
			}
            sd->mInstance = skel;
		}
	}
}

void NxuPhysicsImport::instanceConvexes(NxPhysicsSDK & sdk)
{
	// now load	up any tri meshes
	NxU32 convexSize = mCollection->convexMeshes.size();
	for	(NxU32 i	= 0; i < convexSize; ++i )
	{
		NxConvexMesh *cm = 0;

		NxuConvexMeshDesc *cmd = mCollection->convexMeshes[i];
		if ( cmd )
		{
			// k, do we need to cook the mesh?
			if ( !cmd->mCookedData )
			{
				bool status	= true;
				NxInitCooking();
                ImportMemoryWriteBuffer writeBuffer;
 				status = NxCookConvexMesh( *cmd, writeBuffer );
                if ( status	)
				{
					cm = sdk.createConvexMesh( ImportMemoryReadBuffer(writeBuffer.data)	);
                }
			}
			else
			{
                ImportMemoryReadBuffer readBuffer((const NxU8*)cmd->mCookedData);
                cm = sdk.createConvexMesh( readBuffer);
			}
            cmd->mInstance = cm;
		}
	}
}


void NxuPhysicsImport::instanceTrimeshs(NxPhysicsSDK & sdk)
{
	// now load	up any tri meshes
	NxU32 triSize = mCollection->triangleMeshes.size();
	for	(NxU32 i	= 0; i < triSize; ++i )
	{
		NxTriangleMesh *tm = 0;

		NxuTriangleMeshDesc *tmd = mCollection->triangleMeshes[i];
		if ( tmd )
		{
			// k, do we need to cook the triangle mesh?
			if ( !tmd->mCookedData )
			{
				tmd->pmap = 0;

				bool status	= true;
				NxInitCooking();
                ImportMemoryWriteBuffer writeBuffer;
 				status = NxCookTriangleMesh( *tmd, writeBuffer );
                if ( status	)
				{
					tm = sdk.createTriangleMesh( ImportMemoryReadBuffer(writeBuffer.data)	);
                }
			}
			else
			{
                ImportMemoryReadBuffer readBuffer((const NxU8*)tmd->mCookedData);
                tm = sdk.createTriangleMesh( readBuffer);
			}
            if (tmd->mPmapSize) 
            {
				tmd->pmap = new NxPMap;
                tmd->pmap->data = tmd->mPmap;
                tmd->pmap->dataSize = tmd->mPmapSize;
				bool ok = tm->loadPMap( *tmd->pmap );
			}
            tmd->mInstance = tm;
		}
	}
}

void NxuPhysicsImport::instanceHeightFields(NxPhysicsSDK & sdk)
{
	NxU32 size = mCollection->heightFields.size();
	for	(NxU32 i	= 0; i < size; ++i )
	{
		NxuHeightFieldDesc *hfd = mCollection->heightFields[i];
		if ( hfd )
		{
            hfd->mInstance = sdk.createHeightField(*hfd);
		}
	}
}

void NxuPhysicsImport::instanceModel(NxScene & scene, NxuPhysicsModel & model, NxMat34 & pose, NxuUserNotify* callback)
{
    NxU32 count = model.mModels.size();
    for (NxU32 i=0;i<count;++i)
    {
        NxuPhysicsModelInstance* instance = model.mModels[i];
        NxMat34 xform;
        xform.multiply(pose, instance->mPose);
        instanceModel(scene, *(instance->mModel), xform, callback);
    }

    count = model.mMaterials.size();
    for (NxU32 i=0;i<count;++i)
    {
        NxuMaterialDesc* mat = model.mMaterials[i];
        mat->mInstance = 0;
        while ((mat->mInstance = scene.getMaterialFromIndex(mat->index)) == NULL)
        {
            //Create materials until we have one at the correct index
            scene.createMaterial(*mat);
        }
        if (mat->mInstance) {
            mat->mInstance->loadFromDesc(*mat);
        }
    }

    count = model.mActors.size();
    for (NxU32 i=0;i<count;++i)
    {
        NxuActorDesc* actor = model.mActors[i];
        for (NxU32 j=0;j<actor->shapes.size();j++)
        {
            NxShapeDesc* shape = actor->shapes[j];
            NxuShapeDesc* s;
            switch(shape->getType())
            {
            case NX_SHAPE_PLANE:
                {
                    s = (NxuPlaneShapeDesc*)shape;
                }
            break;
            case NX_SHAPE_SPHERE:
                {
                    s = (NxuSphereShapeDesc*)shape;
                }
                break;
            case NX_SHAPE_BOX:
                {
                    s = (NxuBoxShapeDesc*)shape;
                }
                break;
            case NX_SHAPE_CAPSULE:
                {
                    s = (NxuCapsuleShapeDesc*)shape;
                }
                break;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_WHEEL
            case NX_SHAPE_WHEEL:
                {
                    s = (NxuWheelShapeDesc*)shape;
                }
                break;
#endif
            case NX_SHAPE_CONVEX:
                {
                NxuConvexShapeDesc* csd = (NxuConvexShapeDesc*)shape;
                csd->meshData = csd->mConvexMeshDesc->mInstance;
                s = csd;
                break;
                }
            case NX_SHAPE_MESH:
                {
                NxuTriangleMeshShapeDesc* tmd = (NxuTriangleMeshShapeDesc*)shape;
                tmd->meshData = tmd->mTriangleMeshDesc->mInstance;
                s = tmd;
                break;
                }
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
            case NX_SHAPE_HEIGHTFIELD:
                {
                NxuHeightFieldShapeDesc* hd = (NxuHeightFieldShapeDesc*)shape;
                hd->heightField = hd->mHeightFieldDesc->mInstance;
                s = hd;
                break;
                }
#endif
            default:
                NX_ASSERT(false); //Unknown shape type
            }
            if (s->mCCDSkeleton)
            {
                shape->ccdSkeleton = s->mCCDSkeleton->mInstance;
            }
        }
        actor->mInstance = scene.createActor(*(actor));
		NxMat34 temp = actor->mInstance->getGlobalPose();
		temp.multiply(pose, temp);
		actor->mInstance->setGlobalPose(temp);
        if (callback && actor->mInstance )
        {
            callback->NotifyActor(actor->mInstance);
        }
    }


    count = model.mPairFlags.size();
    for (NxU32 i=0;i<count;++i)
    {
        NxuPairFlagDesc * d = model.mPairFlags[i];
        if (d->mShapeIndex0 == (NxU32)-1 && d->mShapeIndex1 == (NxU32)-1)
        {
            scene.setActorPairFlags(*(d->mActor0->mInstance), *(d->mActor1->mInstance), d->mFlag);
        }
        else
        {
            NxShape*const* s0 = d->mActor0->mInstance->getShapes();
            NxShape*const* s1 = d->mActor1->mInstance->getShapes();
            scene.setShapePairFlags(*(s0[d->mShapeIndex0]), *(s1[d->mShapeIndex1]), d->mFlag);
        }
    }

    count = model.mJoints.size();
    for (NxU32 i=0;i<count;++i)
    {
        NxuJointDesc* joint = model.mJoints[i];
        NxJointDesc* jdesc = joint->getDesc();
        jdesc->actor[0] = joint->mRefAttachActorDesc ? joint->mRefAttachActorDesc->mInstance : 0;
        jdesc->actor[1] = joint->mAttachActorDesc ? joint->mAttachActorDesc->mInstance : 0;
        joint->mInstance = scene.createJoint(*(jdesc));
        if (!joint->mInstance) continue;
        //Create limit planes
        NxU32 planes = joint->mNxuPlaneInfo.size();
        if (planes)
        {
            joint->mInstance->setLimitPoint(joint->mPlaneLimitPoint, joint->mOnActor2 ? true : false);
        }
        for (NxU32 p=0;p<planes;++p)
        {
            joint->mInstance->addLimitPlane(joint->mNxuPlaneInfo[i]->mPlaneNormal, joint->mNxuPlaneInfo[i]->mWorldLimitPt);
        }
        if (callback)
        {
            callback->NotifyJoint(joint->mInstance);
        }
    }

    count = model.mEffectors.size();
    for (NxU32 i=0;i<count;++i)
    {
        NxuSpringAndDamperEffectorDesc* d = model.mEffectors[i];
        d->body1 = d->mRefAttachActorDesc->mInstance;
        d->body2 = d->mAttachActorDesc->mInstance;
        d->mInstance = scene.createSpringAndDamperEffector(*d);
    }

#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
    count = model.mFluids.size();
    //Since this scene has a fluid, we need to cook all triangle mesh for the fluid
    //This is kind of ugly and assumes that the fluid is being created after all other actors
    if (count)
    {
	    // Add all static shapes to the fluid scene
	    NxArray<NxTriangleMeshShape*> shapes;

	    for (NxU32 i = 0; i < scene.getNbActors(); i++) {
		    NxActor* curActor = scene.getActors()[i];
		    if (curActor->isDynamic())
			    continue;

		    for (NxU32 j = 0; j < curActor->getNbShapes(); j++) {
			    NxShape* curShape = curActor->getShapes()[j];
			    NxTriangleMeshShape* mesh = curShape->isTriangleMesh();
			    if (mesh)
				    shapes.pushBack(mesh);
		    }
	    }
        if (shapes.size() > 0)
        {
	        ImportMemoryWriteBuffer buf;
            NxFluidDesc* desc = model.mFluids[0];
	        bool status = NxCookFluidHardwareMesh(desc->restParticlesPerMeter, desc->kernelRadiusMultiplier, desc->motionLimitMultiplier, desc->packetSizeMultiplier, shapes, buf);
	        scene.createFluidHardwareTriangleMesh(ImportMemoryReadBuffer(buf.data));
        }
    }

    for (NxU32 i=0;i<count;++i)
    {
        NxuFluidDesc* d = model.mFluids[i];
        d->mInstance = scene.createFluid(*d);
        if (callback && d->mInstance )
        {
            callback->NotifyFluid(d->mInstance);
        }
    }
#endif
}


NxuSectionTypes NxuPhysicsImport::parse(NxuInputStream & userStream)
{
    return mCollection->read(userStream);
}


void NxuPhysicsImport::instantiate(NxPhysicsSDK & sdk,             // SDK to load the collection into.
	                     NxuUserNotify *notify, // notification events to the application as the scene is loaded.
	                     NxuUserCreate  *create, // if the user wants control over scene creation
                         NxScene* defaultScene,  
                         NxMat34* defaultSceneOffset,
	                     SceneType stype)
{
    NxU32 numParams = mCollection->sdkParams.size();
    for (NxU32 i=0;i<numParams;++i)
    {
        NxuParameterData* p = mCollection->sdkParams[i];
        sdk.setParameter(p->param, p->value);
    }

    
    instanceTrimeshs(sdk);
    instanceConvexes(sdk);
    instanceSkeletons(sdk);
    instanceHeightFields(sdk);

    NxU32 numScenes = mCollection->scenes.size();
    for (NxU32 i=0;i<numScenes;++i)
    {
        NxuSceneDesc* p = mCollection->scenes[i];
        NxScene* newScene = 0;
        if (create)
        {
            newScene = create->CreateScene(&sdk, *p);
        }
        else
        {
            newScene = sdk.createScene(*p);
        }
        p->mInstance = newScene;

        if (newScene)
        {
            NxMat34 xform;
            xform.id();
            instanceModel(*newScene, *(p->mModel), xform, notify);
            if (notify)
            {
                notify->NotifyScene(newScene);
            }
        }
    }

    if (defaultScene)
    {
        NxMat34 xform;
        if (defaultSceneOffset)
        {
            xform =  *defaultSceneOffset;
        } 
        else
        {
            xform.id();
        }
        instanceModel(*defaultScene, mCollection->defaultModel, xform, notify);
    }
}

NX_BOOL instantiate(NxScene& scene,             
	                NxuUserNotify *notify	= 0, 
	                NxuUserCreate  *create     = 0)
{
    //NxuPhysicsModel defaultModel; //For storing arbitrary non-instanced actors, joints, etc
    //NxuActorDesc defaultActor; //For storing aribitary non-instanced shapes
    return false;
}

void NxuPhysicsImport::reset() //Clears cached values so the subsequent parses will not create links with previous data
{
    if (mCollection) delete mCollection;
    mCollection = new NxuPhysicsCollection();
}

NxuPhysicsCollection* NxuPhysicsImport::getCollection() //Obtain a reference to the data read in so far
{
    return mCollection;
}

void NxuPhysicsImport::setCollection(NxuPhysicsCollection* collection) //Obtain a reference to the data read in so far
{
    mCollection = collection;
}



//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND
