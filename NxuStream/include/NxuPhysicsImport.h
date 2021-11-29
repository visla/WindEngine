/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		NxuPhysicsImport.h

		This is the common import implementation that contains format independent import logic.

		Changelist
		----------

*/
#ifndef NXU_PHYSICS_IMPORT_H
#define NXU_PHYSICS_IMPORT_H

#include <stdio.h>

#include "NxuStream.h"
#include "NxStream.h"



class ImportMemoryWriteBuffer : public NxStream
	{
	public:
								ImportMemoryWriteBuffer();
	virtual						~ImportMemoryWriteBuffer();
				void			clear();

	virtual		NxU8			readByte()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		NxU16			readWord()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		NxU32			readDword()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		float			readFloat()								const	{ NX_ASSERT(0);	return 0.0f;}
	virtual		double			readDouble()							const	{ NX_ASSERT(0);	return 0.0;	}
	virtual		void			readBuffer(void* buffer, NxU32 size)	const	{ NX_ASSERT(0);				}

	virtual		NxStream&		storeByte(NxU8 b);
	virtual		NxStream&		storeWord(NxU16 w);
	virtual		NxStream&		storeDword(NxU32 d);
	virtual		NxStream&		storeFloat(NxReal f);
	virtual		NxStream&		storeDouble(NxF64 f);
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size);

				NxU32			currentSize;
				NxU32			maxSize;
				NxU8*			data;
	};

class ImportMemoryReadBuffer : public NxStream
	{
	public:
								ImportMemoryReadBuffer(const NxU8* data);
	virtual						~ImportMemoryReadBuffer();

	virtual		NxU8			readByte()								const;
	virtual		NxU16			readWord()								const;
	virtual		NxU32			readDword()								const;
	virtual		float			readFloat()								const;
	virtual		double			readDouble()							const;
	virtual		void			readBuffer(void* buffer, NxU32 size)	const;

	virtual		NxStream&		storeByte(NxU8 b)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeWord(NxU16 w)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeDword(NxU32 d)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeFloat(NxReal f)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeDouble(NxF64 f)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size)	{ NX_ASSERT(0);	return *this;	}

	mutable		const NxU8*		buffer;
	};












/**
\brief Callback interface that can be used to recieve notifcations when objects are created.

If a subclass of this interface is provided, the importer will make a callback once
the objects have been instantiated.  The callbacks occur after the objects have been created
and fully initialized.
@see NxuPhysicsImport.instantiate()
*/
class NxuUserNotify
{
public:

	virtual	void NotifyScene(NxScene *scene) = 0;

	/**
	* Notify application of	a joint	creation.
	*/
	virtual	void NotifyJoint(NxJoint *joint) = 0;

	/**
	* Notify application of	a actor	creation.
	*/
	virtual	void NotifyActor(NxActor *actor) = 0;

#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
	/**
	* Notify application of	a fluid	creation. Application should call setParticlesWriteData() to specify destination of fluid updates
	*/
    virtual void NotifyFluid(NxFluid *fluid) = 0;
#endif
};

/**
\brief Callback interface to allow full control over the instantiation of objects/

The callbacks will be recieved when the object is ready to be created.
It is expected that the user will create the object and return the new object.
The callback may return NULL if you do not want the object to be instantiated.
It may also modify the descriptor if desired, to add user data for example.
Returning NULL may result in other objects not being created if they are dependant on the current one.

@see NxuPhysicsImport.instantiate()
*/
class NxuUserCreate
{
public:
  virtual NxScene * CreateScene(NxPhysicsSDK *sdk, NxuSceneDesc &desc) = 0;
};

/**
\brief Format independant importer. 

The class can read in data from a provided NxuStream and create the appropriate descriptors.
The imported data can then optionally be instantiated

*/
class NxuPhysicsImport
{
protected:
    NxuPhysicsCollection* mCollection;
    NxuFormatHeader mHeader;

    NxuPhysicsModel*         mCurrentModel;
    NxuActorDesc* mCurrentActor;
    void instanceConvexes(NxPhysicsSDK & sdk);
    void instanceSkeletons(NxPhysicsSDK & sdk);
    void instanceTrimeshs(NxPhysicsSDK & sdk);
    void instanceHeightFields(NxPhysicsSDK & sdk);
    void instanceModel(NxScene & scene, NxuPhysicsModel & model, NxMat34 & pose, NxuUserNotify* callback);
public:
    enum SceneType
    {
	    ST_DEFAULT,
	    ST_FORCE_SOFTWARE,
	    ST_FORCE_HARDWARE,
	    ST_LAST
    };

	NxuPhysicsImport(NxPhysicsSDK *sdk);
    virtual ~NxuPhysicsImport();

    /**
    \brief Reads the next physics data structure from the provided stream

    \param[in] userStream The input stream containing physics data.
    \return The type of object read in.

    The data will be converted to a descriptor and added to the physics collection.
    */
    virtual NxuSectionTypes parse(NxuInputStream & userStream);

    /**
    \brief Adds the collection to the provided SDK, including adding scenes

    \param[in] sdk Physics SDK to load the collection into.
    \param[in] notify Optional notification events to the application as the scene is loaded.
    \param[in] create Optional callback if the user wants control over scene creation.
    \param[in] defaultScene Optional NxScene to load the collections default model into.
    \param[in] defaultSceneOffset Optional The placement of the default model within the default scene
    \param[in] stype Override to force creation of a HW or SW scenes

    If defaultScene is NULL,  defaultSceneOffset is ignored.

    */
	void instantiate(NxPhysicsSDK & sdk,             
	                     NxuUserNotify *notify	= 0, 
	                     NxuUserCreate  *create     = 0,
                         NxScene* defaultScene = 0,
                         NxMat34* defaultSceneOffset = 0,
	                     SceneType stype=ST_DEFAULT); 

    /**
    \brief Clears cached values so the subsequent parses will not create links with previous data

    Will delete anything currently in the collection, releasing memory. Pointers previously provided
    during callback notfication will no longer be valid.
    */
	virtual void reset();

    /**
    \brief Obtain a reference to the data read in since the last reset()

    This can be used to obtain the descriptors and manage them directly rather than using the
    instantiate() methods to generate active objects within the SDK.

    \return A pointer to the collection managed by this importer.
    */

    NxuPhysicsCollection*	getCollection();
	static NxPhysicsSDK*			SDK; // Added by Visla this should be set in constructor

    /**
    \brief Set the collection
    */

    void setCollection(NxuPhysicsCollection* collection);
};


#endif //NXU_PHYSICS_IMPORT_H


//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND

