/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		NxuColladaStream.h

		This header describes the interface for loading and saving physics data to an XML file.

		Changelist
		----------

*/
#ifndef NXU_COLLADA_STREAM_H
#define NXU_COLLADA_STREAM_H

#include <stdio.h>

#include "NxuStream.h"

#include "array.h"
#include "nxmlparse.h"

/**
\brief Reads and writes an XML file format based on the COLLADA digital asset schema

COLLADA is a standard file format for representing graphics and physics data.
http://www.khronos.org/collada/

Note that 1.3.1 and earlier versions of collada do not support physics data, so this exporter will 
not produce validating XML against those schemas.

@see NxuStreamFactory NxuOutputStream NxuInputStream
*/
class NxuColladaStream : public NxuOutputStream, public NxuInputStream
{
protected:
    FILE* mFilePointer;
    char mFileName[1024];
    NX_BOOL mIsOutputStream;
    NxU32 sdkVersion;
    NxU32 mParamIndex;
    int lastHeader;

    pxArray<nxmlNode*> nodestack;
    nxmlNode* collada;
    nxmlNode* library_physics_scenes;
    nxmlNode* library_physics_models;
    nxmlNode* library_physics_materials;
    nxmlNode* library_geometries;
    nxmlNode* current_physics_scene;
    nxmlNode* current_physics_model;
    nxmlNode* current_physics_material;
    nxmlNode* current_rigid_body;
    nxmlNode* current_rigid_constraint;
    nxmlNode* current_shape;
    nxmlNode* current_geometry;
    nxmlNode* current_technique;
    nxmlNode* current_scene;
    nxmlNode* current_model;
    nxmlNode* current_fluid;
    NX_BOOL useTechnique;

    nxmlNode* current_section;

	nxmlNode *current(){return nodestack.count?nodestack[nodestack.count-1]:NULL;}

    void createSection(nxmlNode** index, const char* name, nxmlNode* parent);
    nxmlNode* createTechnique(nxmlNode* base, const char* name);
    void createPhysXandCommonTechniques(nxmlNode* base);    
    void createExtra(nxmlNode** index, const char* name, nxmlNode* parent);
    nxmlNode* getTechnique(nxmlNode* base_node, const char* name);
    void expandNode(nxmlNode* current);
    void fillHeader(NxuFormatHeader &header, NxuSectionTypes type, nxmlNode* current);
    const char* getCurrentBody(const char* name);
    void addNode(const char* tag, const char* body);

    struct nxmlMap
    {
        nxString name;
        nxmlNode* node;
    };
    
    pxArray<nxmlMap*> nxmlLookup;
    nxmlNode* lookupId(const char* id);
    void createLookup(nxmlNode* root);
    nxmlNode* resolveURL(nxmlNode* node);

public:
    NxU32 mFormat;

    NxuColladaStream();
    virtual ~NxuColladaStream();

    /**
    \brief Opens a file for reading or writing.

    \param[in] filename The name of the file to open.
    \param[in] writeable True if the file is going to be written to, false if it will be read from.
    \return True if the file was successfully opened.
    */
    NX_BOOL openFile(const char* filename, NX_BOOL writeable);

    /**
    \brief Closes the file if one was opened.
    */
    void closeFile();

    /**
    \brief Sets a file pointer for reading or writing.

    \return True if the file pointer is valid.
    */
    NX_BOOL setFilePointer(FILE* file, NX_BOOL writeable);

    //
    /**
    \brief Provides access to an XML node representation for adding custom data or post processing.

    \return The root node of the document.
    */
    nxmlNode* getColladaNode() {return collada;}

    virtual     NxU32           getFormat() {return mFormat;} //Returns the format id for this stream
    virtual void                flush();
    virtual void                setSDKVersion(NxU32 version) {sdkVersion = version;}
    virtual NxU32               getSDKVersion() {return sdkVersion;}

    virtual		NxuColladaStream&	readBool(NX_BOOL& b, const char* name = NULL);
    virtual		NxuColladaStream&	readByte(NxU8& b, const char* name = NULL);
	virtual		NxuColladaStream&	readWord(NxU16 &w, const char* name = NULL);
	virtual		NxuColladaStream&	readDword(NxU32 &i, const char* name = NULL);
	virtual		NxuColladaStream&	readFloat(NxF32 &f, const char* name = NULL);
	virtual		NxuColladaStream&	readDouble(NxF64 &d, const char* name = NULL);
	virtual		NxuColladaStream&	readByteBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuColladaStream&	readWordBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuColladaStream&	readDwordBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuColladaStream&	readFloatBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuColladaStream&	readDoubleBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuColladaStream&	readCharBuffer(void* buffer, NxU32 size, const char* name = NULL);

    virtual void readHeader(NxuFormatHeader &header);
	virtual void skip();  //Skips the section covered by the most recent header

	virtual NxuColladaStream& readVector(NxVec3 &vec, const char* name = NULL);
	virtual NxuColladaStream& readQuat(NxQuat &quat, const char* name = NULL);
	virtual NxuColladaStream& readMatrix(NxMat34 &mat, const char* name = NULL);
	virtual NxuColladaStream& readParameter(NxuParameterData& param, const char* name = NULL);

    virtual		NxuColladaStream&		storeBool(NX_BOOL b, const char* name = NULL);
    virtual		NxuColladaStream&		storeByte(NxU8 b, const char* name = NULL);
	virtual		NxuColladaStream&		storeWord(NxU16 w, const char* name= NULL);
	virtual		NxuColladaStream&		storeDword(NxU32 d, const char* name= NULL);
	virtual		NxuColladaStream&		storeFloat(NxF32 f, const char* name= NULL);
	virtual		NxuColladaStream&		storeDouble(NxF64 f, const char* name= NULL);
	virtual		NxuColladaStream&		storeByteBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuColladaStream&		storeWordBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuColladaStream&		storeDwordBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuColladaStream&		storeFloatBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuColladaStream&		storeDoubleBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuColladaStream&		storeCharBuffer(const void* buffer, NxU32 size, const char* name= NULL);

    virtual void storeHeader(const NxuFormatHeader & header);
	virtual void endSection(); 

	virtual NxuColladaStream& storeVector(const NxVec3 &vec, const char* name= NULL);
	virtual NxuColladaStream& storeQuat(const NxQuat &quat, const char* name= NULL);
	virtual NxuColladaStream& storeMatrix(const NxMat34 &mat, const char* name= NULL);
	virtual NxuColladaStream& storeParameter(const NxuParameterData& param, const char* name= NULL);
};


#endif //NXU_COLLADA_STREAM_H


//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND

