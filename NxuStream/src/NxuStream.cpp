/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		NxuStream.cpp

		Serializable implementations

		Changelist
		----------

*/
#include <stdio.h>

#include "NxuStream.h"
#include "NxuPhysicsImport.h"
#include "NxCooking.h"
#include <windows.h>


//
const char* NxuParameterData::GetParamName(NxParameter p)
{

    const char* param = "UNKNOWN";

	switch ( p )
	{
#if NXU_SDK_FEATURES & NXU_SDK_HAS_230_FLAGS
		case NX_BBOX_NOISE_LEVEL:
			param = "NX_BBOX_NOISE_LEVEL";
			break;
		case NX_SOLVER_CONVERGENCE_THRESHOLD:
			param = "NX_SOLVER_CONVERGENCE_THRESHOLD";
			break;
#endif

#if NXU_SDK_FEATURES & NXU_SDK_HAS_231_FLAGS && NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
		case NX_VISUALIZE_FLUID_DRAINS:
			param = "NX_VISUALIZE_FLUID_DRAINS";
			break;
		case NX_VISUALIZE_FLUID_PACKETS:
			param = "NX_VISUALIZE_FLUID_PACKETS";
			break;
		case NX_VISUALIZE_FLUID_MOTION_LIMIT:
			param = "NX_VISUALIZE_FLUID_MOTION_LIMIT";
			break;
		case NX_VISUALIZE_FLUID_DYN_COLLISION:
			param = "NX_VISUALIZE_FLUID_DYN_COLLISION";
			break;
#endif
#if NXU_SDK_FEATURES & NXU_SDK_HAS_230_FLAGS
		case NX_CCD_EPSILON:
			param = "NX_CCD_EPSILON";
			break;
#endif
		case NX_PENALTY_FORCE:
			param =	"NX_PENALTY_FORCE";	break;
		case NX_SKIN_WIDTH:
			param =	"NX_SKIN_WIDTH"; break;
		case NX_DEFAULT_SLEEP_LIN_VEL_SQUARED:
			param =	"NX_DEFAULT_SLEEP_LIN_VEL_SQUARED";	break;
		case NX_DEFAULT_SLEEP_ANG_VEL_SQUARED:
			param =	"NX_DEFAULT_SLEEP_ANG_VEL_SQUARED";	break;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_230_FLAGS
		case NX_BOUNCE_THRESHOLD:
#else
		case NX_BOUNCE_TRESHOLD:
#endif
			param =	"NX_BOUNCE_THRESHOLD"; break;
		case NX_DYN_FRICT_SCALING:
			param =	"NX_DYN_FRICT_SCALING";	break;
		case NX_STA_FRICT_SCALING:
			param =	"NX_STA_FRICT_SCALING";	break;
		case NX_MAX_ANGULAR_VELOCITY:
			param =	"NX_MAX_ANGULAR_VELOCITY"; break;
		case NX_CONTINUOUS_CD:
			param =	"NX_CONTINUOUS_CD";	break;
		case NX_VISUALIZATION_SCALE:
			param =	"NX_VISUALIZATION_SCALE"; break;
		case NX_VISUALIZE_WORLD_AXES:
			param =	"NX_VISUALIZE_WORLD_AXES"; break;
		case NX_VISUALIZE_BODY_AXES:
			param =	"NX_VISUALIZE_BODY_AXES"; break;
		case NX_VISUALIZE_BODY_MASS_AXES:
			param =	"NX_VISUALIZE_BODY_MASS_AXES"; break;
		case NX_VISUALIZE_BODY_LIN_VELOCITY:
			param =	"NX_VISUALIZE_BODY_LIN_VELOCITY"; break;
		case NX_VISUALIZE_BODY_ANG_VELOCITY:
			param =	"NX_VISUALIZE_BODY_ANG_VELOCITY"; break;
		case NX_VISUALIZE_BODY_LIN_MOMENTUM:
			param =	"NX_VISUALIZE_BODY_LIN_MOMENTUM"; break;
		case NX_VISUALIZE_BODY_ANG_MOMENTUM:
			param =	"NX_VISUALIZE_BODY_ANG_MOMENTUM"; break;
		case NX_VISUALIZE_BODY_LIN_ACCEL:
			param =	"NX_VISUALIZE_BODY_LIN_ACCEL"; break;
		case NX_VISUALIZE_BODY_ANG_ACCEL:
			param =	"NX_VISUALIZE_BODY_ANG_ACCEL"; break;
		case NX_VISUALIZE_BODY_LIN_FORCE:
			param =	"NX_VISUALIZE_BODY_LIN_FORCE"; break;
		case NX_VISUALIZE_BODY_ANG_FORCE:
			param =	"NX_VISUALIZE_BODY_ANG_FORCE"; break;
		case NX_VISUALIZE_BODY_REDUCED:
			param =	"NX_VISUALIZE_BODY_REDUCED"; break;
		case NX_VISUALIZE_BODY_JOINT_GROUPS:
			param =	"NX_VISUALIZE_BODY_JOINT_GROUPS"; break;
		case NX_VISUALIZE_BODY_CONTACT_LIST:
			param =	"NX_VISUALIZE_BODY_CONTACT_LIST"; break;
		case NX_VISUALIZE_BODY_JOINT_LIST:
			param =	"NX_VISUALIZE_BODY_JOINT_LIST";	break;
		case NX_VISUALIZE_BODY_DAMPING:
			param =	"NX_VISUALIZE_BODY_DAMPING"; break;
		case NX_VISUALIZE_BODY_SLEEP:
			param =	"NX_VISUALIZE_BODY_SLEEP"; break;
		case NX_VISUALIZE_JOINT_LOCAL_AXES:
			param =	"NX_VISUALIZE_JOINT_LOCAL_AXES"; break;
		case NX_VISUALIZE_JOINT_WORLD_AXES:
			param =	"NX_VISUALIZE_JOINT_WORLD_AXES"; break;
		case NX_VISUALIZE_JOINT_LIMITS:
			param =	"NX_VISUALIZE_JOINT_LIMITS"; break;
		case NX_VISUALIZE_JOINT_ERROR:
			param =	"NX_VISUALIZE_JOINT_ERROR";	break;
		case NX_VISUALIZE_JOINT_FORCE:
			param =	"NX_VISUALIZE_JOINT_FORCE";	break;
		case NX_VISUALIZE_JOINT_REDUCED:
			param =	"NX_VISUALIZE_JOINT_REDUCED"; break;
		case NX_VISUALIZE_CONTACT_POINT:
			param =	"NX_VISUALIZE_CONTACT_POINT"; break;
		case NX_VISUALIZE_CONTACT_NORMAL:
			param =	"NX_VISUALIZE_CONTACT_NORMAL"; break;
		case NX_VISUALIZE_CONTACT_ERROR:
			param =	"NX_VISUALIZE_CONTACT_ERROR"; break;
		case NX_VISUALIZE_CONTACT_FORCE:
			param =	"NX_VISUALIZE_CONTACT_FORCE"; break;
		case NX_VISUALIZE_ACTOR_AXES:
			param =	"NX_VISUALIZE_ACTOR_AXES"; break;
		case NX_VISUALIZE_COLLISION_AABBS:
			param =	"NX_VISUALIZE_COLLISION_AABBS";	break;
		case NX_VISUALIZE_COLLISION_SHAPES:
			param =	"NX_VISUALIZE_COLLISION_SHAPES"; break;
		case NX_VISUALIZE_COLLISION_AXES:
			param =	"NX_VISUALIZE_COLLISION_AXES"; break;
		case NX_VISUALIZE_COLLISION_COMPOUNDS:
			param =	"NX_VISUALIZE_COLLISION_COMPOUNDS";	break;
		case NX_VISUALIZE_COLLISION_VNORMALS:
			param =	"NX_VISUALIZE_COLLISION_VNORMALS"; break;
		case NX_VISUALIZE_COLLISION_FNORMALS:
			param =	"NX_VISUALIZE_COLLISION_FNORMALS"; break;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_230_FLAGS
		case NX_VISUALIZE_COLLISION_EDGES:
			param =	"NX_VISUALIZE_COLLISION_EDGES";	break;
#endif
		case NX_VISUALIZE_COLLISION_SPHERES:
			param =	"NX_VISUALIZE_COLLISION_SPHERES"; break;
		case NX_VISUALIZE_COLLISION_SAP:
			param =	"NX_VISUALIZE_COLLISION_SAP"; break;
		case NX_VISUALIZE_COLLISION_STATIC:
			param =	"NX_VISUALIZE_COLLISION_STATIC"; break;
		case NX_VISUALIZE_COLLISION_DYNAMIC:
			param =	"NX_VISUALIZE_COLLISION_DYNAMIC"; break;
		case NX_VISUALIZE_COLLISION_FREE:
			param =	"NX_VISUALIZE_COLLISION_FREE"; break;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_230_FLAGS
		case NX_VISUALIZE_COLLISION_CCD:
			param =	"NX_VISUALIZE_COLLISION_CCD"; break;
		case NX_VISUALIZE_COLLISION_SKELETONS:
			param =	"NX_VISUALIZE_COLLISION_SKELETONS";	break;
#endif
#if	NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
		case NX_VISUALIZE_FLUID_EMITTERS:
			param =	"NX_VISUALIZE_FLUID_EMITTERS"; break;
		case NX_VISUALIZE_FLUID_POSITION:
			param =	"NX_VISUALIZE_FLUID_POSITION"; break;
		case NX_VISUALIZE_FLUID_VELOCITY:
			param =	"NX_VISUALIZE_FLUID_VELOCITY"; break;
		case NX_VISUALIZE_FLUID_KERNEL_RADIUS:
			param =	"NX_VISUALIZE_FLUID_KERNEL_RADIUS";	break;
		case NX_VISUALIZE_FLUID_BOUNDS:
			param =	"NX_VISUALIZE_FLUID_BOUNDS"; break;
#endif
		case NX_ADAPTIVE_FORCE:
			param =	"NX_ADAPTIVE_FORCE"; break;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_230_FLAGS
		case NX_COLL_VETO_JOINTED:
			param =	"NX_COLL_VETO_JOINTED";	break;
#endif
#if NXU_SDK_FEATURES & NXU_SDK_HAS_230_FLAGS
		case NX_TRIGGER_TRIGGER_CALLBACK:
			param =	"NX_TRIGGER_TRIGGER_CALLBACK"; break;
		case NX_SELECT_HW_ALGO:
			param =	"NX_SELECT_HW_ALGO"; break;
		case NX_VISUALIZE_ACTIVE_VERTICES:
			param =	"NX_VISUALIZE_ACTIVE_VERTICES";	break;
#endif
#if NXU_SDK_FEATURES & NXU_SDK_HAS_CLOTH_API
	
        case NX_VISUALIZE_CLOTH_COLLISIONS:
            param = "NX_VISUALIZE_CLOTH_COLLISIONS"; break;
	
        case NX_VISUALIZE_CLOTH_SELFCOLLISIONS:
            param = "NX_VISUALIZE_CLOTH_SELFCOLLISIONS"; break;
	
        case NX_VISUALIZE_CLOTH_WORKPACKETS:
            param = "NX_VISUALIZE_CLOTH_WORKPACKETS"; break;
#endif
		case NX_PARAMS_NUM_VALUES:
			param =	"NX_PARAMS_NUM_VALUES";	break;
		case NX_MIN_SEPARATION_FOR_PENALTY:
			param =	"NX_MIN_SEPARATION_FOR_PENALTY"; break;
		default:
			assert(0);
			break;
	}

    return param;
}


NX_BOOL storeSpring(NxuOutputStream &stream, NxSpringDesc & spring, const char* name)
{
    stream.storeFloat(spring.spring, "spring");
    stream.storeFloat(spring.damper, "damper");
    stream.storeFloat(spring.targetValue, "targetValue");

    return (stream.errorCode == NXU_NO_ERROR);
}

NX_BOOL readSpring(NxuInputStream &stream, NxSpringDesc & spring, const char* name)
{
    stream.readFloat(spring.spring, "spring");
    stream.readFloat(spring.damper, "damper");
    stream.readFloat(spring.targetValue, "targetValue");

    return (stream.errorCode == NXU_NO_ERROR);
}

//==================================================================================
//==================================================================================
NxuSectionTypes NxuTriangleMeshDesc::store(NxuOutputStream & stream, const char* name)
{
    NxuFormatHeader header;          
    header.sectionType = NxuTypeTriangleMesh;
    header.size =  sizeof(NxU32)*10 +
                        sizeof(NxF32)*2 +
                        numTriangles*materialIndexStride +
                        numVertices*pointStrideBytes +
                        numTriangles*triangleStrideBytes +
                        mCookedDataSize +
                        mPmapSize;
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

    stream.storeBool(materialIndices ? true : false, "hasMaterialIndicies");
    if (materialIndices)
    {
        stream.storeDword(materialIndexStride, "materialIndexStride");
        stream.storeDword(numTriangles, "numMaterialIndices");

		if ( materialIndexStride == 4 )
        {
            stream.storeDwordBuffer((void*)materialIndices, numTriangles*materialIndexStride, "materialIndices");
        } 
        else if  ( materialIndexStride == 2 )
        {
            stream.storeWordBuffer((void*)materialIndices, numTriangles*materialIndexStride, "materialIndices");
        }
        else if ( materialIndexStride == 1 )
        {
            stream.storeByteBuffer((void*)materialIndices, numTriangles*materialIndexStride, "materialIndices");
        }
		else
		{
			// uh, what?
			assert( 0 );
		}
	}

    NxU32 axisEnum = (NxU32)heightFieldVerticalAxis;
    stream.storeDword(axisEnum, "heightFieldVerticalAxis");
    stream.storeFloat(heightFieldVerticalExtent, "heightFieldVerticalExtent");
    stream.storeFloat(convexEdgeThreshold, "convexEdgeThreshold");

    //Store flags
    stream.storeDword(flags, "flags");

    stream.storeDword(numVertices, "numVertices");
    stream.storeDword(pointStrideBytes, "pointStrideBytes");
    if (pointStrideBytes == 3*sizeof(NxF32)) 
    {
        stream.storeFloatBuffer((void*)(points), numVertices*pointStrideBytes, "points");
    }
    else if (pointStrideBytes == 3*sizeof(NxF64))
    {
        stream.storeDoubleBuffer((void*)(points), numVertices*pointStrideBytes, "points");
    }
    else
    {
        //??
        assert(0);
    }

    stream.storeDword(numTriangles, "numTriangles");
    stream.storeDword(triangleStrideBytes, "triangleStrideBytes");
    if (triangleStrideBytes/3	== 1)
    {
        stream.storeByteBuffer((void*)(triangles), numTriangles*triangleStrideBytes, "triangles");
    }
    else if (triangleStrideBytes/3 ==	2)
    {
        stream.storeWordBuffer((void*)(triangles), numTriangles*triangleStrideBytes, "triangles");
    } 
    else if (triangleStrideBytes/3 ==	4)
    {
      stream.storeDwordBuffer((void*)(triangles), numTriangles*triangleStrideBytes, "triangles");
    }
    else
    {
        assert(false);
    }

    stream.storeDword(mCookedDataSize, "mCookedDataSize");
    if (mCookedDataSize)
    {
        stream.storeByteBuffer(mCookedData, mCookedDataSize, "mCookedData");
    }

    stream.storeDword(mPmapSize, "mPmapSize");
    if (mPmapSize)
    {
        stream.storeByteBuffer(mPmap, mPmapSize, "mPmap");
    }
    stream.endSection();

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeTriangleMesh : NxuTypeUnknown;
}

NxuSectionTypes NxuTriangleMeshDesc::read(NxuInputStream & stream, const char* name)
{
    NX_BOOL hasMaterialIndicies;
    stream.readBool(hasMaterialIndicies, "hasMaterialIndicies");

    if (hasMaterialIndicies)
    {
        stream.readDword(materialIndexStride, "materialIndexStride");
        stream.readDword(numTriangles, "numMaterialIndices");
        NxU8* materialBuffer = new NxU8[numTriangles*materialIndexStride];

		if ( materialIndexStride == 4 )
        {
            stream.readDwordBuffer(materialBuffer, numTriangles*materialIndexStride, "Material Indexes");
        } 
        else if  ( materialIndexStride == 2 )
        {
            stream.readWordBuffer(materialBuffer, numTriangles*materialIndexStride, "Material Indexes");
        }
        else if ( materialIndexStride == 1 )
        {
            stream.readByteBuffer(materialBuffer, numTriangles*materialIndexStride, "Material Indexes");
        }
		else
		{
			// uh, what?
			assert( 0 );
		}
        materialIndices = materialBuffer;
	}
    else
    {
        numTriangles = 0;
        materialIndices = 0;
    }

    NxU32 axisEnum;
    stream.readDword(axisEnum, "heightFieldVerticalAxis");
    heightFieldVerticalAxis = (NxHeightFieldAxis) axisEnum;
    stream.readFloat(heightFieldVerticalExtent, "heightFieldVerticalExtent");
    stream.readFloat(convexEdgeThreshold, "convexEdgeThreshold");

    stream.readDword(flags, "flags");
    stream.readDword(numVertices, "numVertices");
    stream.readDword(pointStrideBytes, "pointStrideBytes");

    NxU8* pointBuffer = new NxU8[numVertices*pointStrideBytes];
    if (pointStrideBytes == 3*sizeof(NxF32)) 
    {
        stream.readFloatBuffer(pointBuffer, numVertices*pointStrideBytes, "points");
    }
    else if (pointStrideBytes == 3*sizeof(NxF64))
    {
        stream.readDoubleBuffer(pointBuffer, numVertices*pointStrideBytes, "points");
    }
    else
    {
        if (pointBuffer)
        {
            delete pointBuffer;   
            pointBuffer = 0;
        }
        //??
        assert(0);
    }
    points = (void*)pointBuffer;

    stream.readDword(numTriangles, "numTriangles");
    stream.readDword(triangleStrideBytes, "triangleStrideBytes");

    NxU8* triangleBuffer = new NxU8[numTriangles*triangleStrideBytes];;
    if (triangleStrideBytes/3	== 1)
    {
        stream.readByteBuffer((void*)(triangleBuffer), numTriangles*triangleStrideBytes, "triangles");
    }
    else if (triangleStrideBytes/3 ==	2)
    {
        stream.readWordBuffer((void*)(triangleBuffer), numTriangles*triangleStrideBytes, "triangles");
    } 
    else if (triangleStrideBytes/3 ==	4)
    {
      stream.readDwordBuffer((void*)(triangleBuffer), numTriangles*triangleStrideBytes, "triangles");
    }
    else
    {
        if (triangleBuffer)
        {
            delete triangleBuffer;   
            triangleBuffer = 0;
        }
        assert(false);
    }
    triangles = triangleBuffer;

	NxTriangleMesh *tm;

    stream.readDword(mCookedDataSize, "mCookedDataSize");
    if (mCookedDataSize)
    {
        mCookedData = new NxU8[mCookedDataSize];
        stream.readByteBuffer(mCookedData, mCookedDataSize, "mCookedData");

		// we have cooked just make convexmesh.
		// added by Visla		        
        ImportMemoryReadBuffer readBuffer((const NxU8*)mCookedData);
		tm = NxuPhysicsImport::SDK->createTriangleMesh( readBuffer);
		// ------------------
    }
    else
    {
		// it is not yet cooked we need to cook mesh. This is when importing from Max
		// Added by Visla 
		bool status;
		NxInitCooking();
        ImportMemoryWriteBuffer writeBuffer;
 		status = NxCookTriangleMesh( *this, writeBuffer );
        if ( status	)
		{
			tm = NxuPhysicsImport::SDK->createTriangleMesh( ImportMemoryReadBuffer(writeBuffer.data)	);
        }
		else
		{
			tm = 0;
		}
		// ----------
		mCookedData = new NxU8[writeBuffer.currentSize]; 
		memcpy(mCookedData, writeBuffer.data, writeBuffer.currentSize);		
		mCookedDataSize = writeBuffer.currentSize;
		//mCookedDataSize = 0;
    }

    
    stream.readDword(mPmapSize, "mPmapSize");
    if (mPmapSize)
    {
        mPmap = new NxU8[mPmapSize];
        stream.readByteBuffer(mPmap, mPmapSize, "mPmap");
    }
    else
    {
        mPmap = 0;
    }

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeTriangleMesh : NxuTypeUnknown;
}

//==================================================================================
//==================================================================================
NxuSectionTypes NxuConvexMeshDesc::store(NxuOutputStream & stream, const char* name)
{
	char tmp[100];
	sprintf(tmp, "Snimam cooked data size: %d", mCookedDataSize);
	//MessageBox(0, tmp, 0, 0);

    NxuFormatHeader header;
    header.sectionType = NxuTypeConvexMesh;
    header.size  = sizeof(NxU32)*6 + 
                        numVertices*pointStrideBytes + 
                        numTriangles*triangleStrideBytes + 
                        mCookedDataSize;
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

    //Store flags
    stream.storeDword(flags, "flags");

    stream.storeDword(numVertices, "numVertices");
    stream.storeDword(pointStrideBytes, "pointStrideBytes");
    stream.storeFloatBuffer((void*)(points), numVertices*pointStrideBytes, "points");

    stream.storeDword(numTriangles, "numTriangles");
    stream.storeDword(triangleStrideBytes, "triangleStrideBytes");
    if (flags	& NX_MF_16_BIT_INDICES) {
        stream.storeWordBuffer((void*)(triangles), numTriangles*triangleStrideBytes, "triangles");
    } else {
        stream.storeDwordBuffer((void*)(triangles), numTriangles*triangleStrideBytes, "triangles");
    }

	//mCookedDataSize = 0;
    stream.storeDword(mCookedDataSize, "mCookedDataSize");
    if (mCookedDataSize)
    {
        stream.storeByteBuffer(mCookedData, mCookedDataSize, "mCookedData");
    }
    stream.endSection();

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeConvexMesh : NxuTypeUnknown;
}

NxuSectionTypes NxuConvexMeshDesc::read(NxuInputStream & stream, const char* name)
{
    stream.readDword(flags, "flags");

    stream.readDword(numVertices, "numVertices");
    stream.readDword(pointStrideBytes, "pointStrideBytes");
    NxU8* pointBuffer = new NxU8[numVertices*pointStrideBytes];
    stream.readFloatBuffer(pointBuffer, numVertices*pointStrideBytes, "points");
    points = pointBuffer;

    stream.readDword(numTriangles, "numTriangles");
    stream.readDword(triangleStrideBytes, "triangleStrideBytes");
    NxU8* triangleBuffer = new NxU8[numTriangles*triangleStrideBytes];
    
    if (flags	& NX_MF_16_BIT_INDICES) {
        stream.readWordBuffer(triangleBuffer, numTriangles*triangleStrideBytes, "triangles");
    } else {
        stream.readDwordBuffer(triangleBuffer, numTriangles*triangleStrideBytes, "triangles");
    }
    triangles = triangleBuffer;


	NxConvexMesh *cm;

    stream.readDword(mCookedDataSize, "mCookedDataSize");
    if (mCookedDataSize)
    {
        mCookedData = new NxU8[mCookedDataSize];
        stream.readByteBuffer(mCookedData, mCookedDataSize, "mCookedData");

		// we have cooked just make convexmesh.
		// added by Visla		        
        ImportMemoryReadBuffer readBuffer((const NxU8*)mCookedData);
        cm = NxuPhysicsImport::SDK->createConvexMesh( readBuffer);
		// ------------------
    }
    else
    {
		// it is not yet cooked we need to cook mesh. This is when importing from Max
		// Added by Visla 
		bool status;
		NxInitCooking();
        ImportMemoryWriteBuffer writeBuffer;
 		status = NxCookConvexMesh( *this, writeBuffer );
        if ( status	)
		{
			cm = NxuPhysicsImport::SDK->createConvexMesh( ImportMemoryReadBuffer(writeBuffer.data)	);
        }
		else
		{
			cm = 0;
		}
		// ----------
		//mCookedData = new NxU8[writeBuffer.currentSize]; 
		//memcpy(mCookedData, writeBuffer.data, writeBuffer.currentSize);		
		//mCookedDataSize = writeBuffer.currentSize;
		mCookedDataSize = 0;
    }
    mInstance = cm; // add by Visla


    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeConvexMesh : NxuTypeUnknown;
}

#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
//==================================================================================
//==================================================================================
NxuSectionTypes NxuHeightFieldDesc::store(NxuOutputStream & stream, const char* name)
{
    NxuFormatHeader header;          
    header.sectionType = NxuTypeHeightField;
    header.size =  0; //TODO
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

    stream.storeDword(nbRows, "nbRows");
    stream.storeDword(nbColumns, "nbColumns");
    stream.storeDword((NxU32)format, "format");
    stream.storeDword(sampleStride, "sampleStride");
    stream.storeBool(samples ? true : false, "hasSamples");
    if (samples)
    {
        stream.storeByteBuffer(samples, nbRows*nbColumns*sampleStride, "samples");
    }
    stream.storeFloat(verticalExtent, "verticalExtent");
    stream.storeFloat(convexEdgeThreshold, "convexEdgeThreshold");
    stream.storeDword(flags, "flags");

    stream.endSection();

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeHeightField : NxuTypeUnknown;
}

NxuSectionTypes NxuHeightFieldDesc::read(NxuInputStream & stream, const char* name)
{
    stream.readDword(nbRows, "nbRows");
    stream.readDword(nbColumns, "nbColumns");
    NxU32 dummy;
    stream.readDword(dummy, "format");
    format = (NxHeightFieldFormat)dummy;
    stream.readDword(sampleStride, "sampleStride");
    NX_BOOL hasSamples;
    stream.readBool(hasSamples, "hasSamples");
    if (hasSamples)
    {
        samples = new NxU8[nbRows*nbColumns*sampleStride];
        stream.readByteBuffer(samples, nbRows*nbColumns*sampleStride, "samples");
    }
    stream.readFloat(verticalExtent, "verticalExtent");
    stream.readFloat(convexEdgeThreshold, "convexEdgeThreshold");
    stream.readDword(flags, "flags");

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeHeightField : NxuTypeUnknown;
}
#endif

//
NxuSectionTypes NxuCCDSkeletonDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuFormatHeader header;
    header.sectionType = NxuTypeCCDSkeleton;
    header.size =  sizeof(NxU32)*6 + 
                        numVertices*pointStrideBytes +
                        numTriangles*triangleStrideBytes +
                        mCookedDataSize;
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);


    //Store flags
    stream.storeDword(flags, "flags");

    stream.storeDword(numVertices, "numVertices");
    if (numVertices)
    {
        stream.storeDword(pointStrideBytes, "pointStrideBytes");
        stream.storeFloatBuffer((void*)(points), numVertices*pointStrideBytes, "points");
    }

    stream.storeDword(numTriangles, "numTriangles");
    if (numTriangles)
    {
        stream.storeDword(triangleStrideBytes, "triangleStrideBytes");
        if (flags	& NX_MF_16_BIT_INDICES) {
            stream.storeWordBuffer((void*)(triangles), numTriangles*triangleStrideBytes, "triangles");
        } else {
            stream.storeDwordBuffer((void*)(triangles), numTriangles*triangleStrideBytes, "triangles");
        }
    }

    stream.storeDword(mCookedDataSize, "mCookedDataSize");
    if (mCookedDataSize)
    {
        stream.storeByteBuffer(mCookedData, mCookedDataSize, "mCookedData");
    }

    stream.endSection();

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeCCDSkeleton : NxuTypeUnknown;
};

NxuSectionTypes NxuCCDSkeletonDesc::read(NxuInputStream &stream, const char* name)
{
    //Store flags
    stream.readDword(flags, "flags");

    stream.readDword(numVertices, "numVertices");
    if (numVertices)
    {
        stream.readDword(pointStrideBytes, "pointStrideBytes");
        NxU8* pointBuffer = new NxU8[numVertices*pointStrideBytes];
        stream.readFloatBuffer((void*)(pointBuffer), numVertices*pointStrideBytes, "points");
        points = pointBuffer;
    }

    stream.readDword(numTriangles, "numTriangles");
    if (numTriangles)
    {
        stream.readDword(triangleStrideBytes, "triangleStrideBytes");
        NxU8* triangleBuffer = new NxU8[numTriangles*triangleStrideBytes];
        if (flags	& NX_MF_16_BIT_INDICES) {
            stream.readWordBuffer((void*)(triangleBuffer), numTriangles*triangleStrideBytes, "triangles");
        } else {
            stream.readDwordBuffer((void*)(triangleBuffer), numTriangles*triangleStrideBytes, "triangles");
        }
        triangles = triangleBuffer;
    }

    stream.readDword(mCookedDataSize, "mCookedDataSize");
    if (mCookedDataSize)
    {
        mCookedData = new NxU8[mCookedDataSize];
        stream.readByteBuffer(mCookedData, mCookedDataSize, "mCookedData");
    }
    else
    {
        mCookedData = 0;
    }

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeCCDSkeleton : NxuTypeUnknown;
};


NxuSectionTypes NxuMaterialDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuFormatHeader header;
    header.sectionType = NxuTypeMaterial;
    header.size =  sizeof(NxF32)*5 + sizeof(NxU32)*3 + sizeof(NxVec3) + sizeof(bool) + (spring ? sizeof(NxSpringDesc) : 0);
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

    stream.storeWord(index, "index");

    stream.storeFloat(dynamicFriction, "dynamicFriction");
    stream.storeFloat(staticFriction, "staticFriction");
    stream.storeFloat(restitution, "restitution");
    stream.storeFloat(dynamicFrictionV, "dynamicFrictionV");
    stream.storeFloat(staticFrictionV, "staticFrictionV");

    stream.storeDword(frictionCombineMode, "frictionCombineMode");
    stream.storeDword(restitutionCombineMode, "restitutionCombineMode");

    stream.storeVector(dirOfAnisotropy, "dirOfAnisotropy");
    stream.storeDword(flags, "flags");

    stream.storeBool(spring ? true : false, "hasSpring");
    if (spring)
    {
        storeSpring(stream, *spring, "spring");
    }

    stream.endSection();

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeMaterial : NxuTypeUnknown;
};

NxuSectionTypes NxuMaterialDesc::read(NxuInputStream &stream, const char* name)
{
    stream.readWord(index, "index");

    stream.readFloat(dynamicFriction, "dynamicFriction");
    stream.readFloat(staticFriction, "staticFriction");
    stream.readFloat(restitution, "restitution");
    stream.readFloat(dynamicFrictionV, "dynamicFrictionV");
    stream.readFloat(staticFrictionV, "staticFrictionV");

    stream.readDword(*((NxU32*)&frictionCombineMode), "frictionCombineMode");
    stream.readDword(*((NxU32*)&restitutionCombineMode), "restitutionCombineMode");

    stream.readVector(dirOfAnisotropy, "dirOfAnisotropy");
    stream.readDword(flags, "flags");

    NX_BOOL hasSpring;
    stream.readBool(hasSpring, "hasSpring");
    if (hasSpring)
    {
        spring = new NxSpringDesc();
        readSpring(stream, *spring, "spring");
    }
    else
    {
        spring = 0;
    }

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeMaterial : NxuTypeUnknown;
};


NxuSectionTypes NxuShapeDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuFormatHeader header;

    header.sectionType = NxuTypeShape;
    header.size =  0; //Can't predict the size of a shape well
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

    NxShapeDesc* shapeDesc = getShapeDesc();
    stream.storeDword((NxU32)shapeDesc->getType(), "type");
    stream.storeMatrix(shapeDesc->localPose, "localPose");
    stream.storeDword(shapeDesc->group, "group");
    stream.storeDword(shapeDesc->materialIndex, "materialIndex");

#if NXU_SDK_FEATURES & NXU_SDK_HAS_PER_SHAPE_MASS
    stream.storeFloat(shapeDesc->mass, "mass");
    stream.storeFloat(shapeDesc->density, "density");
#endif

    stream.storeFloat(shapeDesc->skinWidth, "skinWidth");

    stream.storeDword(shapeDesc->shapeFlags, "shapeFlags");


#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
    stream.storeDword(stream.indexLookup.pointerToIndex(mCCDSkeleton), "mCCDSkeleton");
#endif

#if NXU_SDK_FEATURES & NXU_SDK_HAS_BITMASK_FILTER
    stream.storeDword(shapeDesc->groupsMask.bits0, "groupsMask.bits0");
    stream.storeDword(shapeDesc->groupsMask.bits1, "groupsMask.bits1");
    stream.storeDword(shapeDesc->groupsMask.bits2, "groupsMask.bits2");
    stream.storeDword(shapeDesc->groupsMask.bits3, "groupsMask.bits3");
#endif
    char tempString[NXU_STREAM_MAX_STRING_LENGTH];
    memset(tempString, 0, NXU_STREAM_MAX_STRING_LENGTH);
    sprintf( tempString, shapeDesc->name ? shapeDesc->name :	"null" );
    stream.storeCharBuffer((void*)&tempString, NXU_STREAM_MAX_STRING_LENGTH, "name");

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

NxuSectionTypes NxuShapeDesc::read(NxuInputStream &stream, const char* name)
{
    NxShapeDesc* shapeDesc = getShapeDesc();

    stream.readMatrix(shapeDesc->localPose, "localPose");
    stream.readDword(*((NxU32*)&shapeDesc->group), "group");
    stream.readDword(*((NxU32*)&shapeDesc->materialIndex), "materialIndex");

    //TODO: clean up for mismatched versions
#if NXU_SDK_FEATURES & NXU_SDK_HAS_PER_SHAPE_MASS
    if (stream.getSDKVersion() & NXU_SDK_HAS_PER_SHAPE_MASS)
    {
        stream.readFloat(shapeDesc->mass, "mass");
        stream.readFloat(shapeDesc->density, "density");
    }
    else
    {
        shapeDesc->mass = 0;
        shapeDesc->density = 0;
    }
    if(shapeDesc->mass > 0) shapeDesc->density = 0;
#else
    if (stream.getSDKVersion() & NXU_SDK_HAS_PER_SHAPE_MASS)
    {
        NxF32 dummy;
        stream.readFloat(dummy, "mass");
        stream.readFloat(dummy, "density");
    }
#endif

    stream.readFloat(shapeDesc->skinWidth, "skinWidth");

    stream.readDword(shapeDesc->shapeFlags, "shapeFlags");


#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
    if (stream.getSDKVersion() & NXU_SDK_HAS_SKELETONS)
    {
        NxU32 index;
        stream.readDword(index, "mCCDSkeleton");
        stream.pointerLookup.indexToPointer(index, &mCCDSkeleton);
    }
    else
    {
        mCCDSkeleton = 0;
    }
#else
    if (stream.getSDKVersion() & NXU_SDK_HAS_SKELETONS)
    {
        NxU32 dummy;
        stream.readDword(dummy, "mCCDSkeleton");
    }
#endif

#if NXU_SDK_FEATURES & NXU_SDK_HAS_BITMASK_FILTER
    if (stream.getSDKVersion() & NXU_SDK_HAS_BITMASK_FILTER)
    {
        stream.readDword(shapeDesc->groupsMask.bits0, "groupsMask.bits0");
        stream.readDword(shapeDesc->groupsMask.bits1, "groupsMask.bits1");
        stream.readDword(shapeDesc->groupsMask.bits2, "groupsMask.bits2");
        stream.readDword(shapeDesc->groupsMask.bits3, "groupsMask.bits3");
    }
    else
    {
        shapeDesc->groupsMask.bits0 = 0;
        shapeDesc->groupsMask.bits1 = 0;
        shapeDesc->groupsMask.bits2 = 0;
        shapeDesc->groupsMask.bits3 = 0;
    }
#else
    if (stream.getSDKVersion() & NXU_SDK_HAS_BITMASK_FILTER)
    {
        NxU32 dummy;
        stream.readDword(dummy, "groupsMask.bits0");
        stream.readDword(dummy, "groupsMask.bits1");
        stream.readDword(dummy, "groupsMask.bits2");
        stream.readDword(dummy, "groupsMask.bits3");
    }
#endif
    char* tempString = new char[NXU_STREAM_MAX_STRING_LENGTH];
    memset(tempString, 0, NXU_STREAM_MAX_STRING_LENGTH);
    shapeDesc->name = tempString;
    stream.readCharBuffer(tempString, NXU_STREAM_MAX_STRING_LENGTH, "name");

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;

};

NxuSectionTypes NxuPlaneShapeDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuShapeDesc::store(stream, name);
    stream.storeVector(normal, "normal");
    stream.storeFloat(d, "d");
    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

NxuSectionTypes NxuPlaneShapeDesc::read(NxuInputStream &stream, const char* name)
{
    NxuShapeDesc::read(stream, name);
    stream.readVector(normal, "normal");
    stream.readFloat(d, "d");
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

NxuSectionTypes NxuSphereShapeDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuShapeDesc::store(stream, name);
    stream.storeFloat(radius, "radius");
    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

NxuSectionTypes NxuSphereShapeDesc::read(NxuInputStream &stream, const char* name)
{
    NxuShapeDesc::read(stream, name);
    stream.readFloat(radius, "radius");
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

NxuSectionTypes NxuBoxShapeDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuShapeDesc::store(stream, name);
    stream.storeVector(dimensions, "dimensions");
    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

NxuSectionTypes NxuBoxShapeDesc::read(NxuInputStream &stream, const char* name)
{
    NxuShapeDesc::read(stream, name);
    stream.readVector(dimensions, "dimensions");
	char tmp[100];
	sprintf(tmp, "%3.3f, %3.3f, %3.3f", dimensions.x, dimensions.y, dimensions.z);
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

NxuSectionTypes NxuCapsuleShapeDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuShapeDesc::store(stream, name);
    stream.storeFloat(radius, "radius");
    stream.storeFloat(height, "height");
    stream.storeDword(flags, "flags");
    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

NxuSectionTypes NxuCapsuleShapeDesc::read(NxuInputStream &stream, const char* name)
{
    NxuShapeDesc::read(stream, name);
    stream.readFloat(radius, "radius");
    stream.readFloat(height, "height");
    stream.readDword(flags, "flags");
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

NxuSectionTypes NxuConvexShapeDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuShapeDesc::store(stream, name);
    stream.storeDword(stream.indexLookup.pointerToIndex(mConvexMeshDesc), "mConvexMeshDesc");
    stream.storeDword(meshFlags, "meshFlags");
#if NXU_SDK_FEATURES & NXU_SDK_HAS_CONVEX_SCALE
    stream.storeFloat(scale, "scale");
#endif

    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
	
//	// Edited by Visla
//    NxuShapeDesc::store(stream, name);
//    stream.storeDword(0xffffffff, "mConvexMeshDesc");
//	mConvexMeshDesc->store(stream, "mConvexMeshDesc");
//
//    stream.storeDword(meshFlags, "meshFlags");
//#if NXU_SDK_FEATURES & NXU_SDK_HAS_CONVEX_SCALE
//    stream.storeFloat(scale, "scale");
//#endif
//
//    stream.endSection();
//    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
	// -----------------------------
};

NxuSectionTypes NxuConvexShapeDesc::read(NxuInputStream &stream, const char* name)
{
    NxuShapeDesc::read(stream, name);
    NxU32 index;
    stream.readDword(index, "mConvexMeshDesc");
	// add by Visla
	//if (index == 0xffffffff)
	//{
	//	mConvexMeshDesc = new NxuConvexMeshDesc;
	//	mConvexMeshDesc->read(stream, "mConvexMeshDesc");
	//} else
	//	stream.pointerLookup.indexToPointer(index, &mConvexMeshDesc);
	//// -------------
	stream.pointerLookup.indexToPointer(index, &mConvexMeshDesc);

    stream.readDword(meshFlags, "meshFlags");
#if NXU_SDK_FEATURES & NXU_SDK_HAS_CONVEX_SCALE
    if (stream.getSDKVersion() & NXU_SDK_HAS_CONVEX_SCALE)
    {
        stream.readFloat(scale, "scale");
    }
    else
    {
        scale = 1.0f;
    }
#else
    if (stream.getSDKVersion() & NXU_SDK_HAS_CONVEX_SCALE)
    {
        NxF32 ignore;
        stream.readFloat(ignore, "scale");
    }
#endif

	meshData = mConvexMeshDesc->mInstance;
	return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
}

#if NXU_SDK_FEATURES & NXU_SDK_HAS_WHEEL
NX_BOOL storeTireFunction(NxuOutputStream &stream, NxTireFunctionDesc & func, const char* name)
{
    stream.storeFloat(func.extremumSlip, "extremumSlip");
    stream.storeFloat(func.extremumValue, "extremumValue");
    stream.storeFloat(func.asymptoteSlip, "asymptoteSlip");
    stream.storeFloat(func.asymptoteValue, "asymptoteValue");
    stream.storeFloat(func.stiffnessFactor, "stiffnessFactor");

    return (stream.errorCode == NXU_NO_ERROR);
}

NX_BOOL readTireFunction(NxuInputStream &stream, NxTireFunctionDesc & func, const char* name)
{
    stream.readFloat(func.extremumSlip, "extremumSlip");
    stream.readFloat(func.extremumValue, "extremumValue");
    stream.readFloat(func.asymptoteSlip, "asymptoteSlip");
    stream.readFloat(func.asymptoteValue, "asymptoteValue");
    stream.readFloat(func.stiffnessFactor, "stiffnessFactor");

    return (stream.errorCode == NXU_NO_ERROR);
}

NxuSectionTypes NxuWheelShapeDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuShapeDesc::store(stream, name);
    stream.storeFloat(radius, "radius");
    stream.storeFloat(suspensionTravel, "suspensionTravel");
    storeSpring(stream, suspension, "suspension");
    storeTireFunction(stream, longitudalTireForceFunction, "longitudalTireForceFunction");
    storeTireFunction(stream, lateralTireForceFunction, "lateralTireForceFunction");
    stream.storeFloat(inverseWheelMass, "inverseWheelMass");
    stream.storeDword(wheelFlags, "wheelFlags");
    stream.storeFloat(motorTorque, "motorTorque");
    stream.storeFloat(brakeTorque, "brakeTorque");
    stream.storeFloat(steerAngle, "steerAngle");

    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

NxuSectionTypes NxuWheelShapeDesc::read(NxuInputStream &stream, const char* name)
{
    NxuShapeDesc::read(stream, name);
    stream.readFloat(radius, "radius");
    stream.readFloat(suspensionTravel, "suspensionTravel");
    readSpring(stream, suspension, "suspension");
    readTireFunction(stream, longitudalTireForceFunction, "longitudalTireForceFunction");
    readTireFunction(stream, lateralTireForceFunction, "lateralTireForceFunction");
    stream.readFloat(inverseWheelMass, "inverseWheelMass");
    stream.readDword(wheelFlags, "wheelFlags");
    stream.readFloat(motorTorque, "motorTorque");
    stream.readFloat(brakeTorque, "brakeTorque");
    stream.readFloat(steerAngle, "steerAngle");

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};
#endif

NxuSectionTypes NxuTriangleMeshShapeDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuShapeDesc::store(stream, name);
    stream.storeDword(stream.indexLookup.pointerToIndex(mTriangleMeshDesc), "mTriangleMeshDesc");
    stream.storeDword(meshFlags, "meshFlags");
#if NXU_SDK_FEATURES & NXU_SDK_HAS_CONVEX_SCALE
    stream.storeFloat(scale, "scale");
#endif
    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

NxuSectionTypes NxuTriangleMeshShapeDesc::read(NxuInputStream &stream, const char* name)
{
    NxuShapeDesc::read(stream, name);
    NxU32 index;
    stream.readDword(index, "mTriangleMeshDesc");
    stream.pointerLookup.indexToPointer(index, &mTriangleMeshDesc);
    stream.readDword(meshFlags, "meshFlags");
#if NXU_SDK_FEATURES & NXU_SDK_HAS_CONVEX_SCALE 
    if (stream.getSDKVersion() & NXU_SDK_HAS_CONVEX_SCALE)
    {
        stream.readFloat(scale, "scale");
    }
    else
    {
        scale = 1.0f;
    }
#else
    if (stream.getSDKVersion() & NXU_SDK_HAS_CONVEX_SCALE)
    {
        NxF32 ignore;
        stream.readFloat(ignore, "scale");
    }
#endif
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
NxuSectionTypes NxuHeightFieldShapeDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuShapeDesc::store(stream, name);
    stream.storeDword(stream.indexLookup.pointerToIndex(mHeightFieldDesc), "mHeightFieldDesc");
    stream.storeFloat(heightScale, "heightScale");
    stream.storeFloat(rowScale, "rowScale");
    stream.storeFloat(columnScale, "columnScale");
    stream.storeWord(materialIndexHighBits, "materialIndexHighBits");
    stream.storeWord(holeMaterial, "holeMaterial");
    stream.storeDword(meshFlags, "meshFlags");
    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};

NxuSectionTypes NxuHeightFieldShapeDesc::read(NxuInputStream &stream, const char* name)
{
    NxuShapeDesc::read(stream, name);
    NxU32 index;
    stream.readDword(index, "mHeightFieldDesc");
    stream.pointerLookup.indexToPointer(index, &mHeightFieldDesc);
    stream.readFloat(heightScale, "heightScale");
    stream.readFloat(rowScale, "rowScale");
    stream.readFloat(columnScale, "columnScale");
    stream.readWord(materialIndexHighBits, "materialIndexHighBits");
    stream.readWord(holeMaterial, "holeMaterial");
    stream.readDword(meshFlags, "meshFlags");
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeShape : NxuTypeUnknown;
};
#endif

NxuActorDesc::~NxuActorDesc()
{
    while( !shapes.isEmpty())
    {
        NxShapeDesc* d = shapes.back();
        shapes.popBack();
        delete d;
    }
}

NxuSectionTypes NxuActorDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuFormatHeader header;
    header.sectionType = NxuTypeActor;
    header.size =  0;
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

    stream.storeMatrix(globalPose, "globalPose");
    NX_BOOL hasBody = mBody.isValid();
    stream.storeBool( hasBody, "hasBody");
    if (hasBody) 
    {
        stream.storeMatrix(mBody.massLocalPose, "mBody.massLocalPose");
        stream.storeVector(mBody.massSpaceInertia, "mBody.massSpaceInertia");
        stream.storeFloat(mBody.mass, "mBody.mass");
        stream.storeVector(mBody.linearVelocity, "mBody.linearVelocity");
        stream.storeVector(mBody.angularVelocity, "mBody.angularVelocity");
        stream.storeFloat(mBody.wakeUpCounter, "mBody.wakeUpCounter");
        stream.storeFloat(mBody.linearDamping, "mBody.linearDamping");
        stream.storeFloat(mBody.angularDamping, "mBody.angularDamping");
        stream.storeFloat(mBody.maxAngularVelocity, "mBody.maxAngularVelocity");
    #if  NXU_SDK_FEATURES & NXU_SDK_HAS_CCD_THRESHOLD
        stream.storeFloat(mBody.CCDMotionThreshold, "mBody.CCDMotionThreshold");
    #endif
        stream.storeDword(mBody.flags, "mBody.flags");
        stream.storeFloat(mBody.sleepLinearVelocity, "mBody.sleepLinearVelocity");
        stream.storeFloat(mBody.sleepAngularVelocity, "mBody.sleepAngularVelocity");
        stream.storeDword(mBody.solverIterationCount, "mBody.solverIterationCount");
    }

	// this is a HACK - if you take a look in NxBodyDesc::isValidInternal(),	there is a check to	make sure
	// that	a body description does	NOT	have both a	mass and density (actually is a	little more	involved),
	// so we HACK it here to zero out the density if we	have a body	and	it has a mass
	if ( hasBody && ( mBody.mass >	0 )	)
	{
		// gotta make sure the density is 0
		density	= 0;
	}

    stream.storeFloat(density, "density");

    stream.storeDword(group, "group");
    stream.storeDword(flags, "flags");
	stream.storeDword(*((NxU32*)&userData), "userData");

	char buff[NXU_STREAM_MAX_STRING_LENGTH];
    memset(buff, 0, NXU_STREAM_MAX_STRING_LENGTH);
	sprintf( buff, name	? name : "null"	);
    stream.storeCharBuffer((void*)buff, NXU_STREAM_MAX_STRING_LENGTH, "name");

    NxU32 numShapes = shapes.size();
    stream.storeDword(numShapes, "numShapes");

	// write out each shape
    char tempString[1024];
	for	( NxU32	i=0; i<numShapes; ++i )
	{
        sprintf(tempString, "Shape_%d",i);
        NxShapeDesc* sh = shapes[i];
        NxuShapeDesc* s;
        switch(sh->getType())
        {
        case NX_SHAPE_PLANE:
            {
                s = (NxuPlaneShapeDesc*)sh;
            }
        break;
        case NX_SHAPE_SPHERE:
            {
                s = (NxuSphereShapeDesc*)sh;
            }
            break;
        case NX_SHAPE_BOX:
            {
                s = (NxuBoxShapeDesc*)sh;
            }
            break;
        case NX_SHAPE_CAPSULE:
            {
                s = (NxuCapsuleShapeDesc*)sh;
            }
            break;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_WHEEL
        case NX_SHAPE_WHEEL:
            {
                s = (NxuWheelShapeDesc*)sh;
            }
            break;
#endif
        case NX_SHAPE_CONVEX:
            {
                s = (NxuConvexShapeDesc*)sh;
            }
            break;
        case NX_SHAPE_MESH:
            {
                s = (NxuTriangleMeshShapeDesc*)sh;
            }
            break;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
        case NX_SHAPE_HEIGHTFIELD:
            {
                s = (NxuHeightFieldShapeDesc*)sh;
            }
            break;
#endif
        default:
            NX_ASSERT(false); //Unknown shape type
            return NxuTypeUnknown;
        }
        s->store(stream, tempString);
	}

    stream.endSection();

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeActor : NxuTypeUnknown;
};

NxuShapeDesc* readShape(NxuInputStream &stream, const char* name)
{
    NxuFormatHeader header;
    stream.readHeader(header);
    NX_ASSERT(NxuTypeShape == header.sectionType);
    NxU32 type;
    stream.readDword(type, "type");
    NxuShapeDesc* desc;
    switch (type)
    {
    case NX_SHAPE_PLANE:
        {
        desc = new NxuPlaneShapeDesc();
        }
        break;
    case NX_SHAPE_SPHERE:
        {
        desc = new NxuSphereShapeDesc();
        }
        break;
    case NX_SHAPE_BOX:
        {
        desc = new NxuBoxShapeDesc();
        }
        break;
    case NX_SHAPE_CAPSULE:
        {
        desc = new NxuCapsuleShapeDesc();
        }
        break;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_WHEEL
    case NX_SHAPE_WHEEL:
        {
        desc = new NxuWheelShapeDesc();
        }
        break;
#endif
    case NX_SHAPE_CONVEX:
        {
        desc = new NxuConvexShapeDesc();
        }
        break;
    case NX_SHAPE_MESH:
        {
        desc = new NxuTriangleMeshShapeDesc();
        }
        break;
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
    case NX_SHAPE_HEIGHTFIELD:
        {
        desc = new NxuHeightFieldShapeDesc();
        }
        break;
#endif
    default:
        stream.skip();
        return NULL;
    }
    desc->read(stream, name);
    return desc;
}

#include <windows.h>

NxuSectionTypes NxuActorDesc::read(NxuInputStream &stream, const char* name)
{
    stream.readMatrix(globalPose, "globalPose");
    NX_BOOL hasBody;
    stream.readBool(hasBody, "hasBody");
    if (hasBody) 
    {
        stream.readMatrix(mBody.massLocalPose, "mBody.massLocalPose");
        stream.readVector(mBody.massSpaceInertia, "mBody.massSpaceInertia");
        stream.readFloat(mBody.mass, "mBody.mass");
        stream.readVector(mBody.linearVelocity, "mBody.linearVelocity");
        stream.readVector(mBody.angularVelocity, "mBody.angularVelocity");
        stream.readFloat(mBody.wakeUpCounter, "mBody.wakeUpCounter");
        stream.readFloat(mBody.linearDamping, "mBody.linearDamping");
        stream.readFloat(mBody.angularDamping, "mBody.angularDamping");
        stream.readFloat(mBody.maxAngularVelocity, "mBody.maxAngularVelocity");
    #if NXU_SDK_FEATURES & NXU_SDK_HAS_CCD_THRESHOLD
        if (stream.getSDKVersion() & NXU_SDK_HAS_CCD_THRESHOLD)
        {
            stream.readFloat(mBody.CCDMotionThreshold, "mBody.CCDMotionThreshold");
        }
        else
        {
            mBody.CCDMotionThreshold = 0;
        }
    #else
        if (stream.getSDKVersion() & NXU_SDK_HAS_CCD_THRESHOLD)
        {
            NxF32 dummy;
            stream.readFloat(dummy, "mBody.CCDMotionThreshold");
        }
    #endif
        stream.readDword(mBody.flags, "mBody.flags");
        stream.readFloat(mBody.sleepLinearVelocity, "mBody.sleepLinearVelocity");
        stream.readFloat(mBody.sleepAngularVelocity, "mBody.sleepAngularVelocity");
        stream.readDword(mBody.solverIterationCount, "mBody.solverIterationCount");
        body = &mBody;
    }
    else
    {
        mBody.mass = -1;
        body = 0;
    }

    stream.readFloat(density, "density");

	// this is a HACK - if you take a look in NxBodyDesc::isValidInternal(),	there is a check to	make sure
	// that	a body description does	NOT	have both a	mass and density (actually is a	little more	involved),
	// so we HACK it here to zero out the density if we	have a body	and	it has a mass
	if ( hasBody && ( mBody.mass >	0 )	)
	{
		// gotta make sure the density is 0
		density	= 0;
	}

    stream.readDword(*((NxU32*)&group), "group");
    stream.readDword(flags, "flags");
    stream.readDword(*((NxU32*)&userData), "userData");

	char* buff = new char[NXU_STREAM_MAX_STRING_LENGTH];
    memset(buff, 0, NXU_STREAM_MAX_STRING_LENGTH);
    name = buff;
    stream.readCharBuffer(buff, NXU_STREAM_MAX_STRING_LENGTH, "name");

    NxU32 numShapes = 0;
    stream.readDword(numShapes, "numShapes");

	char tmp[100];
	sprintf(tmp, "shapes count: %d", numShapes);
//	MessageBox(0, tmp, 0, 0);


	// read each shape
    char tempString[NXU_STREAM_MAX_STRING_LENGTH];
    memset(tempString, 0, NXU_STREAM_MAX_STRING_LENGTH);
	for	( NxU32	i=0; i<numShapes; ++i )
	{
        sprintf(tempString, "Shape_%d",i);
        NxuShapeDesc* desc = readShape(stream, tempString);
        if (desc) shapes.pushBack(desc->getShapeDesc());
	}

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeActor : NxuTypeUnknown;
};

NxuJointDesc::~NxuJointDesc()
{
    NxuPlaneInfo* pi;
    while (mNxuPlaneInfo.size() > 0)
    {
        pi = mNxuPlaneInfo.back();
        mNxuPlaneInfo.popBack();
        if (pi) delete pi;
    }
}

NxuJointDesc* readJoint(NxuInputStream &stream, const char* name)
{
    NxuFormatHeader header;
    stream.readHeader(header);
    NX_ASSERT(NxuTypeJoint == header.sectionType);
    NxU32 type = 0;
    stream.readDword(type, "type");
    NxuJointDesc* desc;
    switch(type)
    {
	case NX_JOINT_PRISMATIC:
	{
		desc = new NxuPrismaticJointDesc();
	}
	break;

	case NX_JOINT_REVOLUTE:
	{
		desc =  new NxuRevoluteJointDesc();
	}
	break;

	case NX_JOINT_CYLINDRICAL:
	{
		desc = new NxuCylindricalJointDesc();
	}
	break;

	case NX_JOINT_SPHERICAL:
	{
		desc = new NxuSphericalJointDesc();
	}
	break;

	case NX_JOINT_POINT_ON_LINE:
	{
		desc = new NxuPointOnLineJointDesc();
	}
	break;

	case NX_JOINT_POINT_IN_PLANE:
	{
		desc = new NxuPointInPlaneJointDesc();
	}
	break;

	case NX_JOINT_DISTANCE:
	{
		desc = new NxuDistanceJointDesc();
	}
	break;

	case NX_JOINT_PULLEY:
	{
		desc = new NxuPulleyJointDesc();
	}
	break;

	case NX_JOINT_FIXED:
	{
		desc = new NxuFixedJointDesc();
	}
	break;

	case NX_JOINT_D6:
	{
		desc = new NxuD6JointDesc();
	}
	break;

	default:
		assert(	false );
        return false;
    }
    desc->read(stream, name);

    return desc;
}

NxuSectionTypes NxuJointDesc::store(NxuOutputStream &stream, const char* name)
{

    NxuFormatHeader header;
    header.sectionType = NxuTypeJoint;
    header.size =  sizeof(NxU32)*7 + sizeof(NxVec3)*7 + sizeof(NxU8)*1 + 
                        NXU_STREAM_MAX_STRING_LENGTH + 
                        mNxuPlaneInfo.size()*(sizeof(NxVec3)*2 + sizeof(NxF32));
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

	// write joint type
    NxJointDesc* desc = getDesc();
    stream.storeDword(desc->getType(), "type");

    stream.storeDword(stream.indexLookup.pointerToIndex(mRefAttachActorDesc), "mRefAttachActorDesc");
    stream.storeDword(stream.indexLookup.pointerToIndex(mAttachActorDesc), "mAttachActorDesc");

    stream.storeVector(desc->localNormal[0], "localNormal0");
    stream.storeVector(desc->localAxis[0], "localAxis0");
    stream.storeVector(desc->localAnchor[0], "localAnchor0");

    stream.storeVector(desc->localNormal[1], "localNormal1");
    stream.storeVector(desc->localAxis[1], "localAxis1");
    stream.storeVector(desc->localAnchor[1], "localAnchor1");

    stream.storeFloat(desc->maxForce, "maxForce");
    stream.storeFloat(desc->maxTorque, "maxTorque");

	char buff[NXU_STREAM_MAX_STRING_LENGTH];
    memset(buff, 0, NXU_STREAM_MAX_STRING_LENGTH);
	sprintf( buff, desc->name	? desc->name : "null"	);
    stream.storeCharBuffer((void*)buff, NXU_STREAM_MAX_STRING_LENGTH, "name");

    stream.storeDword(desc->jointFlags, "jointFlags");

    int numLimitPlanes = mNxuPlaneInfo.size();

    stream.storeDword(numLimitPlanes, "numLimitPlanes");
    stream.storeVector(mPlaneLimitPoint, "mPlaneLimitPoint");
    stream.storeBool(mOnActor2, "mOnActor2");

	for ( int i = 0; i < numLimitPlanes; ++i )
	{
	    sprintf( buff, "planeNormal%d", i+1 );
        stream.storeVector(mNxuPlaneInfo[i]->mPlaneNormal, buff);
		sprintf( buff, "planeD%d", i+1 );
        stream.storeFloat(mNxuPlaneInfo[i]->mPlaneD, buff);
        sprintf( buff, "worldLimitPt%d", i+1 );
        stream.storeVector(mNxuPlaneInfo[i]->mWorldLimitPt, buff);
    }

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
};

NxuSectionTypes NxuJointDesc::read(NxuInputStream &stream, const char* name)
{
    NxJointDesc* desc = getDesc();

    NxU32 index;
    stream.readDword(index, "mRefAttachActorDesc");
    stream.pointerLookup.indexToPointer(index, &mRefAttachActorDesc);
    stream.readDword(index, "mAttachActorDesc");
    stream.pointerLookup.indexToPointer(index, &mAttachActorDesc);
    NX_ASSERT(mRefAttachActorDesc != 0 || mAttachActorDesc != 0); //Must have one dynamic object in joint definition

    stream.readVector(desc->localNormal[0], "localNormal0");
    stream.readVector(desc->localAxis[0], "localAxis0");
    stream.readVector(desc->localAnchor[0], "localAnchor0");

    stream.readVector(desc->localNormal[1], "localNormal1");
    stream.readVector(desc->localAxis[1], "localAxis1");
    stream.readVector(desc->localAnchor[1], "localAnchor1");

    stream.readFloat(desc->maxForce, "maxForce");
    stream.readFloat(desc->maxTorque, "maxTorque");

	char* buff = new char[NXU_STREAM_MAX_STRING_LENGTH];
    memset(buff, 0, NXU_STREAM_MAX_STRING_LENGTH);
    this->getDesc()->name = buff;
    stream.readCharBuffer(buff, NXU_STREAM_MAX_STRING_LENGTH, "name");

    stream.readDword(desc->jointFlags, "jointFlags");

    NxU32 numLimitPlanes = 0;
    stream.readDword(numLimitPlanes, "numLimitPlanes");
    stream.readVector(mPlaneLimitPoint, "mPlaneLimitPoint");
    stream.readBool(mOnActor2, "mOnActor2");

	for ( NxU32 i = 0; i < numLimitPlanes; ++i )
	{
        NxuPlaneInfo* pi = new NxuPlaneInfo();
	    sprintf( buff, "planeNormal%d", i+1 );
        stream.readVector(pi->mPlaneNormal, buff);
		sprintf( buff, "planeD%d", i+1 );
        stream.readFloat(pi->mPlaneD, buff);
        sprintf( buff, "worldLimitPt%d", i+1 );
        stream.readVector(pi->mWorldLimitPt, buff);
        mNxuPlaneInfo.pushBack(pi);
    }

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
};

NX_BOOL storeJointLimitSoftDesc(NxuOutputStream &stream, NxJointLimitSoftDesc & desc, const char* name)
{
    stream.storeFloat(desc.value, "value");
    stream.storeFloat(desc.restitution, "restitution");
    stream.storeFloat(desc.spring, "spring");
    stream.storeFloat(desc.damping, "damping");

    return (stream.errorCode == NXU_NO_ERROR);
}

NX_BOOL readJointLimitSoftDesc(NxuInputStream &stream, NxJointLimitSoftDesc & desc, const char* name)
{
    stream.readFloat(desc.value, "value");
    stream.readFloat(desc.restitution, "restitution");
    stream.readFloat(desc.spring, "spring");
    stream.readFloat(desc.damping, "damping");

    return (stream.errorCode == NXU_NO_ERROR);
}

NX_BOOL storeJointLimitSoftPairDesc(NxuOutputStream &stream, NxJointLimitSoftPairDesc & desc, const char* name)
{
    storeJointLimitSoftDesc(stream, desc.low, "low");
    storeJointLimitSoftDesc(stream, desc.high, "high");

    return (stream.errorCode == NXU_NO_ERROR);
}

NX_BOOL readJointLimitSoftPairDesc(NxuInputStream &stream, NxJointLimitSoftPairDesc & desc, const char* name)
{
    readJointLimitSoftDesc(stream, desc.low, "low");
    readJointLimitSoftDesc(stream, desc.high, "high");

    return (stream.errorCode == NXU_NO_ERROR);
}

NX_BOOL storeJointDriveDesc(NxuOutputStream &stream, NxJointDriveDesc & desc, const char* name)
{
    stream.storeDword(desc.driveType, "driveType");
    stream.storeFloat(desc.spring, "spring");
    stream.storeFloat(desc.damping, "damping");
    stream.storeFloat(desc.forceLimit, "forceLimit");

    return (stream.errorCode == NXU_NO_ERROR);
}

NX_BOOL readJointDriveDesc(NxuInputStream &stream, NxJointDriveDesc & desc, const char* name)
{
    NxU32 dummy;
    stream.readDword(dummy, "driveType");
    desc.driveType = (NxBitField32)dummy;
    stream.readFloat(desc.spring, "spring");
    stream.readFloat(desc.damping, "damping");
    stream.readFloat(desc.forceLimit, "forceLimit");

    return (stream.errorCode == NXU_NO_ERROR);
}

NxuSectionTypes NxuD6JointDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuJointDesc::store(stream, name);
    stream.storeDword(xMotion, "xMotion");
    stream.storeDword(yMotion, "yMotion");
    stream.storeDword(zMotion, "zMotion");
    stream.storeDword(swing1Motion, "swing1Motion");
    stream.storeDword(swing2Motion, "swing2Motion");
    stream.storeDword(twistMotion, "twistMotion");

    storeJointLimitSoftDesc(stream, linearLimit, "linearLimit");
    storeJointLimitSoftDesc(stream, swing1Limit, "swing1Limit");
    storeJointLimitSoftDesc(stream, swing2Limit, "swing2Limit");
    storeJointLimitSoftPairDesc(stream, twistLimit, "twistLimit");

    storeJointDriveDesc(stream, xDrive, "xDrive");
    storeJointDriveDesc(stream, yDrive, "yDrive");
    storeJointDriveDesc(stream, zDrive, "zDrive");
    storeJointDriveDesc(stream, swingDrive, "swingDrive");
    storeJointDriveDesc(stream, twistDrive, "twistDrive");
    storeJointDriveDesc(stream, slerpDrive, "slerpDrive");

    stream.storeVector(drivePosition, "drivePosition");
    stream.storeQuat(driveOrientation, "driveOrientation");
    stream.storeVector(driveLinearVelocity, "driveLinearVelocity");
    stream.storeVector(driveAngularVelocity, "driveAngularVelocity");

    stream.storeDword(projectionMode, "projectionMode");
    stream.storeFloat(projectionDistance, "projectionDistance");
    stream.storeFloat(projectionAngle, "projectionAngle");
    stream.storeFloat(gearRatio, "gearRatio");
    stream.storeDword(flags, "flags");

    stream.endSection();

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuD6JointDesc::read(NxuInputStream &stream, const char* name)
{
    NxuJointDesc::read(stream);

    NxU32 dummy;
    stream.readDword(dummy, "xMotion");
    xMotion = (NxD6JointMotion)dummy;
    stream.readDword(dummy, "yMotion");
    yMotion = (NxD6JointMotion)dummy;
    stream.readDword(dummy, "zMotion");
    zMotion = (NxD6JointMotion)dummy;
    stream.readDword(dummy, "swing1Motion");
    swing1Motion = (NxD6JointMotion)dummy;
    stream.readDword(dummy, "swing2Motion");
    swing2Motion = (NxD6JointMotion)dummy;
    stream.readDword(dummy, "twistMotion");
    twistMotion = (NxD6JointMotion)dummy;

    readJointLimitSoftDesc(stream, linearLimit, "linearLimit");
    readJointLimitSoftDesc(stream, swing1Limit, "swing1Limit");
    readJointLimitSoftDesc(stream, swing2Limit, "swing2Limit");
    readJointLimitSoftPairDesc(stream, twistLimit, "twistLimit");

    readJointDriveDesc(stream, xDrive, "xDrive");
    readJointDriveDesc(stream, yDrive, "yDrive");
    readJointDriveDesc(stream, zDrive, "zDrive");
    readJointDriveDesc(stream, swingDrive, "swingDrive");
    readJointDriveDesc(stream, twistDrive, "twistDrive");
    readJointDriveDesc(stream, slerpDrive, "slerpDrive");

    stream.readVector(drivePosition, "drivePosition");
    stream.readQuat(driveOrientation, "driveOrientation");
    stream.readVector(driveLinearVelocity, "driveLinearVelocity");
    stream.readVector(driveAngularVelocity, "driveAngularVelocity");

    stream.readDword(dummy, "projectionMode");
    projectionMode = (NxJointProjectionMode)dummy;
    stream.readFloat(projectionDistance, "projectionDistance");
    stream.readFloat(projectionAngle, "projectionAngle");
    stream.readFloat(gearRatio, "gearRatio");
    stream.readDword(flags, "flags");

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuPrismaticJointDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuJointDesc::store(stream);
    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuPrismaticJointDesc::read(NxuInputStream &stream, const char* name)
{
    NxuJointDesc::read(stream);
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuRevoluteJointDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuJointDesc::store(stream);
    stream.storeFloat(limit.low.hardness, "limit.low.hardness");
    stream.storeFloat(limit.low.restitution, "limit.low.restitution");
    stream.storeFloat(limit.low.value, "limit.low.value");
    stream.storeFloat(limit.high.hardness, "limit.high.hardness");
    stream.storeFloat(limit.high.restitution, "limit.high.restitution");
    stream.storeFloat(limit.high.value, "limit.high.value");
    stream.storeBool(motor.freeSpin, "motor.freeSpin");
    stream.storeFloat(motor.maxForce , "motor.maxForce");
    stream.storeFloat(motor.velTarget, "motor.velTarget");

    storeSpring(stream, spring, "spring");

    stream.storeFloat(projectionDistance , "projectionDistance");
    stream.storeFloat(projectionAngle, "projectionAngle");
    stream.storeDword(flags, "flags");
    stream.storeDword(projectionMode, "projectionMode");

    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuRevoluteJointDesc::read(NxuInputStream &stream, const char* name)
{
    NxuJointDesc::read(stream);

    stream.readFloat(limit.low.hardness, "limit.low.hardness");
    stream.readFloat(limit.low.restitution, "limit.low.restitution");
    stream.readFloat(limit.low.value, "limit.low.value");
    stream.readFloat(limit.high.hardness, "limit.high.hardness");
    stream.readFloat(limit.high.restitution, "limit.high.restitution");
    stream.readFloat(limit.high.value, "limit.high.value");
    stream.readBool(motor.freeSpin, "motor.freeSpin");
    stream.readFloat(motor.maxForce, "motor.maxForce");
    stream.readFloat(motor.velTarget, "motor.velTarget");

    readSpring(stream, spring, "spring");

    stream.readFloat(projectionDistance, "projectionDistance");
    stream.readFloat(projectionAngle, "projectionAngle");
    stream.readDword(flags, "flags");
    stream.readDword(*((NxU32*)&projectionMode), "projectionMode");

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuCylindricalJointDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuJointDesc::store(stream);
    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuCylindricalJointDesc::read(NxuInputStream &stream, const char* name)
{
    NxuJointDesc::read(stream);
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuSphericalJointDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuJointDesc::store(stream);
    stream.storeFloat(twistSpring.spring, "twistSpring.spring");
    stream.storeFloat(twistSpring.damper, "twistSpring.damper");
    stream.storeFloat(twistSpring.targetValue, "twistSpring.targetValue");

    stream.storeFloat(swingSpring.spring, "swingSpring.spring");
    stream.storeFloat(swingSpring.damper, "swingSpring.damper");
    stream.storeFloat(swingSpring.targetValue, "swingSpring.targetValue");

    stream.storeFloat(jointSpring.spring, "jointSpring.spring");
    stream.storeFloat(jointSpring.damper, "jointSpring.damper");
    stream.storeFloat(jointSpring.targetValue, "jointSpring.targetValue");

    stream.storeFloat(projectionDistance, "projectionDistance");

    stream.storeFloat(twistLimit.low.hardness, "twistLimit.low.hardness");
    stream.storeFloat(twistLimit.low.restitution, "twistLimit.low.restitution");
    stream.storeFloat(twistLimit.low.value, "twistLimit.low.value");

    stream.storeFloat(twistLimit.high.hardness, "twistLimit.high.hardness");
    stream.storeFloat(twistLimit.high.restitution, "twistLimit.high.restitution");
    stream.storeFloat(twistLimit.high.value, "twistLimit.high.value");

    stream.storeFloat(swingLimit.hardness, "swingLimit.hardness");
    stream.storeFloat(swingLimit.restitution, "swingLimit.restitution");
    stream.storeFloat(swingLimit.value, "swingLimit.value");

    stream.storeDword(flags, "flags");

    stream.storeFloat(swingAxis.x, "swingAxis.x");
    stream.storeFloat(swingAxis.y, "swingAxis.y");
    stream.storeFloat(swingAxis.z, "swingAxis.z");

    stream.storeDword(projectionMode, "projectionMode");

    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuSphericalJointDesc::read(NxuInputStream &stream, const char* name)
{
    NxuJointDesc::read(stream);

    stream.readFloat(twistSpring.spring, "twistSpring.spring");
    stream.readFloat(twistSpring.damper, "twistSpring.damper");
    stream.readFloat(twistSpring.targetValue, "twistSpring.targetValue");

    stream.readFloat(swingSpring.spring, "swingSpring.spring");
    stream.readFloat(swingSpring.damper, "swingSpring.damper");
    stream.readFloat(swingSpring.targetValue, "swingSpring.targetValue");

    stream.readFloat(jointSpring.spring, "jointSpring.spring");
    stream.readFloat(jointSpring.damper, "jointSpring.damper");
    stream.readFloat(jointSpring.targetValue, "jointSpring.targetValue");

    stream.readFloat(projectionDistance, "projectionDistance");

    stream.readFloat(twistLimit.low.hardness, "twistLimit.low.hardness");
    stream.readFloat(twistLimit.low.restitution, "twistLimit.low.restitution");
    stream.readFloat(twistLimit.low.value, "twistLimit.low.value");

    stream.readFloat(twistLimit.high.hardness, "twistLimit.high.hardness");
    stream.readFloat(twistLimit.high.restitution, "twistLimit.high.restitution");
    stream.readFloat(twistLimit.high.value, "twistLimit.high.value");

    stream.readFloat(swingLimit.hardness, "swingLimit.hardness");
    stream.readFloat(swingLimit.restitution, "swingLimit.restitution");
    stream.readFloat(swingLimit.value, "swingLimit.value");

    stream.readDword(flags, "flags");

    stream.readFloat(swingAxis.x, "swingAxis.x");
    stream.readFloat(swingAxis.y, "swingAxis.y");
    stream.readFloat(swingAxis.z, "swingAxis.z");

    stream.readDword(*((NxU32*)&projectionMode), "projectionMode");

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuPointOnLineJointDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuJointDesc::store(stream);
    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuPointOnLineJointDesc::read(NxuInputStream &stream, const char* name)
{
    NxuJointDesc::read(stream);
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuPointInPlaneJointDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuJointDesc::store(stream);
    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuPointInPlaneJointDesc::read(NxuInputStream &stream, const char* name)
{
    NxuJointDesc::read(stream);
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuDistanceJointDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuJointDesc::store(stream);

    stream.storeFloat(minDistance, "minDistance");
    stream.storeFloat(maxDistance, "maxDistance");
    stream.storeDword(flags, "flags");

    storeSpring(stream, spring, "spring");

    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuDistanceJointDesc::read(NxuInputStream &stream, const char* name)
{
    NxuJointDesc::read(stream);

    stream.readFloat(minDistance, "minDistance");
    stream.readFloat(maxDistance, "maxDistance");
    stream.readDword(flags, "flags");

    readSpring(stream, spring, "spring");

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuPulleyJointDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuJointDesc::store(stream);
    
    stream.storeFloat(distance, "distance");
    stream.storeFloat(stiffness, "stiffness");
    stream.storeFloat(ratio, "ratio");
    stream.storeDword(flags, "flags");

    stream.storeBool(motor.freeSpin, "motor.freeSpin");
    stream.storeFloat(motor.maxForce , "motor.maxForce");
    stream.storeFloat(motor.velTarget, "motor.velTarget");

    stream.storeVector(pulley[0], "pulley[0]");
    stream.storeVector(pulley[1], "pulley[1]");

    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuPulleyJointDesc::read(NxuInputStream &stream, const char* name)
{
    NxuJointDesc::read(stream);

    stream.readFloat(distance, "distance");
    stream.readFloat(stiffness, "stiffness");
    stream.readFloat(ratio, "ratio");
    stream.readDword(flags, "flags");

    stream.readBool(motor.freeSpin, "motor.freeSpin");
    stream.readFloat(motor.maxForce, "motor.maxForce");
    stream.readFloat(motor.velTarget, "motor.velTarget");

    stream.readVector(pulley[0], "pulley[0]");
    stream.readVector(pulley[1], "pulley[1]");

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuFixedJointDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuJointDesc::store(stream);
    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuFixedJointDesc::read(NxuInputStream &stream, const char* name)
{
    NxuJointDesc::read(stream);
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeJoint : NxuTypeUnknown;
}

NxuSectionTypes NxuSpringAndDamperEffectorDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuFormatHeader header;
    header.sectionType = NxuTypeEffector;
    header.size =  sizeof(NxU32)*2 + sizeof(NxMat34)*2 + sizeof(NxF32)*9;
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

    stream.storeDword(stream.indexLookup.pointerToIndex(mRefAttachActorDesc), "mRefAttachActorDesc");
    stream.storeDword(stream.indexLookup.pointerToIndex(mAttachActorDesc), "mAttachActorDesc");

	// we currently cannot write who these things are connected
	// to, but we will write out their world matrices
    stream.storeVector(pos1, "pos1");
    stream.storeVector(pos2, "pos2");
	
    // now we write information about the linear spring
    stream.storeFloat(springDistCompressSaturate, "springDistCompressSaturate"); 
    stream.storeFloat(springDistRelaxed, "springDistRelaxed"); 
    stream.storeFloat(springDistStretchSaturate, "springDistStretchSaturate"); 
    stream.storeFloat(springMaxCompressForce, "springMaxCompressForce"); 
    stream.storeFloat(springMaxStretchForce, "springMaxStretchForce"); 

	// now we write information about the linear damper
    stream.storeFloat(damperVelCompressSaturate, "damperVelCompressSaturate"); 
    stream.storeFloat(damperVelStretchSaturate, "damperVelStretchSaturate"); 
    stream.storeFloat(damperMaxCompressForce, "damperMaxCompressForce"); 
    stream.storeFloat(damperMaxStretchForce, "damperMaxStretchForce"); 

    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeEffector : NxuTypeUnknown;
}

NxuSectionTypes NxuSpringAndDamperEffectorDesc::read(NxuInputStream &stream, const char* name)
{
    NxU32 index;
    stream.readDword(index, "mRefAttachActorDesc");
    stream.pointerLookup.indexToPointer(index, &mRefAttachActorDesc);
    stream.readDword(index, "mAttachActorDesc");
    stream.pointerLookup.indexToPointer(index, &mAttachActorDesc);

	// we currently cannot write who these things are connected
	// to, but we will write out their world matrices
    stream.readVector(pos1, "pos1");
    stream.readVector(pos2, "pos2");
	
    // now we write information about the linear spring
    stream.readFloat(springDistCompressSaturate, "springDistCompressSaturate"); 
    stream.readFloat(springDistRelaxed, "springDistRelaxed"); 
    stream.readFloat(springDistStretchSaturate, "springDistStretchSaturate"); 
    stream.readFloat(springMaxCompressForce, "springMaxCompressForce"); 
    stream.readFloat(springMaxStretchForce, "springMaxStretchForce"); 

	// now we write information about the linear damper
    stream.readFloat(damperVelCompressSaturate, "damperVelCompressSaturate"); 
    stream.readFloat(damperVelStretchSaturate, "damperVelStretchSaturate"); 
    stream.readFloat(damperMaxCompressForce, "damperMaxCompressForce"); 
    stream.readFloat(damperMaxStretchForce, "damperMaxStretchForce"); 

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeEffector : NxuTypeUnknown;
}

NxuSectionTypes NxuPairFlagDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuFormatHeader header;
    header.sectionType = NxuTypePairFlag;
    header.size =  sizeof(NxU32)*5;
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

    stream.storeDword(mFlag, "mFlag");
    stream.storeDword(stream.indexLookup.pointerToIndex(mActor0), "mActor0");
    stream.storeDword(mShapeIndex0, "mShapeIndex0");
    stream.storeDword(stream.indexLookup.pointerToIndex(mActor1), "mActor1");
    stream.storeDword(mShapeIndex1, "mShapeIndex1");

    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypePairFlag : NxuTypeUnknown;
}

NxuSectionTypes NxuPairFlagDesc::read(NxuInputStream &stream, const char* name)
{
    stream.readDword(mFlag, "mFlag");
    NxU32 index = (NxU32)-1;
    stream.readDword(index, "mActor0");
    stream.pointerLookup.indexToPointer(index, &mActor0);
    index = (NxU32)-1;
    stream.readDword(mShapeIndex0, "mShapeIndex0");
    stream.readDword(index, "mActor1");
    stream.pointerLookup.indexToPointer(index, &mActor1);
    stream.readDword(mShapeIndex1, "mShapeIndex1");

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypePairFlag : NxuTypeUnknown;
}

#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
NxuSectionTypes NxuFluidEmitterDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuFormatHeader header;
    header.sectionType = NxuTypeFluidEmitter;
    header.size =  0;
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

    stream.storeMatrix(relPose, "relPose");
    stream.storeDword(stream.indexLookup.pointerToIndex(mFrameActor), "mFrameActor");

    stream.storeDword(type, "type");
    stream.storeDword(maxParticles, "maxParticles");
    stream.storeDword(shape, "shape");
    stream.storeFloat(dimensionX, "dimensionX");
    stream.storeFloat(dimensionY, "dimensionY");
    stream.storeVector(randomPos, "randomPos");
    stream.storeFloat(randomAngle, "randomAngle");
    stream.storeFloat(fluidVelocityMagnitude, "fluidVelocityMagnitude");
    stream.storeFloat(rate, "rate");
    stream.storeFloat(particleLifetime, "particleLifetime");
    stream.storeDword(flags, "flags");
    stream.storeDword(*((NxU32*)&userData), "userData");

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeFluidEmitter : NxuTypeUnknown;
}

NxuSectionTypes NxuFluidEmitterDesc::read(NxuInputStream &stream, const char* name)
{
    stream.readMatrix(relPose, "relPose");
    NxU32 index;
    stream.readDword(index, "mFrameActor");
    stream.pointerLookup.indexToPointer(index, &mFrameActor);
    stream.readDword(type, "type");
    stream.readDword(maxParticles, "maxParticles");
    stream.readDword(shape, "shape");
    stream.readFloat(dimensionX, "dimensionX");
    stream.readFloat(dimensionY, "dimensionY");
    stream.readVector(randomPos, "randomPos");
    stream.readFloat(randomAngle, "randomAngle");
    stream.readFloat(fluidVelocityMagnitude, "fluidVelocityMagnitude");
    stream.readFloat(rate, "rate");
    stream.readFloat(particleLifetime, "particleLifetime");
    stream.readDword(flags, "flags");
    stream.readDword(*((NxU32*)&userData), "userData");

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeFluidEmitter : NxuTypeUnknown;
}

void storeParticleData(NxParticleData & data, NxuOutputStream &stream)
{
    stream.storeDword(data.maxParticles, "data.maxParticles");
    NxU32 count = *(data.numParticlesPtr);
    stream.storeDword(count,"data.numParticles");


    stream.storeDword(data.bufferPosByteStride, "data.bufferPosByteStride");
    stream.storeDword(data.bufferVelByteStride, "data.bufferVelByteStride");
    stream.storeDword(data.bufferLifeByteStride, "data.bufferLifeByteStride");
    stream.storeDword(data.bufferDensityByteStride, "data.bufferDensityByteStride");
    stream.storeDword(data.bufferFlagsByteStride, "data.bufferFlagsByteStride");

    if (data.bufferPosByteStride)
    {
        stream.storeFloatBuffer(data.bufferPos, count*data.bufferPosByteStride, "data.bufferPos");
    }
    if (data.bufferVelByteStride)
    {
        stream.storeFloatBuffer(data.bufferVel, count*data.bufferVelByteStride, "data.bufferVel");
    }
    if (data.bufferLifeByteStride)
    {
        stream.storeFloatBuffer(data.bufferLife, count*data.bufferLifeByteStride, "data.bufferLife");
    }
    if (data.bufferDensityByteStride)
    {
        stream.storeFloatBuffer(data.bufferDensity, count*data.bufferDensityByteStride, "data.bufferDensity");
    }
    if (data.bufferFlagsByteStride)
    {
        stream.storeDwordBuffer(data.bufferFlags, count*data.bufferFlagsByteStride, "data.bufferDensity");
    }

}


//Deprecated
//void storeImplicitMesh(NxImplicitMeshDesc & surfaceMeshDesc, NxuOutputStream &stream)
//{
//    stream.storeFloat(surfaceMeshDesc.surfaceRadius, "surfaceMeshDesc.surfaceRadius");
//    stream.storeFloat(surfaceMeshDesc.singleRadius, "surfaceMeshDesc.singleRadius");
//    stream.storeFloat(surfaceMeshDesc.blend, "surfaceMeshDesc.blend");
//    stream.storeFloat(surfaceMeshDesc.triangleSize, "surfaceMeshDesc.triangleSize");
//    stream.storeDword(surfaceMeshDesc.smoothingIterations, "surfaceMeshDesc.smoothingIterations");
//
//    //NxMeshData
//    stream.storeDword(surfaceMeshDesc.meshData.maxVertices, "surfaceMeshDesc.meshData.maxVertices");
//    stream.storeDword(surfaceMeshDesc.meshData.verticesPosByteStride, "surfaceMeshDesc.meshData.verticesPosByteStride");
//    stream.storeDword(surfaceMeshDesc.meshData.verticesNormalByteStride, "surfaceMeshDesc.meshData.verticesNormalByteStride");
//    NxU32 count = *(surfaceMeshDesc.meshData.numVerticesPtr);
//    stream.storeDword(count, "surfaceMeshDesc.meshData.numVerticesPtr");
//    stream.storeDword(surfaceMeshDesc.meshData.flags, "surfaceMeshDesc.meshData.flags");
//    if (surfaceMeshDesc.meshData.flags & NX_MDF_16_BIT_COMPRESSED_FLOATS)
//    {
//        stream.storeWordBuffer(surfaceMeshDesc.meshData.verticesPosBegin, count*surfaceMeshDesc.meshData.verticesPosByteStride, "surfaceMeshDesc.meshData.verticesPosBegin");
//    }
//    else
//    {
//        stream.storeFloatBuffer(surfaceMeshDesc.meshData.verticesPosBegin, count*surfaceMeshDesc.meshData.verticesPosByteStride, "surfaceMeshDesc.meshData.verticesPosBegin");
//    }
//
//    if (surfaceMeshDesc.meshData.flags & NX_MDF_16_BIT_COMPRESSED_FLOATS)
//    {
//        stream.storeWordBuffer(surfaceMeshDesc.meshData.verticesNormalBegin, count*surfaceMeshDesc.meshData.verticesNormalByteStride, "surfaceMeshDesc.meshData.verticesNormalBegin");
//    }
//    else
//    {
//        stream.storeFloatBuffer(surfaceMeshDesc.meshData.verticesNormalBegin, count*surfaceMeshDesc.meshData.verticesNormalByteStride, "surfaceMeshDesc.meshData.verticesNormalBegin");
//    }
//
//    stream.storeDword(surfaceMeshDesc.meshData.maxIndices, "surfaceMeshDesc.meshData.maxIndices");
//    stream.storeDword(surfaceMeshDesc.meshData.indicesByteStride, "surfaceMeshDesc.meshData.indicesByteStride");
//    NxU32 tricount = *(surfaceMeshDesc.meshData.numTrianglesPtr);
//    stream.storeDword(tricount, "surfaceMeshDesc.meshData.numTrianglesPtr");
//    if (surfaceMeshDesc.meshData.flags & NX_MDF_INDEXED_MESH)
//    {
//        if (surfaceMeshDesc.meshData.flags & NX_MDF_16_BIT_INDICES)
//        {
//            stream.storeWordBuffer(surfaceMeshDesc.meshData.indicesBegin, tricount*surfaceMeshDesc.meshData.indicesByteStride, "surfaceMeshDesc.meshData.indicesBegin");
//        }
//        else
//        {
//            stream.storeDwordBuffer(surfaceMeshDesc.meshData.indicesBegin, tricount*surfaceMeshDesc.meshData.indicesByteStride, "surfaceMeshDesc.meshData.indicesBegin");
//        }    
//    }
//
//    stream.storeDword(surfaceMeshDesc.meshData.maxParentIndices, "surfaceMeshDesc.meshData.maxParentIndices");
//    stream.storeDword(surfaceMeshDesc.meshData.parentIndicesByteStride, "surfaceMeshDesc.meshData.parentIndicesByteStride");
//    NxU32 parentCount = *(surfaceMeshDesc.meshData.numParentIndicesPtr);
//    stream.storeDword(parentCount, "surfaceMeshDesc.meshData.numParentIndicesPtr");
//    stream.storeDwordBuffer(surfaceMeshDesc.meshData.parentIndicesBegin, parentCount*surfaceMeshDesc.meshData.parentIndicesByteStride, "surfaceMeshDesc.meshData.parentIndicesBegin");
//
//}

NxuSectionTypes NxuFluidDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuFormatHeader header;
    header.sectionType = NxuTypeFluid;
    header.size =  0;
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

    NxU32 numEmitters = mEmitters.size();
    stream.storeDword(numEmitters, "numEmitters");

    storeParticleData(initialParticleData, stream);
    stream.storeDword(maxParticles, "maxParticles");
    stream.storeFloat(restParticlesPerMeter, "restParticlesPerMeter");
    stream.storeFloat(restDensity, "restDensity");
    stream.storeFloat(kernelRadiusMultiplier, "kernelRadiusMultiplier");
    stream.storeFloat(motionLimitMultiplier, "motionLimitMultiplier");
    stream.storeDword(packetSizeMultiplier, "packetSizeMultiplier");
    stream.storeFloat(stiffness, "stiffness");
    stream.storeFloat(viscosity, "viscosity");
    stream.storeFloat(damping, "damping");
    stream.storeVector(externalAcceleration, "externalAcceleration");
    stream.storeFloat(staticCollisionRestitution, "staticCollisionRestitution");
    stream.storeFloat(dynamicCollisionRestitution, "dynamicCollisionRestitution");
    stream.storeFloat(dynamicCollisionAdhesion, "dynamicCollisionAdhesion");
    stream.storeDword(simulationMethod, "simulationMethod");
    stream.storeDword(collisionMethod, "collisionMethod");

    //Deprecated
    //stream.storeBool(surfaceMeshDesc ? true : false, "hasSurfaceMeshDesc");
    //if (surfaceMeshDesc)
    //{
    //    storeImplicitMesh(*surfaceMeshDesc, stream);
    //}

    //Particle write data is not saved
    
    stream.storeDword(flags, "flags");

    stream.storeDword(*((NxU32*)&userData), "userData");

    char tempString[NXU_STREAM_MAX_STRING_LENGTH];
    for (NxU32 i=0;i<numEmitters;++i)
    {
        sprintf(tempString, "Emitter_%d", i);
        mEmitters[i]->store(stream, tempString);
    }

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeFluid : NxuTypeUnknown;
}

void readParticleData(NxParticleData & data, NxU32 &numParticles, NxuInputStream &stream)
{
    stream.readDword(data.maxParticles, "data.maxParticles");
    stream.readDword(numParticles,"data.numParticles");
    data.numParticlesPtr = &numParticles;

    stream.readDword(data.bufferPosByteStride, "data.bufferPosByteStride");
    stream.readDword(data.bufferVelByteStride, "data.bufferVelByteStride");
    stream.readDword(data.bufferLifeByteStride, "data.bufferLifeByteStride");
    stream.readDword(data.bufferDensityByteStride, "data.bufferDensityByteStride");
    stream.readDword(data.bufferFlagsByteStride, "data.bufferFlagsByteStride");

    if (data.bufferPosByteStride) 
    {
        data.bufferPos = (NxF32*) new NxU8[data.maxParticles*data.bufferPosByteStride];
        stream.readFloatBuffer(data.bufferPos, numParticles*data.bufferPosByteStride, "data.bufferPos");
    }
    if (data.bufferVelByteStride) 
    {
        data.bufferVel = (NxF32*) new NxU8[data.maxParticles*data.bufferVelByteStride];
        stream.readFloatBuffer(data.bufferVel, numParticles*data.bufferVelByteStride, "data.bufferVel");
    }
    if (data.bufferLifeByteStride) 
    {
        data.bufferLife = (NxF32*) new NxU8[data.maxParticles*data.bufferLifeByteStride];
        stream.readFloatBuffer(data.bufferLife, numParticles*data.bufferLifeByteStride, "data.bufferLife");
    }
    if (data.bufferDensityByteStride) 
    {
        data.bufferDensity = (NxF32*) new NxU8[data.maxParticles*data.bufferDensityByteStride];
        stream.readFloatBuffer(data.bufferDensity, numParticles*data.bufferDensityByteStride, "data.bufferDensity");
    }
    if (data.bufferFlagsByteStride) 
    {
        data.bufferFlags = (NxU32*) new NxU8[data.maxParticles*data.bufferFlagsByteStride];
        stream.readDwordBuffer(data.bufferFlags, numParticles*data.bufferFlagsByteStride, "data.bufferDensity");
    }
}
//Deprecated
//void readImplicitMesh(NxImplicitMeshDesc & surfaceMeshDesc, NxU32 &vertCount, NxU32 & indCount, NxU32 & parentCount, NxuInputStream &stream)
//{
//    stream.readFloat(surfaceMeshDesc.surfaceRadius, "surfaceMeshDesc.surfaceRadius");
//    stream.readFloat(surfaceMeshDesc.singleRadius, "surfaceMeshDesc.singleRadius");
//    stream.readFloat(surfaceMeshDesc.blend, "surfaceMeshDesc.blend");
//    stream.readFloat(surfaceMeshDesc.triangleSize, "surfaceMeshDesc.triangleSize");
//    stream.readDword(surfaceMeshDesc.smoothingIterations, "surfaceMeshDesc.smoothingIterations");
//
//    //NxMeshData
//    stream.readDword(surfaceMeshDesc.meshData.maxVertices, "surfaceMeshDesc.meshData.maxVertices");
//    stream.readDword(*((NxU32*)&surfaceMeshDesc.meshData.verticesPosByteStride), "surfaceMeshDesc.meshData.verticesPosByteStride");
//    stream.readDword(*((NxU32*)&surfaceMeshDesc.meshData.verticesNormalByteStride), "surfaceMeshDesc.meshData.verticesNormalByteStride");
//    stream.readDword(vertCount, "surfaceMeshDesc.meshData.numVerticesPtr");
//    surfaceMeshDesc.meshData.numVerticesPtr = &vertCount;
//    stream.readDword(surfaceMeshDesc.meshData.flags, "surfaceMeshDesc.meshData.flags");
//    surfaceMeshDesc.meshData.verticesPosBegin = new NxU8[vertCount*surfaceMeshDesc.meshData.verticesPosByteStride];
//    if (surfaceMeshDesc.meshData.flags & NX_MDF_16_BIT_COMPRESSED_FLOATS)
//    {
//        stream.readWordBuffer(surfaceMeshDesc.meshData.verticesPosBegin, vertCount*surfaceMeshDesc.meshData.verticesPosByteStride, "surfaceMeshDesc.meshData.verticesPosBegin");
//    }
//    else
//    {
//        stream.readFloatBuffer(surfaceMeshDesc.meshData.verticesPosBegin, vertCount*surfaceMeshDesc.meshData.verticesPosByteStride, "surfaceMeshDesc.meshData.verticesPosBegin");
//    }
//    surfaceMeshDesc.meshData.verticesNormalBegin = new NxU8[vertCount*surfaceMeshDesc.meshData.verticesNormalByteStride];
//    if (surfaceMeshDesc.meshData.flags & NX_MDF_16_BIT_COMPRESSED_FLOATS)
//    {
//        stream.readWordBuffer(surfaceMeshDesc.meshData.verticesNormalBegin, vertCount*surfaceMeshDesc.meshData.verticesNormalByteStride, "surfaceMeshDesc.meshData.verticesNormalBegin");
//    }
//    else
//    {
//        stream.readFloatBuffer(surfaceMeshDesc.meshData.verticesNormalBegin, vertCount*surfaceMeshDesc.meshData.verticesNormalByteStride, "surfaceMeshDesc.meshData.verticesNormalBegin");
//    }
//
//    stream.readDword(surfaceMeshDesc.meshData.maxIndices, "surfaceMeshDesc.meshData.maxIndices");
//    stream.readDword(*((NxU32*)&surfaceMeshDesc.meshData.indicesByteStride), "surfaceMeshDesc.meshData.indicesByteStride");
//    stream.readDword(indCount, "surfaceMeshDesc.meshData.numTrianglesPtr");
//    surfaceMeshDesc.meshData.numTrianglesPtr = &indCount;
//    surfaceMeshDesc.meshData.indicesBegin = new NxU8[indCount*surfaceMeshDesc.meshData.indicesByteStride];
//    if (surfaceMeshDesc.meshData.flags & NX_MDF_INDEXED_MESH)
//    {
//        if (surfaceMeshDesc.meshData.flags & NX_MDF_16_BIT_INDICES)
//        {
//            stream.readWordBuffer(surfaceMeshDesc.meshData.indicesBegin, indCount*surfaceMeshDesc.meshData.indicesByteStride, "surfaceMeshDesc.meshData.indicesBegin");
//        }
//        else
//        {
//            stream.readDwordBuffer(surfaceMeshDesc.meshData.indicesBegin, indCount*surfaceMeshDesc.meshData.indicesByteStride, "surfaceMeshDesc.meshData.indicesBegin");
//        }    
//    }
//
//    stream.readDword(surfaceMeshDesc.meshData.maxParentIndices, "surfaceMeshDesc.meshData.maxParentIndices");
//    stream.readDword(*((NxU32*)&surfaceMeshDesc.meshData.parentIndicesByteStride), "surfaceMeshDesc.meshData.parentIndicesByteStride");
//    stream.readDword(parentCount, "surfaceMeshDesc.meshData.numParentIndicesPtr");
//    surfaceMeshDesc.meshData.numParentIndicesPtr = &parentCount;
//    surfaceMeshDesc.meshData.parentIndicesBegin = new NxU8[parentCount*surfaceMeshDesc.meshData.parentIndicesByteStride];
//    stream.readDwordBuffer(surfaceMeshDesc.meshData.parentIndicesBegin, parentCount*surfaceMeshDesc.meshData.parentIndicesByteStride, "surfaceMeshDesc.meshData.parentIndicesBegin");
//
//}

NxuSectionTypes NxuFluidDesc::read(NxuInputStream &stream, const char* name)
{
    NxU32 numEmitters = 0;
    stream.readDword(numEmitters, "numEmitters");

    readParticleData(initialParticleData, mNumParticles, stream);
    initialParticleData.numParticlesPtr = &mNumParticles;

    stream.readDword(maxParticles, "maxParticles");
    stream.readFloat(restParticlesPerMeter, "restParticlesPerMeter");
    stream.readFloat(restDensity, "restDensity");
    stream.readFloat(kernelRadiusMultiplier, "kernelRadiusMultiplier");
    stream.readFloat(motionLimitMultiplier, "motionLimitMultiplier");
    stream.readDword(packetSizeMultiplier, "packetSizeMultiplier");
    stream.readFloat(stiffness, "stiffness");
    stream.readFloat(viscosity, "viscosity");
    stream.readFloat(damping, "damping");
    stream.readVector(externalAcceleration, "externalAcceleration");
    stream.readFloat(staticCollisionRestitution, "staticCollisionRestitution");
    stream.readFloat(dynamicCollisionRestitution, "dynamicCollisionRestitution");
    stream.readFloat(dynamicCollisionAdhesion, "dynamicCollisionAdhesion");
    stream.readDword(simulationMethod, "simulationMethod");
    stream.readDword(collisionMethod, "collisionMethod");

    //Implicit Mesh is deprecated
    //NX_BOOL hasSurfaceMeshDesc;
    //stream.readBool(hasSurfaceMeshDesc, "hasSurfaceMeshDesc");
    //if (hasSurfaceMeshDesc)
    //{
    //    surfaceMeshDesc = new NxImplicitMeshDesc();
    //    surfaceMeshDesc->setToDefault();
    //    readImplicitMesh(*surfaceMeshDesc, mImplicitVerts, mImplicitIndexes, mImplicitParents,  stream);
    //}

    //Particle write data is not saved
    
    stream.readDword(flags, "flags");

    NxU32 dummy;
    stream.readDword(*((NxU32*)&dummy), "userData");

    char tempString[NXU_STREAM_MAX_STRING_LENGTH];
    for (NxU32 i=0;i<numEmitters;++i)
    {
        NxuFormatHeader header;
        stream.readHeader(header);
        NX_ASSERT(NxuTypeFluidEmitter == header.sectionType);
        sprintf(tempString, "Emitter_%d", i);
        NxuFluidEmitterDesc* desc = new NxuFluidEmitterDesc();
        desc->read(stream, tempString);
        mEmitters.pushBack(desc);
        emitters.pushBack(*desc); //Add it to the reference based list as well
    }


    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeFluid : NxuTypeUnknown;
}
#endif

NxuSectionTypes NxuPhysicsModelInstance::read(NxuInputStream &stream, const char* name)
{   
    stream.readMatrix(mPose, "mPose");
    NxU32 index;
    stream.readDword(index, "mModel");
    stream.pointerLookup.indexToPointer(index, &mModel);
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeModelInstance : NxuTypeUnknown;

}

NxuSectionTypes NxuPhysicsModelInstance::store(NxuOutputStream &stream, const char* name)
{
    NxuFormatHeader header;
    header.sectionType = NxuTypeModelInstance;
    header.size = sizeof(NxMat34) + sizeof(NxU32);
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);
    
    stream.storeMatrix(mPose, "mPose");
    stream.storeDword(stream.indexLookup.pointerToIndex(mModel), "mModel");

    stream.endSection(); 
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeModelInstance : NxuTypeUnknown;
}



NxuPhysicsModel::~NxuPhysicsModel()
{
   while( !mActors.isEmpty())
    {
        NxuActorDesc* d = mActors.back();
        mActors.popBack();
        delete d;
    }
    while( !mEffectors.isEmpty())
    {
        NxuSpringAndDamperEffectorDesc* d = mEffectors.back();
        mEffectors.popBack();
        delete d;
    }
    while( !mJoints.isEmpty())
    {
        NxuJointDesc* d= mJoints.back();
        mJoints.popBack(); 
        delete (d->getDesc());
    }
    while( !mMaterials.isEmpty())
    {
        NxuMaterialDesc* d = mMaterials.back();
        mMaterials.popBack();
        delete d;
    }
    while( !mModels.isEmpty())
    {
        NxuPhysicsModelInstance* d = mModels.back();
        mModels.popBack();
        delete d;
    }
    while( !mPairFlags.isEmpty())
    {
        NxuPairFlagDesc* d = mPairFlags.back();
        mPairFlags.popBack();
        delete d;
    }
}

NxuSectionTypes NxuPhysicsModel::store(NxuOutputStream &stream, const char* name)
{
    stream.indexLookup.model = this;
    NxuFormatHeader header;
    header.sectionType = NxuTypeModel;
    header.size =  0; //Can't predict the size of a model well
    memset(header.name, 0, NXU_STREAM_MAX_STRING_LENGTH);
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

    stream.storeCharBuffer(name, NXU_STREAM_MAX_STRING_LENGTH, "name");
    
    NxU32 matCount = mMaterials.size();
    stream.storeDword(matCount, "matCount");
    
    NxU32 actorCount = mActors.size();
    stream.storeDword(actorCount, "actorCount");

    NxU32 jointCount = mJoints.size();
    stream.storeDword(jointCount, "jointCount");

    NxU32 pairFlagCount = mPairFlags.size();
    stream.storeDword(pairFlagCount, "pairFlagCount");

	NxU32 collisionGroupCount = 0;
	stream.storeDword(collisionGroupCount, "collisionGroupCount");

    NxU32 effectorCount = mEffectors.size();
    stream.storeDword(effectorCount, "effectorCount");

#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
    NxU32 fluidCount = mFluids.size();
#else
	NxU32 fluidCount = 0;
#endif
    stream.storeDword(fluidCount, "fluidCount");
    
    stream.storeDword(fluidCount, "clothCount");

    NxU32 modelCount = mModels.size();
    stream.storeDword(modelCount, "modelCount");

    char tempString[NXU_STREAM_MAX_STRING_LENGTH];
    memset(tempString, 0, NXU_STREAM_MAX_STRING_LENGTH);
    for (NxU32 i=0; i<matCount; ++i)
    {
        sprintf(tempString, "Material_%d",i);
        mMaterials[i]->store(stream, tempString);
    }

    
    for (NxU32 i=0; i<actorCount; ++i)
    {
        sprintf(tempString, "Actor_%d",i);
        mActors[i]->store(stream, tempString);
    }

    
    for (NxU32 i=0; i<jointCount; ++i)
    {
        sprintf(tempString, "Joint_%d",i);
        mJoints[i]->store(stream, tempString);
    }

    
    for (NxU32 i=0; i<pairFlagCount; ++i)
    {
        sprintf(tempString, "PairFlag_%d",i);
        mPairFlags[i]->store(stream, tempString);
    }

    
    for (NxU32 i=0; i<effectorCount; ++i)
    {
        sprintf(tempString, "Effector_%d",i);
        mEffectors[i]->store(stream, tempString);
    }

#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
    for (NxU32 i=0; i<fluidCount; ++i)
    {
        sprintf(tempString, "Fluid_%d",i);
        mFluids[i]->store(stream, tempString);
    }
#endif

    for (NxU32 i=0;i<modelCount;++i)
    {
        sprintf(tempString, "Instanced Model_%d",i);
        mModels[i]->store(stream, tempString);
    }

    stream.endSection();
    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeModel : NxuTypeUnknown;
}

#include <windows.h>

NxuSectionTypes NxuPhysicsModel::read(NxuInputStream &stream, const char* name)
{
    NxuFormatHeader header;
    stream.pointerLookup.model = this;
    stream.readCharBuffer(this->name, NXU_STREAM_MAX_STRING_LENGTH, "name");
    
    NxU32 matCount = 0;
    stream.readDword(matCount, "matCount");
    
    NxU32 actorCount = 0;
    stream.readDword(actorCount, "actorCount");

    NxU32 jointCount = 0;
    stream.readDword(jointCount, "jointCount");

    NxU32 pairFlagCount = 0;
    stream.readDword(pairFlagCount, "pairFlagCount");

	NxU32 collisionGroupCount = 0;
	stream.readDword(collisionGroupCount, "collisionGroupCount");

    NxU32 effectorCount = 0;
    stream.readDword(effectorCount, "effectorCount");

    NxU32 fluidCount = 0;
    stream.readDword(fluidCount, "fluidCount");

	NxU32 clothCount = 0;
	stream.readDword(clothCount, "clothCount");

    NxU32 modelCount = 0;
    stream.readDword(modelCount, "modelCount");

    char tempString[NXU_STREAM_MAX_STRING_LENGTH];
    memset(tempString, 0, NXU_STREAM_MAX_STRING_LENGTH);



    for (NxU32 i=0; i<matCount; ++i)
    {
        stream.readHeader(header);

	char tmp[100];
	sprintf(tmp, "ID:%d", header.sectionType);
//	MessageBox(0, tmp, 0, 0);

        NX_ASSERT(NxuTypeMaterial == header.sectionType);
        sprintf(tempString, "Material_%d",i);
        NxuMaterialDesc* desc = new NxuMaterialDesc();
        desc->read(stream, tempString);
        mMaterials.pushBack(desc);
    }
    
    for (NxU32 i=0; i<actorCount; ++i)
    {
        stream.readHeader(header);
        NX_ASSERT(NxuTypeActor == header.sectionType);
        sprintf(tempString, "Actor_%d",i);
        NxuActorDesc* desc = new NxuActorDesc();
        desc->read(stream, tempString);
		char tmp[200];
		sprintf(tmp, "%3.3f, %3.3f, %3.3f", desc->globalPose.t.x, desc->globalPose.t.y, desc->globalPose.t.z);
//		MessageBox(0, tmp, 0, 0);
        mActors.pushBack(desc);
    }
    
    for (NxU32 i=0; i<jointCount; ++i)
    {
        sprintf(tempString, "Joint_%d",i);
        NxuJointDesc* desc = readJoint(stream, tempString);
        mJoints.pushBack(desc);
    }
    
    for (NxU32 i=0; i<pairFlagCount; ++i)
    {
        stream.readHeader(header);
        NX_ASSERT(NxuTypePairFlag == header.sectionType);
        NxuPairFlagDesc* desc = new NxuPairFlagDesc();
        sprintf(tempString, "PairFlag_%d",i);
        desc->read(stream, tempString);
        mPairFlags.pushBack(desc);
    }
    
    for (NxU32 i=0; i<effectorCount; ++i)
    {
        stream.readHeader(header);
        NX_ASSERT(NxuTypeEffector == header.sectionType);
        NxuSpringAndDamperEffectorDesc* desc = new NxuSpringAndDamperEffectorDesc();
        sprintf(tempString, "Effector_%d",i);
        desc->read(stream, tempString);
        mEffectors.pushBack(desc);
    }

#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
    for (NxU32 i=0; i<fluidCount; ++i)
    {
        stream.readHeader(header);
        NX_ASSERT(NxuTypeFluid == header.sectionType);
        NxuFluidDesc* desc = new NxuFluidDesc();
        sprintf(tempString, "Fluid_%d",i);
        desc->read(stream, tempString);
        mFluids.pushBack(desc);
    }
#endif

    for (NxU32 i=0;i<modelCount;++i)
    {
        stream.readHeader(header);
        NX_ASSERT(NxuTypeModelInstance == header.sectionType);
        sprintf(tempString, "Instanced Model_%d",i);
        NxuPhysicsModelInstance* desc = new NxuPhysicsModelInstance();
        desc->read(stream, tempString);
        mModels.pushBack(desc);
    }

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeModel : NxuTypeUnknown;
}

NxuSectionTypes NxuSceneDesc::store(NxuOutputStream &stream, const char* name)
{
    NxuFormatHeader header;
    header.sectionType = NxuTypeScene;
    header.size =  0; //Can't predict the size of a scene well
    strcpy(header.name, name ? name : "");
    stream.storeHeader(header);

    stream.storeBool(groundPlane, "groundPlane");
    stream.storeBool(boundsPlanes, "boundsPlanes");
    stream.storeVector(gravity, "gravity");
    stream.storeDword(timeStepMethod, "timeStepMethod");
    stream.storeFloat(maxTimestep, "maxTimestep");
    stream.storeDword(maxIter, "maxIter");
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HARDWARE
    stream.storeDword(simType, "simType");
    stream.storeDword(hwSceneType, "hwSceneType");
#endif

#if NXU_SDK_FEATURES & NXU_SDK_HAS_PIPELINE
    stream.storeDword(pipelineSpec, "pipelineSpec");
#endif

    stream.storeBool(limits ? true : false, "hasLimits");

	if ( limits )
	{
        stream.storeDword(limits->maxNbActors, "limits.maxActors");
        stream.storeDword(limits->maxNbBodies, "limits.maxBodies");
        stream.storeDword(limits->maxNbStaticShapes, "limits.maxStaticShapes");
        stream.storeDword(limits->maxNbDynamicShapes, "limits.maxDynamicShapes");
        stream.storeDword(limits->maxNbJoints, "limits.maxJoints");
	}

    stream.storeBool(maxBounds ? true : false, "hasBounds");

	if ( maxBounds )
	{
        stream.storeVector(maxBounds->min, "min");
        stream.storeVector(maxBounds->max, "max");
	}
#if NXU_SDK_FEATURES & NXU_SDK_HAS_THREADING
    stream.storeDword(internalThreadCount, "internalThreadCount");
    stream.storeDword(threadMask, "threadMask");
#endif
    sprintf(mModel->name, "%s_%s", name, "contents");
    mModel->store(stream, mModel->name);

    stream.endSection();

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeScene : NxuTypeUnknown;
}

NxuSectionTypes NxuSceneDesc::read(NxuInputStream &stream, const char* name)
{
    stream.readBool(*((NX_BOOL*)&groundPlane), "groundPlane");
    stream.readBool(*((NX_BOOL*)&boundsPlanes), "boundsPlanes");
    stream.readVector(gravity, "gravity");
    stream.readDword(*(NxU32*)&timeStepMethod, "timeStepMethod");
    stream.readFloat(maxTimestep, "maxTimestep");
    stream.readDword(maxIter, "maxIter");
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HARDWARE
    if (stream.getSDKVersion() & NXU_SDK_HAS_HARDWARE)
    {
        NxU32 type;
        stream.readDword(type, "simType");
        simType = (NxSimulationType)type;
        stream.readDword(type, "hwSceneType");
        hwSceneType = (NxHwSceneType)type;
    }
    else
    {
        simType = NX_SIMULATION_SW;
        hwSceneType = NX_HW_SCENE_TYPE_RB;
    }
#else
    if (stream.getSDKVersion() & NXU_SDK_HAS_HARDWARE)
    {
        NxU32 dummy;
        stream.readDword(dummy, "simType");
        stream.readDword(dummy, "hwSceneType");
    }
#endif

#if NXU_SDK_FEATURES & NXU_SDK_HAS_PIPELINE
    if (stream.getSDKVersion()  & NXU_SDK_HAS_PIPELINE)
    {
        NxU32 spec;
        stream.readDword(spec, "pipelineSpec");
        pipelineSpec = (NxHwPipelineSpec)spec;
    }
    else
    {
        pipelineSpec = NX_HW_RB_PIPELINE_HLP_ONLY;
    }
#else
    if (stream.getSDKVersion()  & NXU_SDK_HAS_PIPELINE)
    {
        NxU32 dummy;
        stream.readDword(dummy, "pipelineSpec");
    }
#endif

    NX_BOOL hasLimits = false;
    stream.readBool(hasLimits, "hasLimits");

	if ( hasLimits )
	{
        limits = new NxSceneLimits();
        stream.readDword(limits->maxNbActors, "limits.maxActors");
        stream.readDword(limits->maxNbBodies, "limits.maxBodies");
        stream.readDword(limits->maxNbStaticShapes, "limits.maxStaticShapes");
        stream.readDword(limits->maxNbDynamicShapes, "limits.maxDynamicShapes");
        stream.readDword(limits->maxNbJoints, "limits.maxJoints");
	}

    NX_BOOL hasBounds = false;
    stream.readBool(hasBounds, "hasBounds");

	if ( hasBounds )
	{
        maxBounds = new NxBounds3();
        stream.readVector(maxBounds->min, "min");
        stream.readVector(maxBounds->max, "max");
	}

#if NXU_SDK_FEATURES & NXU_SDK_HAS_THREADING
    if (stream.getSDKVersion() & NXU_SDK_HAS_THREADING)
    {
        stream.readDword(internalThreadCount, "internalThreadCount");
        stream.readDword(threadMask, "threadMask");
    }
    else
    {
        internalThreadCount =0;
        threadMask = 0;
    }
    customScheduler = 0;
#else
    if (stream.getSDKVersion() & NXU_SDK_HAS_THREADING)
    {
        NxU32 dummy;
        stream.readDword(dummy, "internalThreadCount");
        stream.readDword(dummy, "threadMask");
    }
#endif

    NxuFormatHeader header;
    stream.readHeader(header);
    NX_ASSERT(NxuTypeModel == header.sectionType);
    mModel = new NxuPhysicsModel();
    sprintf(mModel->name, "%s_%s", name, "contents");
    mModel->read(stream, mModel->name);

    return (stream.errorCode == NXU_NO_ERROR) ? NxuTypeScene : NxuTypeUnknown;
}

NxuPhysicsCollection::~NxuPhysicsCollection()
{
    while( !convexMeshes.isEmpty())
    {
        NxuConvexMeshDesc* d = convexMeshes.back();
        convexMeshes.popBack();
        delete d;
    }
    while( !models.isEmpty())
    {
        NxuPhysicsModel* d = models.back();
        models.popBack();
        delete d;
    }
    while( !scenes.isEmpty())
    {
        NxuSceneDesc* d = scenes.back();
        scenes.popBack();
        delete d;
    }
   while( !sdkParams.isEmpty())
    {
        NxuParameterData* d = sdkParams.back();
        sdkParams.popBack();
        delete d;
    }
    while( !skeletons.isEmpty())
    {
        NxuCCDSkeletonDesc* d = skeletons.back();
        skeletons.popBack();
        delete d;
    }
    while( !triangleMeshes.isEmpty())
    {
        NxuTriangleMeshDesc* d = triangleMeshes.back();
        triangleMeshes.popBack();
        delete d;
    }
}

NxuSectionTypes NxuPhysicsCollection::store(NxuOutputStream & stream, const char*  name)
{
    NxuFormatHeader mBaseHeader;
    char tempString[NXU_STREAM_MAX_STRING_LENGTH];
    memset(tempString, 0, NXU_STREAM_MAX_STRING_LENGTH);
    NxU32 numParams = sdkParams.size();
    NxU32 numConvexes = convexMeshes.size();
    NxU32 numTrimesh = triangleMeshes.size();
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
    NxU32 numHeightFields = heightFields.size();
#else
    NxU32 numHeightFields = 0;
#endif 
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
    //Write skeletons
    NxU32 numSkeletons = skeletons.size();
#else
    NxU32 numSkeletons = 0;
#endif 
    NxU32 numShapes = defaultActor.shapes.size();
    NxU32 numMats = defaultModel.mMaterials.size();
    NxU32 numActors = defaultModel.mActors.size();
    NxU32 numJoints = defaultModel.mJoints.size();
    NxU32 numPairFlags = defaultModel.mPairFlags.size();
    NxU32 numEffectors = defaultModel.mEffectors.size();
#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
    NxU32 numFluids = defaultModel.mFluids.size();
#else
	NxU32 numFluids = 0;
#endif
	NxU32 numClothes = 0;
    NxU32 numModels= models.size();
    NxU32 numScenes= scenes.size();

    stream.indexLookup.collection = this;
	char tmp[100];
	sprintf(tmp, "Stote NumParams: %d", numParams);
	//MessageBox(0, tmp, 0, 0);

    //Write a global header if writing out more than one object or writting out SDK params
    if (numParams || 
        (1 < numConvexes + numTrimesh + numSkeletons + numShapes + numMats + numActors + numJoints + numPairFlags + numEffectors + numModels + numScenes)
        )
    {
		//MessageBox(0, "222", 0, 0);
        mBaseHeader.sectionType = NxuTypeSDK;
        mBaseHeader.size = 0;
        sprintf(mBaseHeader.name,"PhysX_SDK");
        stream.storeHeader(mBaseHeader);

        stream.storeDword(numParams, "numParams");
        stream.storeDword(numConvexes, "numConvexes");
        stream.storeDword(numTrimesh, "numTrimesh");
        stream.storeDword(numHeightFields, "numHeightFields");
        stream.storeDword(numSkeletons, "numSkeletons");
        stream.storeDword(numShapes, "numShapes");
        stream.storeDword(numMats, "numMats");
        stream.storeDword(numActors, "numActors");
        stream.storeDword(numJoints, "numJoints");
        stream.storeDword(numPairFlags, "numPairFlags");
        stream.storeDword(numEffectors, "numEffectors");
#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
        stream.storeDword(numFluids, "numFluids");
#endif
		stream.storeDword(numClothes, "numClothes");
        stream.storeDword(numModels, "numModels");
        stream.storeDword(numScenes, "numScenes");

        //Write paramters
        for (NxU32 i=0; i<numParams; ++i)
        {
            stream.storeParameter(*(sdkParams[i]), NxuParameterData::GetParamName(sdkParams[i]->param));
        }

        stream.endSection();
    }
    else
    {
        if( numConvexes) mBaseHeader.sectionType = NxuTypeConvexMesh;
        if( numTrimesh) mBaseHeader.sectionType = NxuTypeTriangleMesh;
        if (numHeightFields) mBaseHeader.sectionType = NxuTypeHeightField;
        if( numSkeletons) mBaseHeader.sectionType = NxuTypeCCDSkeleton;
        if( numShapes) mBaseHeader.sectionType = NxuTypeShape;
        if( numMats) mBaseHeader.sectionType = NxuTypeMaterial;
        if( numActors) mBaseHeader.sectionType = NxuTypeActor;
        if( numJoints) mBaseHeader.sectionType = NxuTypeJoint;
        if( numPairFlags) mBaseHeader.sectionType = NxuTypePairFlag;
        if( numEffectors) mBaseHeader.sectionType = NxuTypeEffector;
        if (numFluids) mBaseHeader.sectionType = NxuTypeFluid;
        if( numModels) mBaseHeader.sectionType = NxuTypeModel;
        if( numScenes) mBaseHeader.sectionType = NxuTypeScene;
    }

    //Write convexes
    for (NxU32 i=0; i<numConvexes; ++i)
    {
        sprintf(tempString, "ConvexMesh_%d", i);
        convexMeshes[i]->store(stream, tempString);
    }

    //Write trimesh
    for (NxU32 i=0; i<numTrimesh; ++i)
    {
        sprintf(tempString, "TriangleMesh_%d", i);
        triangleMeshes[i]->store(stream, tempString);
    }

#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD    
    //Write heightfields
	for	( NxU32	i=0; i<numHeightFields; ++i )
    {
        sprintf(tempString, "Heightfield_%d",i);
        heightFields[i]->store(stream, tempString);
	}
#endif

#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
    //Write skeletons
    for (NxU32 i=0; i<numSkeletons; ++i)
    {
        sprintf(tempString, "CCDSkeleton_%d", i);
        skeletons[i]->store(stream, tempString);
    }
#endif

    //Write defaults (unstructured data)
	for	( NxU32	i=0; i<numShapes; ++i )
    {
        sprintf(tempString, "__templateShape_%d",i);
        ((NxuShapeDesc*)(defaultActor.shapes[i]))->store(stream, tempString);
	}

    stream.indexLookup.model = &defaultModel;

	for	( NxU32	i=0; i<numMats; ++i )
    {
        sprintf(tempString, "__templateMaterial_%d",i);
        defaultModel.mMaterials[i]->store(stream, tempString);
	}

	for	( NxU32	i=0; i<numActors; ++i )
    {
        sprintf(tempString, "__templateActor_%d",i);
        defaultModel.mActors[i]->store(stream, tempString);
	}

	for	( NxU32	i=0; i<numJoints; ++i )
    {
        sprintf(tempString, "__templateJoint_%d",i);
        defaultModel.mJoints[i]->store(stream, tempString);
	}

	for	( NxU32	i=0; i<numPairFlags; ++i )
    {
        sprintf(tempString, "__templatePairFlag_%d",i);
        defaultModel.mPairFlags[i]->store(stream, tempString);
	}

	for	( NxU32	i=0; i<numEffectors; ++i )
    {
        sprintf(tempString, "__templateEffector_%d",i);
        defaultModel.mEffectors[i]->store(stream, tempString);
	}

#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
	for	( NxU32	i=0; i<numFluids; ++i )
    {
        sprintf(tempString, "__templateFluid_%d",i);
        defaultModel.mFluids[i]->store(stream, tempString);
	}
#endif

    //Write Models
    for (NxU32 i=0; i<numModels; ++i)
    {
        sprintf(tempString, "Model_%d", i);
        models[i]->store(stream, tempString);
    }

    for (NxU32 i=0; i<numScenes; ++i)
    {
        sprintf(tempString, "Scene_%d", i);
        scenes[i]->store(stream, tempString);
    }
    
    return (stream.errorCode == NXU_NO_ERROR) ? (NxuSectionTypes)mBaseHeader.sectionType : NxuTypeUnknown;
}

#include <windows.h>

NxuSectionTypes NxuPhysicsCollection::read(NxuInputStream & stream, const char*  name)
{
    char tempString[NXU_STREAM_MAX_STRING_LENGTH];
    memset(tempString, 0, NXU_STREAM_MAX_STRING_LENGTH);
    NxuFormatHeader mBaseHeader;
    stream.readHeader(mBaseHeader);

    NxU32 numParams = 0;
    NxU32 numConvexes = 0;
    NxU32 numTrimesh = 0;
    NxU32 numSkeletons = 0;
    NxU32 numShapes = 0;
    NxU32 numMats = 0;
    NxU32 numActors = 0;
    NxU32 numJoints = 0;
    NxU32 numPairFlags = 0;
    NxU32 numEffectors = 0;
    NxU32 numFluids = 0;
    NxU32 numHeightFields = 0;
    NxU32 numModels= 0;
	NxU32 numClothes= 0;
    NxU32 numScenes= 0;

    bool readHeader = true;

    stream.pointerLookup.collection = this;
	stream.pointerLookup.model = &this->defaultModel;

    if ( NxuTypeSDK == mBaseHeader.sectionType)
    {
        stream.readDword(numParams, "numParams");
        stream.readDword(numConvexes, "numConvexes");
        stream.readDword(numTrimesh, "numTrimesh");
        if (stream.getSDKVersion() & NXU_SDK_HAS_HEIGHTFIELD)
        {
            stream.readDword(numHeightFields, "numHeightFields");
        }
        if (stream.getSDKVersion() & NXU_SDK_HAS_SKELETONS)
        {
            stream.readDword(numSkeletons, "numSkeletons");
        }
        stream.readDword(numShapes, "numShapes");
        stream.readDword(numMats, "numMats");
        stream.readDword(numActors, "numActors");
        stream.readDword(numJoints, "numJoints");
        stream.readDword(numPairFlags, "numPairFlags");
        stream.readDword(numEffectors, "numEffectors");
        if (stream.getSDKVersion() & NXU_SDK_HAS_FLUID_API)
        {
            stream.readDword(numFluids, "numFluids");
        }
		stream.readDword(numClothes, "numClothes");
        stream.readDword(numModels, "numModels");
        stream.readDword(numScenes, "numScenes");
        readHeader = true;
    } 
    else 
    {
        switch(mBaseHeader.sectionType)
        {
        case NxuTypeScene:
            numScenes = 1;
            break;
        case NxuTypeModel:
            numModels = 1;
            break;
        case NxuTypeActor:
            numActors = 1;
            break;
        case NxuTypeJoint:
            numJoints = 1;
            break;
        case NxuTypeMaterial:
            numMats = 1;
            break;
        case NxuTypePairFlag:
            numPairFlags = 1;
            break;
        case NxuTypeShape:
            numShapes = 1;
            break;
        case NxuTypeTriangleMesh:
            numTrimesh = 1;
            break;
        case NxuTypeConvexMesh:
            numConvexes = 1;
            break;
        case NxuTypeCCDSkeleton:
            numSkeletons = 1;
            break;
        case NxuTypeEffector:
            numEffectors = 1;
            break;
        case NxuTypeFluid:
            numFluids = 1;
            break;			
        case NxuTypeHeightField:
            numHeightFields = 1;
            break;
        case NxuTypeModelInstance:
            NX_ASSERT(0);
            break;
        case NxuTypeUnknown:
            NX_ASSERT(0);
            break;
        default:
            NX_ASSERT(0);
        }
        readHeader = false;
    }

	char tmp[100];
	sprintf(tmp, "NumParams: %d", numParams);
	//MessageBox(0, tmp, 0, 0);

    //Read paramters
    for (NxU32 i=0; i<numParams; ++i)
    {
        NxuParameterData* param = new NxuParameterData;
        stream.readParameter(*(param));
        sdkParams.pushBack(param);
    }

    //Read convexes
    for (NxU32 i=0; i<numConvexes; ++i)
    {
        NxuFormatHeader header;
        if (readHeader)
        {
            stream.readHeader(header);
        }
        else
        {
            header = mBaseHeader;
        }
        NX_ASSERT(NxuTypeConvexMesh == header.sectionType);
        sprintf(tempString, "ConvexMesh_%d", i);
        NxuConvexMeshDesc* desc = new NxuConvexMeshDesc();
        desc->read(stream, tempString);
        convexMeshes.pushBack(desc);
    }

    //Read trimesh
    for (NxU32 i=0; i<numTrimesh; ++i)
    {
        NxuFormatHeader header;
        if (readHeader)
        {
            stream.readHeader(header);
        }
        else
        {
            header = mBaseHeader;
        }
        NX_ASSERT(NxuTypeTriangleMesh == header.sectionType);
        sprintf(tempString, "TriangleMesh_%d", i);
        NxuTriangleMeshDesc* desc = new NxuTriangleMeshDesc();
        desc->read(stream, tempString);
        triangleMeshes.pushBack(desc);
    }

    for (NxU32 i=0; i<numHeightFields; ++i)
    {
        NxuFormatHeader header;
        if (readHeader)
        {
            stream.readHeader(header);
        }
        else
        {
            header = mBaseHeader;
        }
        NX_ASSERT(NxuTypeHeightField == header.sectionType);
#if NXU_SDK_FEATURES & NXU_SDK_HAS_HEIGHTFIELD
        sprintf(tempString, "Heightfield_%d", i);
        NxuHeightFieldDesc* desc = new NxuHeightFieldDesc();
        desc->read(stream, tempString);
        heightFields.pushBack(desc);
#else
        stream.skip();
#endif
    }

    for (NxU32 i=0; i<numSkeletons; ++i)
    {
        NxuFormatHeader header;
        if (readHeader)
        {
            stream.readHeader(header);
        }
        else
        {
            header = mBaseHeader;
        }
        NX_ASSERT(NxuTypeCCDSkeleton == header.sectionType);
#if NXU_SDK_FEATURES & NXU_SDK_HAS_SKELETONS
    //Read skeletons
        sprintf(tempString, "CCDSkeleton_%d", i);
        NxuCCDSkeletonDesc* desc = new NxuCCDSkeletonDesc();
        desc->read(stream, tempString);
        skeletons.pushBack(desc);
#else
        stream.skip();
#endif
    }

    //Read defaults (unstructured data)
	for	( NxU32	i=0; i<numShapes; ++i )
    {
        NxuFormatHeader header;
        if (readHeader)
        {
            stream.readHeader(header);
        }
        else
        {
            header = mBaseHeader;
        }
        NX_ASSERT(NxuTypeShape == header.sectionType);
        sprintf(tempString, "__templateShape_%d",i);
        NxuShapeDesc* desc = readShape(stream, tempString);
        defaultActor.shapes.pushBack(desc->getShapeDesc());
	}

	for	( NxU32	i=0; i<numMats; ++i )
    {
        NxuFormatHeader header;
        if (readHeader)
        {
            stream.readHeader(header);
        }
        else
        {
            header = mBaseHeader;
        }
        NX_ASSERT(NxuTypeMaterial == header.sectionType);
        sprintf(tempString, "__templateMaterial_%d",i);
        NxuMaterialDesc* desc = new NxuMaterialDesc();
        desc->read(stream, tempString);
        defaultModel.mMaterials.pushBack(desc);
	}

	for	( NxU32	i=0; i<numActors; ++i )
    {
        NxuFormatHeader header;
        if (readHeader)
        {
            stream.readHeader(header);
        }
        else
        {
            header = mBaseHeader;
        }
        NX_ASSERT(NxuTypeActor == header.sectionType);
        sprintf(tempString, "__templateActor_%d",i);
        NxuActorDesc* desc = new NxuActorDesc();
        desc->read(stream, tempString);
        defaultModel.mActors.pushBack(desc);
	}

	for	( NxU32	i=0; i<numJoints; ++i )
    {
        //NxuFormatHeader header;
        //if (readHeader)
        //{
        //    stream.readHeader(header);
        //}
        //else
        //{
        //    header = mBaseHeader;
        //}
        //NX_ASSERT(NxuTypeJoint == header.sectionType);
        sprintf(tempString, "__templateJoint_%d",i);
        NxuJointDesc* desc = readJoint(stream, tempString);
        defaultModel.mJoints.pushBack(desc);
	}

	for	( NxU32	i=0; i<numPairFlags; ++i )
    {
        NxuFormatHeader header;
        if (readHeader)
        {
            stream.readHeader(header);
        }
        else
        {
            header = mBaseHeader;
        }
        NX_ASSERT(NxuTypePairFlag == header.sectionType);
        sprintf(tempString, "__templatePairFlag_%d",i);
        NxuPairFlagDesc* desc = new NxuPairFlagDesc();
        desc->read(stream, tempString);
        defaultModel.mPairFlags.pushBack(desc);
	}

	for	( NxU32	i=0; i<numEffectors; ++i )
    {
        NxuFormatHeader header;
        if (readHeader)
        {
            stream.readHeader(header);
        }
        else
        {
            header = mBaseHeader;
        }
        NX_ASSERT(NxuTypeEffector == header.sectionType);
        sprintf(tempString, "__templateEffector_%d",i);
        NxuSpringAndDamperEffectorDesc* desc = new NxuSpringAndDamperEffectorDesc();
        desc->read(stream, tempString);
        defaultModel.mEffectors.pushBack(desc);
	}

    for (NxU32 i=0; i<numFluids; ++i)
    {
        NxuFormatHeader header;
        if (readHeader)
        {
            stream.readHeader(header);
        }
        else
        {
            header = mBaseHeader;
        }
        NX_ASSERT(NxuTypeFluid == header.sectionType);
#if NXU_SDK_FEATURES & NXU_SDK_HAS_FLUID_API
        sprintf(tempString, "Fluid_%d", i);
        NxuFluidDesc* desc = new NxuFluidDesc();
        desc->read(stream, tempString);
        defaultModel.mFluids.pushBack(desc);
#else
        stream.skip();
#endif
    }

    //Read Models
    for (NxU32 i=0; i<numModels; ++i)
    {
        NxuFormatHeader header;
        if (readHeader)
        {
            stream.readHeader(header);
        }
        else
        {
            header = mBaseHeader;
        }
        NX_ASSERT(NxuTypeModel == header.sectionType);
        sprintf(tempString, "Model_%d", i);
        NxuPhysicsModel* desc = new NxuPhysicsModel();
        desc->read(stream, tempString);
		models.pushBack(desc);
        //models[i]->read(stream, tempString);
    }

    for (NxU32 i=0; i<numScenes; ++i)
    {
        NxuFormatHeader header;
        if (readHeader)
        {
            stream.readHeader(header);
        }
        else
        {
            header = mBaseHeader;
        }
        NX_ASSERT(NxuTypeScene == header.sectionType);
        sprintf(tempString, "Scene_%d", i);
        NxuSceneDesc* scene = new NxuSceneDesc();
        scene->read(stream, tempString);
        scenes.pushBack(scene);
    }

    return (stream.errorCode == NXU_NO_ERROR) ? (NxuSectionTypes)mBaseHeader.sectionType : NxuTypeUnknown;
}



template<class T> NxU32 NxuPointerToIndex(const NxArray<T> & array, void* d)
{
    NxU32 count = array.size();
    if (d)
    {
        for (NxU32 i=0; i<count; ++i)
        {
            if (d == array[i]) {
                return i;
            }
        }
    }
    return (NxU32)-1;  
};

template<class T> void* NxuIndexToPointer(const NxArray<T> & array, NxU32 index)
{
    if (index != (NxU32)-1 && index < array.size())
    {
        return array[index];
    }
    else
    {
        return 0;
    }
};

//Callback mechanism to convert pointers to and from serialized form

NxU32 NxuIndexLookup::pointerToIndex(NxuActorDesc* p)
{
   return NxuPointerToIndex(model->mActors, p);
}

NxU32 NxuIndexLookup::pointerToIndex(NxuPhysicsModel* p)
{
   return NxuPointerToIndex(collection->models, p);
}
NxU32 NxuIndexLookup::pointerToIndex(NxuCCDSkeletonDesc* p)
{
   return NxuPointerToIndex(collection->skeletons, p);
}
NxU32 NxuIndexLookup::pointerToIndex(NxuConvexMeshDesc* p)
{
   return NxuPointerToIndex(collection->convexMeshes, p);
}
NxU32 NxuIndexLookup::pointerToIndex(NxuTriangleMeshDesc* p)
{
   return NxuPointerToIndex(collection->triangleMeshes, p);
}
NxU32 NxuIndexLookup::pointerToIndex(NxuHeightFieldDesc* p)
{
   return NxuPointerToIndex(collection->heightFields, p);
}

void NxuPointerLookup::indexToPointer(NxU32 index, NxuActorDesc** p)
{
   *p = (NxuActorDesc*)NxuIndexToPointer(model->mActors, index);
}
void NxuPointerLookup::indexToPointer(NxU32 index, NxuPhysicsModel** p)
{
   *p = (NxuPhysicsModel*)NxuIndexToPointer(collection->models, index);
}
void NxuPointerLookup::indexToPointer(NxU32 index, NxuCCDSkeletonDesc** p)
{
   *p = (NxuCCDSkeletonDesc*)NxuIndexToPointer(collection->skeletons, index);
}
void NxuPointerLookup::indexToPointer(NxU32 index, NxuConvexMeshDesc** p)
{
   *p = (NxuConvexMeshDesc*)NxuIndexToPointer(collection->convexMeshes, index);
}
void NxuPointerLookup::indexToPointer(NxU32 index, NxuTriangleMeshDesc** p)
{
   *p = (NxuTriangleMeshDesc*)NxuIndexToPointer(collection->triangleMeshes, index);
}
void NxuPointerLookup::indexToPointer(NxU32 index, NxuHeightFieldDesc** p)
{
   *p = (NxuHeightFieldDesc*)NxuIndexToPointer(collection->heightFields, index);
}


//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND