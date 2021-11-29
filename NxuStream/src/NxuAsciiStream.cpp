/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		NxuAsciiStream.cpp

		Implementation of saving physics data to a ascii stream.

		Changelist
		----------

*/


#include "NxuAsciiStream.h"

NxuAsciiStream::NxuAsciiStream()
{
    mFilePointer = 0;
    mFileName[0] = 0;
    tabLevel = 0;
    mIsOutputStream = false;
}


//Returns true if the file was successfully opened
NX_BOOL NxuAsciiStream::openFile(const char* filename, NX_BOOL writeable)
{
    NX_ASSERT(strlen(filename) < 1024);
    strncpy(mFileName, filename, 1024);
    return setFilePointer(fopen(mFileName, writeable ? "w" : "r"), writeable);
}

void NxuAsciiStream::closeFile()
{
    if (mFilePointer && mFileName[0] != 0) fclose(mFilePointer);
    mFilePointer = 0;
    mFileName[0] = 0;

}

//Returns true if the file pointer is valid
NX_BOOL NxuAsciiStream::setFilePointer(FILE* file, NX_BOOL writeable)
{
    NX_ASSERT(writeable);
    if (!writeable) return false; //Reading not implemented!

    if (mFilePointer) closeFile(); //Close any previously opened file
    mFilePointer = file;
    if (!file) return false;
    mIsOutputStream = writeable;
    tabLevel = 0;
    return true;
}

NxuAsciiStream::~NxuAsciiStream()
{
    closeFile();
}

NxuAsciiStream&			NxuAsciiStream::readBool(NX_BOOL& b, const char* name)								
{
    NX_ASSERT(!mIsOutputStream);
    NX_ASSERT(!"Not Implemented");

    return *this;
}

NxuAsciiStream&			NxuAsciiStream::readByte(NxU8 &b, const char* name)								
{
    NX_ASSERT(!mIsOutputStream);
    NX_ASSERT(!"Not Implemented");

    return *this;
}

NxuAsciiStream&			NxuAsciiStream::readWord(NxU16 &w, const char* name)								
{

    NX_ASSERT(!"Not Implemented");
    return *this;
}

NxuAsciiStream&			NxuAsciiStream::readDword(NxU32 &i, const char* name)								
{

    NX_ASSERT(!"Not Implemented");
    return *this;
}

NxuAsciiStream&			NxuAsciiStream::readFloat(NxF32 &f, const char* name)								
{

    NX_ASSERT(!"Not Implemented");
    return *this;
}

NxuAsciiStream&			NxuAsciiStream::readDouble(NxF64 &f, const char* name)							
{

    NX_ASSERT(!"Not Implemented");
    return *this;
}

NxuAsciiStream& NxuAsciiStream::readByteBuffer(void* buffer, NxU32 size, const char* name)	
{
    NX_ASSERT(!"Not Implemented");
    NX_ASSERT(!mIsOutputStream);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::readWordBuffer(void* buffer, NxU32 size, const char* name)	
{

    NX_ASSERT(!"Not Implemented");
    return *this;
}

NxuAsciiStream& NxuAsciiStream::readDwordBuffer(void* buffer, NxU32 size, const char* name)	
{
    NX_ASSERT(!"Not Implemented");
    return *this;
}

NxuAsciiStream& NxuAsciiStream::readFloatBuffer(void* buffer, NxU32 size, const char* name)	
{
    NX_ASSERT(!"Not Implemented");
    return *this;
}

NxuAsciiStream& NxuAsciiStream::readDoubleBuffer(void* buffer, NxU32 size, const char* name)	
{
    NX_ASSERT(!"Not Implemented");
    return *this;
}	

NxuAsciiStream&	NxuAsciiStream::readCharBuffer(void* buffer, NxU32 size, const char* name)
{
    NX_ASSERT(!"Not Implemented");
    return *this;
}


void NxuAsciiStream::readHeader(NxuFormatHeader &header)
{
    NX_ASSERT(!"Not Implemented");  
}

void NxuAsciiStream::skip()
{
    NX_ASSERT(!"Not Implemented");
}

NxuAsciiStream& NxuAsciiStream::readVector(NxVec3 &vec, const char* name)
{
    readFloat(vec.x);
    readFloat(vec.y);
    readFloat(vec.z);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::readQuat(NxQuat &quat, const char* name)
{
    readFloat(quat.x);
    readFloat(quat.y);
    readFloat(quat.z);
    readFloat(quat.w);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::readMatrix(NxMat34 &mat, const char* name)
{
	NxF32 m[9];
    readFloatBuffer(m, 9*sizeof(NxF32));
	mat.M.setRowMajor( m	);
    readVector(mat.t);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::readParameter(NxuParameterData& param, const char* name)
{
    readByte(*((NxU8*)&param.param));
    readFloat(param.value);
    return *this;
}

void NxuAsciiStream::writeLine(const char* buffer)
{
    for (int i =0;i<tabLevel; ++i) 
    {
        fwrite("\t", 1, 1, mFilePointer);
    }
    fwrite(buffer, strlen(buffer), 1, mFilePointer);
    fwrite("\n", 1, 1, mFilePointer);
}

NxuAsciiStream& NxuAsciiStream::storeBool(NX_BOOL b, const char* name)
{
    sprintf(temp,"%s: %s",name, b ? "true" : "false");
    writeLine(temp);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeByte(NxU8 b, const char* name)
{
    sprintf(temp,"%s: %d",name, b);
    writeLine(temp);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeWord(NxU16 w, const char* name)
{
    sprintf(temp,"%s: %d",name, w);
    writeLine(temp);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeDword(NxU32 d, const char* name)
{
    sprintf(temp,"%s: %d",name, d);
    writeLine(temp);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeFloat(NxReal f, const char* name)
{
    sprintf(temp,"%s: %f",name, f);
    writeLine(temp);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeDouble(NxF64 f, const char* name)
{
    sprintf(temp,"%s: %f",name, f);
    writeLine(temp);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeByteBuffer(const void* buffer, NxU32 size, const char* name)
{
    sprintf(temp, "%s:", name);
    writeLine(temp);
    ++tabLevel;
    NxU32 offset = 0;
    NxU8 * current = (NxU8*)buffer;
    while (offset < size)
    {
        int index = 0;
        int maxOnLine = size - offset;
        if (maxOnLine > 10) maxOnLine = 10;
        for (int i=0; i<maxOnLine; ++i)
        {
           index += sprintf(temp + index, "%d ", *current);
           ++current;
           ++offset;
        }
        writeLine(temp);
    }
    --tabLevel;
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeWordBuffer(const void* buffer, NxU32 size, const char* name)
{
    NX_ASSERT((size/sizeof(NxU16))*sizeof(NxU16) == size); //Must be multiple of Word size
    sprintf(temp, "%s:", name);
    writeLine(temp);
    ++tabLevel;
    NxU32 offset = 0;
    NxU16 * current = (NxU16*)buffer;
    while (offset < size)
    {
        int index = 0;
        int maxOnLine = size - offset;
        if (maxOnLine > 10) maxOnLine = 10;
        for (int i=0; i<maxOnLine; ++i)
        {
           index += sprintf(temp + index, "%d ", *current);
           ++current;
           ++offset;
        }
        writeLine(temp);
    }
    --tabLevel;
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeDwordBuffer(const void* buffer, NxU32 size, const char* name)
{
    NX_ASSERT((size/sizeof(NxU32))*sizeof(NxU32) == size); //Must be multiple of Dword size
    sprintf(temp, "%s:", name);
    writeLine(temp);
    ++tabLevel;
    NxU32 offset = 0;
    NxU32 * current = (NxU32*)buffer;
    while (offset < size)
    {
        int index = 0;
        int maxOnLine = size - offset;
        if (maxOnLine > 10) maxOnLine = 10;
        for (int i=0; i<maxOnLine; ++i)
        {
           index += sprintf(temp + index, "%d ", *current);
           ++current;
           ++offset;
        }
        writeLine(temp);
    }
    --tabLevel;
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeFloatBuffer(const void* buffer, NxU32 size, const char* name)
{
    NX_ASSERT((size/sizeof(NxF32))*sizeof(NxF32) == size); //Must be multiple of Float size
    sprintf(temp, "%s:", name);
    writeLine(temp);
    ++tabLevel;
    NxU32 offset = 0;
    NxF32 * current = (NxF32*)buffer;
    while (offset < size)
    {
        int index = 0;
        int maxOnLine = size - offset;
        if (maxOnLine > 10) maxOnLine = 10;
        for (int i=0; i<maxOnLine; ++i)
        {
           index += sprintf(temp + index, "%f ", *current);
           ++current;
           ++offset;
        }
        writeLine(temp);
    }
    --tabLevel;
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeDoubleBuffer(const void* buffer, NxU32 size, const char* name)
{
    NX_ASSERT((size/sizeof(NxF64))*sizeof(NxF64) == size); //Must be multiple of Float size
    sprintf(temp, "%s:", name);
    writeLine(temp);
    ++tabLevel;
    NxU32 offset = 0;
    NxF64 * current = (NxF64*)buffer;
    while (offset < size)
    {
        int index = 0;
        int maxOnLine = size - offset;
        if (maxOnLine > 10) maxOnLine = 10;
        for (int i=0; i<maxOnLine; ++i)
        {
           index += sprintf(temp + index, "%f ", *current);
           ++current;
           ++offset;
        }
        writeLine(temp);
    }
    --tabLevel;
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeCharBuffer(const void* buffer, NxU32 size, const char* name)
{
    sprintf(temp,"%s: %s",name, buffer);
    writeLine(temp);
    return *this;
}



void NxuAsciiStream::storeHeader(const NxuFormatHeader & header)
{
    sprintf(temp, "%s:", header.name);
    writeLine(temp);
	++tabLevel;        
}

void NxuAsciiStream::endSection()
{
    --tabLevel; 
}

NxuAsciiStream& NxuAsciiStream::storeVector(const NxVec3 &vec, const char* name)
{
    sprintf(temp, "%s: %f %f %f", name, vec.x, vec.y, vec.z);
    writeLine(temp);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeQuat(const NxQuat &quat, const char* name)
{
    sprintf(temp, "%s (XYZW): %f %f %f %f", name, quat.x, quat.y, quat.z, quat.w);
    writeLine(temp);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeMatrix(const NxMat34 &mat, const char* name)
{
	NxF32 m[9];
	mat.M.getRowMajor( m );
    char temp[1024];
    sprintf(temp,"%s (RowMajor + Trans): %f,%f,%f %f,%f,%f %f,%f,%f %f,%f,%f", name, 
        m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], mat.t.x, mat.t.y, mat.t.z );

    writeLine(temp);
    return *this;
}

NxuAsciiStream& NxuAsciiStream::storeParameter(const NxuParameterData& param, const char* name)
{
    sprintf(temp, "%s: %f", name, param.value);
    writeLine(temp);
    return *this;
}



//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND