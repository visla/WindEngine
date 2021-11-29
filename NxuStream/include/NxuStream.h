/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		NxuStream.h

		This header describes common interface elements to support loading and saving
		of physics data to a stream.

		Changelist
		----------

*/

#ifndef NXU_STREAM_H
#define NXU_STREAM_H

#define NXU_STREAM_MAX_STRING_LENGTH 256

//Importing interfaces
#include "NxPhysics.h"
#include "NxCCDSkeleton.h"

//These flags indicate what SDK features are supported
#define NXU_SDK_HAS_SKELETONS       (1 << 0)
#define NXU_SDK_HAS_CCD_THRESHOLD   (1 << 1)
#define NXU_SDK_HAS_HARDWARE        (1 << 2)
#define NXU_SDK_HAS_WHEEL           (1 << 3)
#define NXU_SDK_HAS_230_FLAGS       (1 << 4)
#define NXU_SDK_HAS_PIPELINE        (1 << 5)   
#define NXU_SDK_HAS_231_FLAGS       (1 << 6)
#define NXU_SDK_HAS_THREADING       (1 << 7)
#define NXU_SDK_HAS_HEIGHTFIELD     (1 << 8)
#define NXU_SDK_HAS_EFFECTOR_DESC   (1 << 9)
#define NXU_SDK_HAS_FLUID_API       (1 << 10)
#define NXU_SDK_HAS_CLOTH_API       (1 << 11)
#define NXU_SDK_HAS_PER_SHAPE_MASS  (1 << 12)
#define NXU_SDK_HAS_BITMASK_FILTER  (1 << 13)
#define NXU_SDK_HAS_CONVEX_SCALE    (1 << 14)

#if NX_SDK_VERSION_NUMBER >= 230
#define NXU_SDK_230_FEATURES  NXU_SDK_HAS_SKELETONS | NXU_SDK_HAS_CCD_THRESHOLD | NXU_SDK_HAS_HARDWARE | NXU_SDK_HAS_WHEEL | NXU_SDK_HAS_230_FLAGS | NXU_SDK_HAS_EFFECTOR_DESC | NXU_SDK_HAS_PER_SHAPE_MASS
#else
#define NXU_SDK_230_FEATURES 0
#endif

#if NX_SDK_VERSION_NUMBER >= 231
#define NXU_SDK_231_FEATURES  NXU_SDK_HAS_PIPELINE | NXU_SDK_HAS_231_FLAGS
#else
#define NXU_SDK_231_FEATURES 0
#endif

#if NX_SDK_VERSION_NUMBER >= 240
#define NXU_SDK_240_FEATURES NXU_SDK_HAS_THREADING | NXU_SDK_HAS_HEIGHTFIELD
#else
#define NXU_SDK_240_FEATURES 0
#endif

#if NX_USE_FLUID_API
#define NXU_SDK_FLUID_FEATURES NXU_SDK_HAS_FLUID_API
#else
#define NXU_SDK_FLUID_FEATURES 0
#endif

#ifdef NX_USE_CLOTH_API
#define NXU_SDK_CLOTH_FEATURES NXU_SDK_HAS_CLOTH_API
#else
#define NXU_SDK_CLOTH_FEATURES 0
#endif

#ifdef NX_SUPPORT_NEW_FILTERING
#define NXU_SDK_FILTERING_FEATURES NXU_SDK_HAS_BITMASK_FILTER
#else
#define NXU_SDK_FILTERING_FEATURES 0
#endif 

#ifdef NX_SUPPORT_CONVEX_SCALE
#define NXU_SDK_SCALE_FEATURES NXU_SDK_HAS_CONVEX_SCALE
#else
#define NXU_SDK_SCALE_FEATURES 0
#endif

#define NXU_SDK_FEATURES ( NXU_SDK_230_FEATURES | NXU_SDK_231_FEATURES | NXU_SDK_240_FEATURES | NXU_SDK_FLUID_FEATURES | NXU_SDK_CLOTH_FEATURES | NXU_SDK_FILTERING_FEATURES | NXU_SDK_SCALE_FEATURES)


/**
\brief This stuct is used to represent and SDK paramter

@see NxPhysicsSDK.setParameter() NxPhysicsSDK.getParameter()
*/
struct NxuParameterData
{
	NxParameter param;
	NxReal value;
    static const char* GetParamName(NxParameter p);
};

/**
\brief Type of error encountered when accessing stream

*/
enum NxuStreamError
{
    NXU_NO_ERROR = 0,
    NXU_IOERROR,
    NXU_END_OF_FILE,
    NXU_UNKNOWN
};

/**
\brief Section header for defining sections of the data stream

sectionType is the type of object stored in this section
size is the total byte size of this section, may not be used for text formats
name is the label for this section, may be null for binary formats

@see NxuInputStream.readHeader() NxuOutputStream.storeHeader()
*/
struct NxuFormatHeader
{
	NxU8 sectionType;                     //The type of object stored in this section
    NxU32 size;                           //The total byte size of this section, may not be used for text formats
	char name[NXU_STREAM_MAX_STRING_LENGTH];//The label for this section, may be null for binary formats
};


class NxuActorDesc;
class NxuPhysicsModel;
class NxuCCDSkeletonDesc;
class NxuConvexMeshDesc;
class NxuTriangleMeshDesc;
class NxuHeightFieldDesc;
class NxuPhysicsCollection;

/**
\brief Callback mechanism to convert pointers to serialized form

These callbacks are used internally to convert a pointer to a index within one of the collections arrays.
This allows the index to be stored in the file and later converted back to a pointer when the stream is read in.

*/
class NxuIndexLookup
{
public:
    NxuPhysicsCollection* collection;
    NxuPhysicsModel* model;
    virtual NxU32 pointerToIndex(NxuActorDesc* p);
    virtual NxU32 pointerToIndex(NxuPhysicsModel* p);
    virtual NxU32 pointerToIndex(NxuCCDSkeletonDesc* p);
    virtual NxU32 pointerToIndex(NxuConvexMeshDesc* p);
    virtual NxU32 pointerToIndex(NxuTriangleMeshDesc* p);
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
    virtual NxU32 pointerToIndex(NxuHeightFieldDesc* p);
#endif
};

/**
\brief Callback mechanism to convert pointers from serialized form

These callbacks are used internally to convert an index within one of the collections arrays to a pointer.
This allows the index to be stored in the file and later converted back to a pointer when the stream is read in.

*/
class NxuPointerLookup
{
public:
    NxuPhysicsCollection* collection;
    NxuPhysicsModel* model;
    virtual void indexToPointer(NxU32 index, NxuActorDesc** p);
    virtual void indexToPointer(NxU32 index, NxuPhysicsModel** p);
    virtual void indexToPointer(NxU32 index, NxuCCDSkeletonDesc** p);
    virtual void indexToPointer(NxU32 index, NxuConvexMeshDesc** p);
    virtual void indexToPointer(NxU32 index, NxuTriangleMeshDesc** p);
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
    virtual void indexToPointer(NxU32 index, NxuHeightFieldDesc** p);
#endif
};

/**
\brief Abstract interface definition for reading a structure stream

This is expected to be subclassed for each file format other other stream.
The interace is used by the importer for extracting primitives and structures from a stream.

@see NxuPhysicsImport NxuBinaryStream
*/
class NxuInputStream
{
public:
    NxuPointerLookup pointerLookup;
    virtual ~NxuInputStream() {}
    NxuStreamError       errorCode;
    /**
    \brief Returns the format id for this stream.

    This allows for identifying the type of the stream.

    \return The format id for this stream

    @see NxuStreamFactory
    */
    virtual NxU32               getFormat() = 0;

    /**
    \brief Gets the feature list of the SDK used to write the file.

    The list of features present in the SDK when the file was written. Allows for backward compatibility.

    \return The SDK version flags from NXU_SDK_FEATURES.
    */
    virtual NxU32               getSDKVersion() = 0;

    virtual		NxuInputStream&	readBool(NX_BOOL& b, const char* name = NULL)					 = 0;
    virtual		NxuInputStream&	readByte(NxU8& b, const char* name = NULL)					 = 0;
	virtual		NxuInputStream&	readWord(NxU16 &w, const char* name = NULL)					 = 0;
	virtual		NxuInputStream&	readDword(NxU32 &i, const char* name = NULL)				 = 0;
	virtual		NxuInputStream&	readFloat(NxF32 &f, const char* name = NULL)				 = 0;
	virtual		NxuInputStream&	readDouble(NxF64 &d, const char* name = NULL)				 = 0;
	virtual		NxuInputStream&	readByteBuffer(void* buffer, NxU32 size, const char* name = NULL)	 = 0;
	virtual		NxuInputStream&	readWordBuffer(void* buffer, NxU32 size, const char* name = NULL)	 = 0;
	virtual		NxuInputStream&	readDwordBuffer(void* buffer, NxU32 size, const char* name = NULL)	 = 0;
	virtual		NxuInputStream&	readFloatBuffer(void* buffer, NxU32 size, const char* name = NULL)	 = 0;
	virtual		NxuInputStream&	readDoubleBuffer(void* buffer, NxU32 size, const char* name = NULL)	 = 0;
	virtual		NxuInputStream&	readCharBuffer(void* buffer, NxU32 size, const char* name = NULL)	 = 0;

    /**
    \brief Reads the header for the next section

    /param [out] header Contains the type of the next section.
    */
    virtual void readHeader(NxuFormatHeader &header) = 0;
    /**
    \brief Skips rest of the section covered by the most recent header.
    */
	virtual void skip() =  0;

	virtual NxuInputStream& readVector(NxVec3 &vec, const char* name = NULL) = 0;
	virtual NxuInputStream& readQuat(NxQuat &quat, const char* name = NULL) = 0;
	virtual NxuInputStream& readMatrix(NxMat34 &mat, const char* name = NULL) = 0;
	virtual NxuInputStream& readParameter(NxuParameterData& param, const char* name = NULL) = 0;

};

/**
\brief Abstract interface definition for writing a structure stream

This is expected to be subclassed for each file format other other stream.
The interace is used by the importer for inserting primitives and structures to a stream.

@see NxuPhysicsExport NxuBinaryStream
*/
class NxuOutputStream
{
public:
    virtual ~NxuOutputStream() {}
    NxuIndexLookup indexLookup;
    NxuStreamError       errorCode;

    /**
    \brief Returns the format id for this stream.

    This allows for identifying the type of the stream.

    \return The format id for this stream

    @see NxuStreamFactory
    */
    virtual NxU32               getFormat() = 0;

    /**
    \brief Forces any buffered data out into the underlying stream

    */
    virtual void                 flush() = 0;

    /**
    \brief Sets the feature list of the SDK used to write the file.

    When writing a file, set the sdk feature set being used by the exporter so that it can be stored in the file.
    This is needed on import to properly read the file.

    param [in] version The SDK version flags NXU_SDK_FEATURES.
    */
    virtual void                setSDKVersion(NxU32 version) = 0;

    virtual		NxuOutputStream&		storeBool(NX_BOOL b, const char* name = NULL) = 0;
    virtual		NxuOutputStream&		storeByte(NxU8 b, const char* name = NULL) = 0;
	virtual		NxuOutputStream&		storeWord(NxU16 w, const char* name= NULL) = 0;
	virtual		NxuOutputStream&		storeDword(NxU32 d, const char* name= NULL) = 0;
	virtual		NxuOutputStream&		storeFloat(NxF32 f, const char* name= NULL) = 0;
	virtual		NxuOutputStream&		storeDouble(NxF64 f, const char* name= NULL) = 0;
	virtual		NxuOutputStream&		storeByteBuffer(const void* buffer, NxU32 size, const char* name= NULL) = 0;
	virtual		NxuOutputStream&		storeWordBuffer(const void* buffer, NxU32 size, const char* name= NULL) = 0;
	virtual		NxuOutputStream&		storeDwordBuffer(const void* buffer, NxU32 size, const char* name= NULL) = 0;
	virtual		NxuOutputStream&		storeFloatBuffer(const void* buffer, NxU32 size, const char* name= NULL) = 0;
	virtual		NxuOutputStream&		storeDoubleBuffer(const void* buffer, NxU32 size, const char* name= NULL) = 0;
	virtual		NxuOutputStream&		storeCharBuffer(const void* buffer, NxU32 size, const char* name= NULL) = 0;

    /**
    \brief Stores the header for the section about to written

    /param [in] header Contains the type of the next section.
    */
    virtual void storeHeader(const NxuFormatHeader & header) = 0;

    /**
    \brief Ends the section started by the most recent header.

    Subsequent headers will be placed at the same level as the previous one rather than within it.
    */
	virtual void endSection() = 0;

	virtual NxuOutputStream& storeVector(const NxVec3 &vec, const char* name= NULL) = 0;
	virtual NxuOutputStream& storeQuat(const NxQuat &quat, const char* name= NULL) = 0;
	virtual NxuOutputStream& storeMatrix(const NxMat34 &mat, const char* name= NULL) = 0;
	virtual NxuOutputStream& storeParameter(const NxuParameterData& param, const char* name= NULL) = 0;

};

/**
\brief Sections of the file

This is an enumeration of the various logical data structures that can be streamed.
It is used to organize the data stream and to identify what information was recently read in.

*/
enum NxuSectionTypes
{
    NxuTypeSDK = 0,
    NxuTypeScene,
    NxuTypeModel,
    NxuTypeActor,
    NxuTypeJoint,
    NxuTypeMaterial,
    NxuTypePairFlag,
    NxuTypeShape,
    NxuTypeTriangleMesh,
    NxuTypeConvexMesh,
    NxuTypeCCDSkeleton,
	NxuTypeEffector,
    NxuTypeFluid,
    NxuTypeFluidEmitter,
    NxuTypeHeightField,
    NxuTypeModelInstance,
    NxuTypeUserData,
    //Insert new types here
    NxuTypeUnknown
};



/**
\brief Abstract interface for streamable descriptors

This interace is used by all the extended descriptors to allow them to read and write themselves from a stream.

*/
class NxuStreamable
{
public:
    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL) = 0;
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL) = 0;
};


//==================================================================================
//==================================================================================
//This section defines extended subclass of the PhysX descriptors
//These extended descriptors allow full representation of the structures outside of the SDK
//They can be used to generate fully instantiable scenes without needed to refer to other instantiated objects
//The "mInstance" field refers to the instantiated object if it exists or is null otherwise
//Each extended descriptor structure is streamable so that it can write itself to a file
//Refer to the base class within the PhysX API

/**
\brief TriangleMesh descriptor. 

Includes the ability to represent the cooked data for faster loading
*/
class NxuTriangleMeshDesc : public NxTriangleMeshDesc, public NxuStreamable
{
public:
	void *					mCookedData;
	NxU32					mCookedDataSize;
    void *                  mPmap;
    NxU32                   mPmapSize;
    NxTriangleMesh*   mInstance;
    NxuTriangleMeshDesc() : mCookedData(0), mCookedDataSize(0), mPmap(0), mPmapSize(0), mInstance(0) {this->setToDefault();}
    virtual ~NxuTriangleMeshDesc() {if (mCookedData) delete[] mCookedData; if (mPmap) delete[] mPmap;}
    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
} ;


/**
\brief ConvexMesh descriptor.

Includes the ability to represent the cooked data for faster loading
*/
class NxuConvexMeshDesc : public NxConvexMeshDesc, public NxuStreamable
{
public:
	void *					mCookedData;
	NxU32					mCookedDataSize;
    NxConvexMesh*     mInstance; //Optional reference to instantiated mesh for id purposes
    NxuConvexMeshDesc() : mCookedData(0), mCookedDataSize(0), mInstance(0) {this->setToDefault();}
    virtual ~NxuConvexMeshDesc() {if (mCookedData) delete[] mCookedData;}
	
    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

/**
\brief Heightfield descriptor

Note that unlike the base class, the extended descriptor will manage
its own memory and deallocate the samples array on deletion. Be sure to zero out the samples
field if you set it manually on creation and don't want it deleted.
*/
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
class NxuHeightFieldDesc : public NxHeightFieldDesc, public NxuStreamable
{
public:
    NxHeightField*              mInstance; //Optional reference to instantiated mesh for id purposes
    NxuHeightFieldDesc()  : mInstance(0) {}
    virtual ~NxuHeightFieldDesc() {if (samples) delete[] samples;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};
#endif

/**
\brief CCD Skeleton descriptor.

Includes the ability to represent the cooked data for faster loading
WARNING: Currently Skeletons exported from the SDK will only contain binary data,
the triangle data from NxSimpleTriangleMesh will be empty in this case.
*/
class NxuCCDSkeletonDesc : public NxSimpleTriangleMesh, public NxuStreamable
{
public:
	void *					mCookedData;
	NxU32					mCookedDataSize;
    NxCCDSkeleton*    mInstance; //Optional reference to instantiated mesh for id purposes
    NxuCCDSkeletonDesc() : mCookedData(0), mCookedDataSize(0), mInstance(0) {this->setToDefault();}
    virtual ~NxuCCDSkeletonDesc() {if (mCookedData) delete[] mCookedData;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};


/**
\brief Material descriptor.

The extended descriptor preserves the index so that the 
material is reloaded correctly and all objects refering to the old index will
still get the correct material. WARNING: If you save a material from one scene
and load it into another which already has a material at the saved index,
it will be overwritten by the new material.
*/
class NxuMaterialDesc : public NxMaterialDesc, public NxuStreamable
{
public:
    NxuMaterialDesc() : index(NxU16(-1)), mInstance(0) {this->setToDefault();}
    virtual ~NxuMaterialDesc() {}
    NxMaterialIndex index;
    NxMaterial* mInstance;

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

//

/**
\brief Base abstract class for extended shape descriptors

Preserves the CCD skeleton if one exists for that shape.
The getShapeDesc() method allows extended descriptors to downcast themselves.
*/
class NxuShapeDesc : public NxuStreamable
{
public:
    NxuCCDSkeletonDesc*         mCCDSkeleton;
    NxShape*          mInstance; //reference to shape if it exists

    NxuShapeDesc() :  mCCDSkeleton(0), mInstance(0) {}
    virtual ~NxuShapeDesc() {}

    virtual NxShapeDesc*    getShapeDesc() = 0;

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);

};

//Extended shape descriptors. Each PhysX shape is represented by an extended descriptor.
//----------------------------------------------------------------

class NxuPlaneShapeDesc :  public  NxPlaneShapeDesc, public  NxuShapeDesc  
{
public:
    virtual ~NxuPlaneShapeDesc() {}
    virtual NxuPlaneShapeDesc*    getShapeDesc() {return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

class NxuSphereShapeDesc : public  NxSphereShapeDesc, public  NxuShapeDesc
{
public:
    virtual ~NxuSphereShapeDesc() {}
    NxuSphereShapeDesc*    getShapeDesc(){return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};
class NxuBoxShapeDesc: public  NxBoxShapeDesc, public  NxuShapeDesc
{
public:
    virtual ~NxuBoxShapeDesc() {}
    NxuBoxShapeDesc*    getShapeDesc(){return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};
class NxuCapsuleShapeDesc: public  NxCapsuleShapeDesc, public  NxuShapeDesc
{
public:
    virtual ~NxuCapsuleShapeDesc() {}
    NxuCapsuleShapeDesc*    getShapeDesc(){return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};


/**
\brief Convex shape descriptor

Contains a reference to the Convex Mesh descritor containing its geometry
*/
class NxuConvexShapeDesc: public  NxConvexShapeDesc, public  NxuShapeDesc
{
public:
    virtual ~NxuConvexShapeDesc() {}
    NxuConvexMeshDesc*      mConvexMeshDesc;
    NxuConvexShapeDesc() :  mConvexMeshDesc(0) {};
    NxuConvexShapeDesc*    getShapeDesc(){return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

#if NXU_SDK_FEATURES & NXU_SDK_HAS_WHEEL
class NxuWheelShapeDesc : public NxWheelShapeDesc, public  NxuShapeDesc
{
public:
    virtual ~NxuWheelShapeDesc() {}
    NxuWheelShapeDesc*    getShapeDesc(){return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};
#endif

/**
\brief Trinagle shape descriptor

Contains a reference to the Triangle Mesh descritor containing its geometry.
*/
class NxuTriangleMeshShapeDesc: public  NxTriangleMeshShapeDesc, public  NxuShapeDesc
{
public:
    virtual ~NxuTriangleMeshShapeDesc() {}
    NxuTriangleMeshDesc*          mTriangleMeshDesc;
    NxuTriangleMeshShapeDesc() :  mTriangleMeshDesc((NxuTriangleMeshDesc*)0) {};
    NxuTriangleMeshShapeDesc*    getShapeDesc(){return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

/**
\brief Heightfield shape descriptor

Contains a reference to the Heightfield descritor containing its data.
*/
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
class NxuHeightFieldShapeDesc: public  NxHeightFieldShapeDesc, public  NxuShapeDesc
{
public:
    virtual ~NxuHeightFieldShapeDesc() {}
    NxuHeightFieldDesc*          mHeightFieldDesc;
    NxuHeightFieldShapeDesc() :  mHeightFieldDesc((NxuHeightFieldDesc*)0) {};
    NxuHeightFieldShapeDesc*    getShapeDesc(){return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};
#endif

//==================================================================================
//==================================================================================

/**
\brief Actor descriptor

Contains a reference to its body
If mBody.mass is less than 0, the body is not valid and won't be used,
this is appropriate for static shapes.
*/
class NxuActorDesc : public NxActorDesc, public NxuStreamable
{
public:
    NxBodyDesc                  mBody;
    NxActor               *mInstance; //reference to actor instantiation if it exists
    virtual ~NxuActorDesc();

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

/**
\brief Structure defining a limit plane

*/
struct NxuPlaneInfo
{
	NxVec3			mPlaneNormal;
	NxVec3          mWorldLimitPt;
	float           mPlaneD;
};


//==================================================================================
//==================================================================================
//This section descripts the joint descriptors.

/**
\brief Abstract base class for extended joint descriptors.

The extended joint descriptor contains a reference to actor descriptors that it joins
as well as limit plane information. This allows it to fully recreate the jointed pair of actors with limit planes.
The RefAttachActor is used for actor[0], the AttachActor is used for actor[i]
The getDesc() method can be used by subclasses to properly downcast.

@see NxuShapeDesc
*/
class NxuJointDesc : public NxuStreamable
{
public:
	NxVec3                       mPlaneLimitPoint;
	NX_BOOL                         mOnActor2;
	NxArray< NxuPlaneInfo * >       mNxuPlaneInfo;
    NxuActorDesc*                 mRefAttachActorDesc;
    NxuActorDesc*                 mAttachActorDesc;
    NxJoint*               mInstance; 
    NxuJointDesc() : mPlaneLimitPoint(NxVec3(0,0,0)) {}
    virtual NxJointDesc*    getDesc() = 0;
    virtual ~NxuJointDesc();

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

class NxuD6JointDesc : public NxD6JointDesc, public NxuJointDesc 
{
public:
    virtual ~NxuD6JointDesc() {}
    NxuD6JointDesc*    getDesc() {return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

class NxuPrismaticJointDesc : public NxPrismaticJointDesc, public NxuJointDesc 
{
public:
    virtual ~NxuPrismaticJointDesc() {}
    NxuPrismaticJointDesc*    getDesc() {return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

class NxuRevoluteJointDesc : public NxRevoluteJointDesc, public NxuJointDesc 
{
public:
    virtual ~NxuRevoluteJointDesc() {}
    NxuRevoluteJointDesc*    getDesc() {return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

class NxuCylindricalJointDesc : public NxCylindricalJointDesc, public NxuJointDesc 
{
public:
    virtual ~NxuCylindricalJointDesc() {}
    NxuCylindricalJointDesc*    getDesc() {return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

class NxuSphericalJointDesc : public NxSphericalJointDesc, public NxuJointDesc
{
public:
    virtual ~NxuSphericalJointDesc() {}
    NxuSphericalJointDesc*    getDesc() {return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

class NxuPointOnLineJointDesc : public NxPointOnLineJointDesc, public NxuJointDesc
{
public:
    virtual ~NxuPointOnLineJointDesc() {}
    NxuPointOnLineJointDesc*    getDesc() {return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

class NxuPointInPlaneJointDesc : public NxPointInPlaneJointDesc, public NxuJointDesc
{
public:
    virtual ~NxuPointInPlaneJointDesc() {}
    NxuPointInPlaneJointDesc*    getDesc() {return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

class NxuDistanceJointDesc : public NxDistanceJointDesc, public NxuJointDesc 
{
public:
    virtual ~NxuDistanceJointDesc() {}
   NxuDistanceJointDesc*    getDesc() {return this;}

   virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
   virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

class NxuPulleyJointDesc : public NxPulleyJointDesc, public NxuJointDesc 
{
public:
    virtual ~NxuPulleyJointDesc() {}
   NxuPulleyJointDesc*    getDesc() {return this;}

   virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
   virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

class NxuFixedJointDesc : public NxFixedJointDesc, public NxuJointDesc 
{
public:
    virtual ~NxuFixedJointDesc() {}
   NxuFixedJointDesc*    getDesc() {return this;}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

/**
\brief Extended descriptor for the spring and damper effector.

It contains refereces to descriptions for the attached actors so that the whole system can be reconstituted.
RefAttachActor is actor[0], AttachActor is actor[1].

*/
class NxuSpringAndDamperEffectorDesc : public NxSpringAndDamperEffectorDesc, public NxuStreamable
{
public:
    NxuActorDesc*                 mRefAttachActorDesc;
    NxuActorDesc*                 mAttachActorDesc;
    NxSpringAndDamperEffector*    mInstance;

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
    NxuSpringAndDamperEffectorDesc() : mRefAttachActorDesc((NxuActorDesc*)-1), mAttachActorDesc((NxuActorDesc*)-1) {}
    virtual ~NxuSpringAndDamperEffectorDesc() {}
};

/**
\brief Pair wise flags between actors or shapes.

This descriptor contains the descriptors for the actors to which the flag applies
mShapeIndex0 refers to the offset within mActor0's shape list if it is a shape pair flag, or is -1 otherwise
mShapeIndex1 refers to the offset within mActor1's shape list if it is a shape pair flag, or is -1 otherwise

*/
class NxuPairFlagDesc : public NxuStreamable
{
public:
    NxuPairFlagDesc() : mFlag(0), mActor0(0), mActor1(0), mShapeIndex0(NxU32(-1)), mShapeIndex1(NxU32(-1)) {}
    virtual ~NxuPairFlagDesc() {}
	NxU32 mFlag;
	NxuActorDesc* mActor0;
	NxuActorDesc* mActor1;
	NxU32 mShapeIndex0;
    NxU32 mShapeIndex1;

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
/**
\brief Extended description of a fluid emitter.

Contains a referece to the actor that the emitter is attached to (if any).

*/
class NxuFluidEmitterDesc : public NxFluidEmitterDesc, public NxuStreamable
{
public:
    NxuFluidEmitterDesc() : mFrameActor(0), mInstance(0) {}
    virtual ~NxuFluidEmitterDesc() {}
    NxFluidEmitter*    mInstance;
    NxuActorDesc* mFrameActor;

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

/**
\brief Extended descriptor of a fluid

@see NxuFluidEmitterDesc
*/
class NxuFluidDesc : public NxFluidDesc, public NxuStreamable
{
public:
    NxuFluidDesc() : mInstance(0), mNumParticles(0)/*,  mImplicitVerts(0), mImplicitIndexes(0), mImplicitParents(0)*/ {}
    virtual ~NxuFluidDesc() {}
    NxFluid*    mInstance;
    NxArray<NxuFluidEmitterDesc*> mEmitters; //Cant reuse base class array because it uses objects instead of pointers

    NxU32 mNumParticles;            //The size of the particle data buffers for NxParticleData

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};
#endif



class NxuPhysicsModel;


/**
\brief Allows nesting of physics models.

A phyics model instance allows a group of things defined within a physics model to be placed within
 another model. This allows many copies of the same ragdoll to be placed at different points within
 a scene without having to define the descriptors for the ragdoll each time.
 mModel references the physics model being instantiated, mPose represents the placement
 relative to the parent models origin.

@see NxuPhysicsModel
*/
class NxuPhysicsModelInstance : public NxuStreamable
{
public:
    NxuPhysicsModel* mModel;
    NxMat34 mPose;
    NxuPhysicsModelInstance() {mPose.id(); mModel = 0;}
    virtual ~NxuPhysicsModelInstance() {}

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

/**
\brief Grouping of objects and constraints that can be placed within a scene

Physics models allow representation of systems of actors, joints, etc. that can be managed as a unit.
Examples might include ragdolls, cars, or just the layout of objects within a scene.
This allows copies of objects to be placed in multiple locations without needing to completely redefine the object.

@see NxuPhysicsModelInstance
*/
class NxuPhysicsModel : public NxuStreamable
{
public:
    NxuPhysicsModel() {name[0] = 0;}
    virtual ~NxuPhysicsModel();
    char name[NXU_STREAM_MAX_STRING_LENGTH];
    NxArray< NxuMaterialDesc *	>    mMaterials;
    NxArray< NxuActorDesc *	>    mActors;

    NxArray< NxuJointDesc * >  mJoints;
    NxArray< NxuPairFlagDesc *> mPairFlags;

    NxArray< NxuSpringAndDamperEffectorDesc * > mEffectors;

#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
    NxArray< NxuFluidDesc* > mFluids;
#endif

    NxArray< NxuPhysicsModelInstance* > mModels;


    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

/**
\brief Extended descriptor of a scene.

Includes the physics model containing all the contents of the scene.

@see NxuPhysicsModel
*/
class NxuSceneDesc : public NxSceneDesc, public NxuStreamable
{
public:
    NxuSceneDesc() : mModel(0), mInstance(0) {}
    virtual ~NxuSceneDesc() {if (mModel) delete mModel;}
    NxuPhysicsModel* mModel;
    NxScene* mInstance; //Optional reference to scene pointer for identification

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};

/**
\brief A collection class for containing everything to be exported or that was just imported.

This includes the SDK parameters, all scenes, all geometry data (convex, trinagle mesh, heightfields, skeletons), etc.
The defaultModel field is used to store actors and constraints that dont exist within the context of a known scene.
The defaultActor field is used to store shapes that dont have an associated actor.
These two fields allow export of libraries of objects without having to place them within the context of a PhysX scene.
*/
class NxuPhysicsCollection : public NxuStreamable
{
public:
    NxuPhysicsCollection() {}
    virtual ~NxuPhysicsCollection();
    NxArray<NxuParameterData*> sdkParams;
    NxArray<NxuConvexMeshDesc*> convexMeshes;
    NxArray<NxuTriangleMeshDesc*> triangleMeshes;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
    NxArray<NxuHeightFieldDesc*> heightFields;
#endif
    NxArray<NxuCCDSkeletonDesc*> skeletons;
    NxArray<NxuSceneDesc*> scenes;
    NxArray<NxuPhysicsModel*> models;
    NxuPhysicsModel defaultModel; //For storing arbitrary non-instanced actors, joints, etc
    NxuActorDesc defaultActor; //For storing aribitary non-instanced shapes

    virtual NxuSectionTypes store(NxuOutputStream & stream, const char* name = NULL);
    virtual NxuSectionTypes read(NxuInputStream & stream, const char* name = NULL);
};


#endif //NXU_STREAM_H


//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND

