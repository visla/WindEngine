/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		NxuBinaryStream.h

		This header describes the interface for loading and saving physics data to a binary stream.

		Changelist
		----------

*/
#ifndef NXU_BINARY_STREAM_H
#define NXU_BINARY_STREAM_H

#include <stdio.h>

#include "NxuStream.h"

/**
\brief Reads and writes a binary format file.

@see NxuStreamFactory NxuOutputStream NxuInputStream
*/
class NxuBinaryStream : public NxuOutputStream, public NxuInputStream
{
protected:
    FILE* mFilePointer;
    char mFileName[1024];
    NX_BOOL mIsOutputStream;
    NxU32 currentSize;
    NxU32 sdkVersion;

public:
    NxU32 mFormat;
    /**
	\brief True if the file uses the same byte order as the current application.

    This can be set to false when writing to force the file to output bytes in reverse endian order.
	*/
    NX_BOOL mismatch;

    NxuBinaryStream();
    virtual ~NxuBinaryStream();

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

    virtual     NxU32           getFormat() {return mFormat;}
    virtual void                flush() {if (mFilePointer) fflush(mFilePointer);}
    virtual void                setSDKVersion(NxU32 version) {sdkVersion = version;}
    virtual NxU32               getSDKVersion() {return sdkVersion;}

    virtual		NxuBinaryStream&	readBool(NX_BOOL& b, const char* name = NULL);
    virtual		NxuBinaryStream&	readByte(NxU8& b, const char* name = NULL);
	virtual		NxuBinaryStream&	readWord(NxU16 &w, const char* name = NULL);
	virtual		NxuBinaryStream&	readDword(NxU32 &i, const char* name = NULL);
	virtual		NxuBinaryStream&	readFloat(NxF32 &f, const char* name = NULL);
	virtual		NxuBinaryStream&	readDouble(NxF64 &d, const char* name = NULL);
	virtual		NxuBinaryStream&	readByteBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuBinaryStream&	readWordBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuBinaryStream&	readDwordBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuBinaryStream&	readFloatBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuBinaryStream&	readDoubleBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuBinaryStream&	readCharBuffer(void* buffer, NxU32 size, const char* name = NULL);

    virtual void readHeader(NxuFormatHeader &header);
	virtual void skip();  //Skips the section covered by the most recent header

	virtual NxuBinaryStream& readVector(NxVec3 &vec, const char* name = NULL);
	virtual NxuBinaryStream& readQuat(NxQuat &quat, const char* name = NULL);
	virtual NxuBinaryStream& readMatrix(NxMat34 &mat, const char* name = NULL);
	virtual NxuBinaryStream& readParameter(NxuParameterData& param, const char* name = NULL);

    virtual		NxuBinaryStream&		storeBool(NX_BOOL b, const char* name = NULL);
    virtual		NxuBinaryStream&		storeByte(NxU8 b, const char* name = NULL);
	virtual		NxuBinaryStream&		storeWord(NxU16 w, const char* name= NULL);
	virtual		NxuBinaryStream&		storeDword(NxU32 d, const char* name= NULL);
	virtual		NxuBinaryStream&		storeFloat(NxF32 f, const char* name= NULL);
	virtual		NxuBinaryStream&		storeDouble(NxF64 f, const char* name= NULL);
	virtual		NxuBinaryStream&		storeByteBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuBinaryStream&		storeWordBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuBinaryStream&		storeDwordBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuBinaryStream&		storeFloatBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuBinaryStream&		storeDoubleBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuBinaryStream&		storeCharBuffer(const void* buffer, NxU32 size, const char* name= NULL);

    virtual void storeHeader(const NxuFormatHeader & header);
	virtual void endSection(); 

	virtual NxuBinaryStream& storeVector(const NxVec3 &vec, const char* name= NULL);
	virtual NxuBinaryStream& storeQuat(const NxQuat &quat, const char* name= NULL);
	virtual NxuBinaryStream& storeMatrix(const NxMat34 &mat, const char* name= NULL);
	virtual NxuBinaryStream& storeParameter(const NxuParameterData& param, const char* name= NULL);
};


#endif //NXU_BINARY_STREAM_H


//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND

