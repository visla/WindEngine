/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		CoreDumpStream.cpp

		Implementation of saving physics data to a binary stream.

		Changelist
		----------

*/


#include "NxuBinaryStream.h"


static char littleEndian()	{ int i = 1; return *((char*)&i);	}

NxuBinaryStream::NxuBinaryStream()
{
    mFilePointer = 0;
    mFileName[0] = 0;
    mIsOutputStream = false;
    currentSize =0;
    mismatch = 0;
}


//Returns true if the file was successfully opened
NX_BOOL NxuBinaryStream::openFile(const char* filename, NX_BOOL writeable)
{
    NX_ASSERT(strlen(filename) < 1024);
    strncpy(mFileName, filename, 1024);
    currentSize = 0;
    return setFilePointer(fopen(mFileName, writeable ? "wb" : "rb"), writeable);
}

void NxuBinaryStream::closeFile()
{
    if (mFilePointer && mFileName[0] != 0) fclose(mFilePointer);
    mFilePointer = 0;
    mFileName[0] = 0;
    currentSize = 0;
}

//Returns true if the file pointer is valid
NX_BOOL NxuBinaryStream::setFilePointer(FILE* file, NX_BOOL writeable)
{
    if (mFilePointer) closeFile(); //Close any previously opened file
    mFilePointer = file;
    if (!file) return false;
    mIsOutputStream = writeable;
    if (!writeable) 
    {
        NxU8 endian;
        readByte(endian, "littleEndian");
        mismatch = endian == littleEndian() ? false : true;
        readDword(sdkVersion, "sdkVersion");
    } 
    else 
    {
        storeByte(mismatch ? littleEndian()^1 : littleEndian());
        storeDword(sdkVersion, "sdkVersion");
    }
    return true;
}

NxuBinaryStream::~NxuBinaryStream()
{
    closeFile();
}

NxuBinaryStream& NxuBinaryStream::readBool(NX_BOOL& b, const char* name)								
{
    NxU8 byte;
    readByte(byte);
    b = byte ? true : false;
    return *this;
}

NxuBinaryStream& NxuBinaryStream::readByte(NxU8& b, const char* name)								
{
    NX_ASSERT(!mIsOutputStream);
    readByteBuffer(&b, 1);
    return *this;
}

NxuBinaryStream& NxuBinaryStream::readWord(NxU16& w, const char* name)								
{
    NxU8* p = (NxU8*)&w;
    if (mismatch)
    {
        readByte(*(p+1));
        readByte(*p);
    } 
    else
    {
        readByteBuffer(p, sizeof(w));
    }

    return *this;
}

NxuBinaryStream& NxuBinaryStream::readDword(NxU32& d, const char* name)								
{
    NxU16* p = (NxU16*)&d;
    if (mismatch)
    {
        readWord(*(p+1));
        readWord(*p);
    } 
    else
    {
        readByteBuffer(p, sizeof(d));
    }   
    return *this;
}

NxuBinaryStream& NxuBinaryStream::readFloat(NxF32& f, const char* name)								
{

    NxU32* p = (NxU32*)&f;
    if (mismatch)
    {
        readDword(*p);
    }
    else
    {
        readByteBuffer(&f, sizeof(f));
    }
    return *this;
}

NxuBinaryStream& NxuBinaryStream::readDouble(NxF64& d, const char* name)							
{
    NxU32* p = (NxU32*)&d;
    if (mismatch)
    {
        readDword(*(p+1));
        readDword(*p);
    }
    else
    {
        readByteBuffer(&d, sizeof(d));
    }
    return *this;
}

NxuBinaryStream&  NxuBinaryStream::readByteBuffer(void* buffer, NxU32 size, const char* name)	
{
    NX_ASSERT(!mIsOutputStream);
    NxuInputStream::errorCode = NXU_NO_ERROR;
    fread(buffer, size, 1, mFilePointer);
    if (feof(mFilePointer) != 0) 
    {
        NxuInputStream::errorCode = NXU_END_OF_FILE;
    }
    if (ferror(mFilePointer) != 0) 
    {
        NxuInputStream::errorCode = NXU_IOERROR;
    }

    currentSize += size;
    return *this;
}

NxuBinaryStream&  NxuBinaryStream::readWordBuffer(void* buffer, NxU32 size, const char* name)	
{
    if (mismatch)
    {
        NxU16* bufPtr = (NxU16*)buffer;
        for (unsigned int i=0;i<size/sizeof(NxU16);++size)
        {
            readWord(*bufPtr);
            bufPtr++;
        }
    }
    else
    {
        readByteBuffer(buffer, size);
    }
    return *this;

}

NxuBinaryStream&  NxuBinaryStream::readDwordBuffer(void* buffer, NxU32 size, const char* name)	
{
    if (mismatch)
    {
        NxU32* bufPtr = (NxU32*)buffer;
        for (unsigned int i=0;i<size/sizeof(NxU32);++i)
        {
            readDword(*bufPtr);
            bufPtr++;
        }
    }
    else
    {
        readByteBuffer(buffer, size);
    }
    return *this;

}

NxuBinaryStream&  NxuBinaryStream::readFloatBuffer(void* buffer, NxU32 size, const char* name)	
{
    if (mismatch)
    {
        NxF32* bufPtr = (NxF32*)buffer;
        for (unsigned int i=0;i<size/sizeof(NxF32);++i)
        {
            readFloat(*bufPtr);
            bufPtr++;
        }
    }
    else
    {
        readByteBuffer(buffer, size);
    }
    return *this;

}

NxuBinaryStream&  NxuBinaryStream::readDoubleBuffer(void* buffer, NxU32 size, const char* name)	
{
    if (mismatch)
    {
        NxF64* bufPtr = (NxF64*)buffer;
        for (unsigned int i=0;i<size/sizeof(NxF64);++i)
        {
            readDouble(*bufPtr);
            bufPtr++;
        }
    }
    else
    {
        readByteBuffer(buffer, size);
    }
    return *this;

}

NxuBinaryStream&	NxuBinaryStream::readCharBuffer(void* buffer, NxU32 size, const char* name)
{
    readByteBuffer(buffer, size);
    return *this;
}

void NxuBinaryStream::readHeader(NxuFormatHeader &header)
{       
	readByte(header.sectionType);                     
    readDword(header.size);                           
	header.name[0] = 0;
    currentSize = header.size;
}

void NxuBinaryStream::skip()
{
    NX_ASSERT(!mIsOutputStream);
    NX_ASSERT(!currentSize);
    fseek(mFilePointer, currentSize, SEEK_CUR);
    currentSize = 0;
}

NxuBinaryStream&  NxuBinaryStream::readVector(NxVec3 &vec, const char* name)
{
    readFloat(vec.x);
    readFloat(vec.y);
    readFloat(vec.z);
    return *this;
}

NxuBinaryStream&  NxuBinaryStream::readQuat(NxQuat &quat, const char* name)
{
    readFloat(quat.x);
    readFloat(quat.y);
    readFloat(quat.z);
    readFloat(quat.w);
    return *this;
}

NxuBinaryStream&  NxuBinaryStream::readMatrix(NxMat34 &mat, const char* name)
{
	NxF32 m[9];
    readFloatBuffer(m, 9*sizeof(NxF32));
	mat.M.setRowMajor( m	);

    readVector(mat.t);
    return *this;
}

NxuBinaryStream&  NxuBinaryStream::readParameter(NxuParameterData& param, const char* name)
{
    NxU8 parameter;
    readByte(parameter);
    param.param = (NxParameter)parameter;
    readFloat(param.value);
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeBool(NX_BOOL b, const char* name)
{
    NX_ASSERT(mIsOutputStream);
    storeByte(b ? NxU8(1) : NxU8(0));
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeByte(NxU8 b, const char* name)
{
    NX_ASSERT(mIsOutputStream);
    storeByteBuffer(&b,1);
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeWord(NxU16 w, const char* name)
{
    if (mismatch)
    {
        NxU8 *a = (NxU8*)&w;
        NxU8 *b = a + 1;
        storeByte(*b, "");
        storeByte(*a, "");
    }
    else 
    {
        storeByteBuffer(&w, sizeof(w));
    }
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeDword(NxU32 d, const char* name)
{
    if (mismatch)
    {
        NxU16 *a = (NxU16*)&d;
        NxU16 *b = a + 1;
        storeWord(*b, "");
        storeWord(*a, "");
    }
    else 
    {
        storeByteBuffer(&d, sizeof(d));
    }
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeFloat(NxF32 f, const char* name)
{
    storeDword(*((NxU32*)&f), "");
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeDouble(NxF64 f, const char* name)
{
    NxU32* p = (NxU32*)&f;
    if (mismatch)
    {
        storeDword(*(p+1), "");
        storeDword(*p, "");
    }
    else
    {
        storeByteBuffer(&f, sizeof(f));
    }
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeByteBuffer(const void* buffer, NxU32 size, const char* name)
{
    NX_ASSERT(mIsOutputStream);
    NxuOutputStream::errorCode = NXU_NO_ERROR;
    fwrite(buffer, size, 1, mFilePointer);
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeWordBuffer(const void* buffer, NxU32 size, const char* name)
{
    NX_ASSERT((size/sizeof(NxU16))*sizeof(NxU16) == size); //Must be multiple of Word size
    if (mismatch)
    {
        NxU16* bufPtr = (NxU16*)buffer;
        for (unsigned int i=0;i<size/sizeof(NxU16);++i)
        {
            storeWord(bufPtr[i], "");
        }
    }
    else
    {
        storeByteBuffer(buffer, size, name);
    }
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeDwordBuffer(const void* buffer, NxU32 size, const char* name)
{
    NX_ASSERT((size/sizeof(NxU32))*sizeof(NxU32) == size); //Must be multiple of Dword size
    if (mismatch)
    {
        NxU32* bufPtr = (NxU32*)buffer;
        for (unsigned int i=0;i<size/sizeof(NxU32);++i)
        {
            storeDword(bufPtr[i], "");
        }
    }
    else
    {
        storeByteBuffer(buffer, size, name);
    }
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeFloatBuffer(const void* buffer, NxU32 size, const char* name)
{
    NX_ASSERT((size/sizeof(NxF32))*sizeof(NxF32) == size); //Must be multiple of Float size
    if (mismatch)
    {
        NxF32* bufPtr = (NxF32*)buffer;
        for (unsigned int i=0;i<size/sizeof(NxF32);++i)
        {
            storeFloat(bufPtr[i], "");
        }
    }
    else
    {
        storeByteBuffer(buffer, size, name);
    }
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeDoubleBuffer(const void* buffer, NxU32 size, const char* name)
{
    NX_ASSERT((size/sizeof(NxF64))*sizeof(NxF64) == size); //Must be multiple of Float size
    if (mismatch)
    {
        NxF64* bufPtr = (NxF64*)buffer;
        for (unsigned int i=0;i<size/sizeof(NxF64);++i)
        {
            storeDouble(bufPtr[i], "");
        }
    }
    else
    {
        storeByteBuffer(buffer, size, name);
    }
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeCharBuffer(const void* buffer, NxU32 size, const char* name)
{
    storeByteBuffer(buffer, size, name);
    return *this;
}

void NxuBinaryStream::storeHeader(const NxuFormatHeader & header)
{             
	storeByte(header.sectionType, "");                     
    storeDword(header.size, "");          
}

void NxuBinaryStream::endSection()
{
 return;  
}

NxuBinaryStream& NxuBinaryStream::storeVector(const NxVec3 &vec, const char* name)
{
    storeFloat(vec.x);
    storeFloat(vec.y);
    storeFloat(vec.z);
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeQuat(const NxQuat &quat, const char* name)
{
    storeFloat(quat.x);
    storeFloat(quat.y);
    storeFloat(quat.z);
    storeFloat(quat.w);
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeMatrix(const NxMat34 &mat, const char* name)
{
	NxF32 m[9];
	mat.M.getRowMajor( m );
    storeFloatBuffer(m, 9*sizeof(NxF32));

    storeVector(mat.t);
    return *this;
}

NxuBinaryStream& NxuBinaryStream::storeParameter(const NxuParameterData& param, const char* name)
{
    storeByte(NxU8(param.param));
    storeFloat(param.value);
    return *this;
}



//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND