/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		NxuAsciiStream.h

		This header describes the interface for loading and saving physics data to an ascii stream.

		Changelist
		----------

*/
#ifndef NXU_ASCII_STREAM_H
#define NXU_ASCII_STREAM_H

#include <stdio.h>

#include "NxuStream.h"

/**
\brief Reads and writes a acsii text format file.

This file format is intended to help debug problems by providing a human readable version that mirrors
the NxuBinaryStream format.

@see NxuStreamFactory NxuOutputStream NxuInputStream NxuBinaryStream
*/
class NxuAsciiStream : public NxuOutputStream, public NxuInputStream
{
protected:
    FILE* mFilePointer;
    char mFileName[1024];
    NX_BOOL mIsOutputStream;
    int tabLevel;
    char temp[1024];
    NxU32 sdkVersion;

    void writeLine(const char* buffer);

public:
    NxU32 mFormat;

    NxuAsciiStream();
    virtual ~NxuAsciiStream();

    /**
    \brief Opens a file for reading or writing.

    \param[in] filename The name of the file to open.
    \param[in] writeable True if the file is going to be written to, false if it will be read from.
    \return True if the file was successfully opened.

    \warning Reading from ASCII files is not currently supported.
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

    virtual		NxuAsciiStream&	readBool(NX_BOOL& b, const char* name = NULL);
    virtual		NxuAsciiStream&	readByte(NxU8& b, const char* name = NULL);
	virtual		NxuAsciiStream&	readWord(NxU16 &w, const char* name = NULL);
	virtual		NxuAsciiStream&	readDword(NxU32 &i, const char* name = NULL);
	virtual		NxuAsciiStream&	readFloat(NxF32 &f, const char* name = NULL);
	virtual		NxuAsciiStream&	readDouble(NxF64 &d, const char* name = NULL);
	virtual		NxuAsciiStream&	readByteBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuAsciiStream&	readWordBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuAsciiStream&	readDwordBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuAsciiStream&	readFloatBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuAsciiStream&	readDoubleBuffer(void* buffer, NxU32 size, const char* name = NULL);
	virtual		NxuAsciiStream&	readCharBuffer(void* buffer, NxU32 size, const char* name = NULL);

    virtual void readHeader(NxuFormatHeader &header);
	virtual void skip();  //Skips the section covered by the most recent header

	virtual NxuAsciiStream& readVector(NxVec3 &vec, const char* name = NULL);
	virtual NxuAsciiStream& readQuat(NxQuat &quat, const char* name = NULL);
	virtual NxuAsciiStream& readMatrix(NxMat34 &mat, const char* name = NULL);
	virtual NxuAsciiStream& readParameter(NxuParameterData& param, const char* name = NULL);

    virtual		NxuAsciiStream&		storeBool(NX_BOOL b, const char* name = NULL);
    virtual		NxuAsciiStream&		storeByte(NxU8 b, const char* name = NULL);
	virtual		NxuAsciiStream&		storeWord(NxU16 w, const char* name= NULL);
	virtual		NxuAsciiStream&		storeDword(NxU32 d, const char* name= NULL);
	virtual		NxuAsciiStream&		storeFloat(NxF32 f, const char* name= NULL);
	virtual		NxuAsciiStream&		storeDouble(NxF64 f, const char* name= NULL);
	virtual		NxuAsciiStream&		storeByteBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuAsciiStream&		storeWordBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuAsciiStream&		storeDwordBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuAsciiStream&		storeFloatBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuAsciiStream&		storeDoubleBuffer(const void* buffer, NxU32 size, const char* name= NULL);
	virtual		NxuAsciiStream&		storeCharBuffer(const void* buffer, NxU32 size, const char* name= NULL);

    virtual void storeHeader(const NxuFormatHeader & header);
	virtual void endSection(); 
	virtual NxuAsciiStream& storeVector(const NxVec3 &vec, const char* name= NULL);
	virtual NxuAsciiStream& storeQuat(const NxQuat &quat, const char* name= NULL);
	virtual NxuAsciiStream& storeMatrix(const NxMat34 &mat, const char* name= NULL);
	virtual NxuAsciiStream& storeParameter(const NxuParameterData& param, const char* name= NULL);
};


#endif //NXU_ASCII_STREAM_H


//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND

