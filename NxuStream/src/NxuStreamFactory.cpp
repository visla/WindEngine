/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		NxuStreamFactory.h

		This factory class creates stream objects for exporting to known file formats.

		Changelist
		----------

*/

#include "NxuStreamFactory.h"
#include "NxuBinaryStream.h"
#include "NxuAsciiStream.h"
#include "NxuColladaStream.h"

static char littleEndian()	{ int i = 1; return *((char*)&i);	}

static NxU32 getSDKVersion()
{
    return NXU_SDK_FEATURES;
}

NxuInputStream* NxuStreamFactory::getInputStream(NxuStreamFactory::FormatType format, FILE* file)
{
//
    NxuInputStream* stream = NULL;
    if (format == NxuStreamFactory::BINARY || format == NxuStreamFactory::BINARY_LITTLE_END || format == NxuStreamFactory::BINARY_BIG_END) 
    {
        NxuBinaryStream* s = new NxuBinaryStream();
		if (!s->setFilePointer(file, false)) {
			releaseStream((NxuInputStream*)s);
			return NULL;
		}
        if (s->mismatch)
        {
            if (littleEndian())
            {
                s->mFormat = NxuStreamFactory::BINARY_BIG_END;
            }
            else
            {
                s->mFormat = NxuStreamFactory::BINARY_LITTLE_END;
            }
        }
        else
        {
            if (littleEndian())
            {
                s->mFormat = NxuStreamFactory::BINARY_LITTLE_END;
            }
            else
            {
                s->mFormat = NxuStreamFactory::BINARY_BIG_END;
            }
        }
        stream = s;
    }
    else if (format == NxuStreamFactory::ASCII)
    {
        NxuAsciiStream* s = new NxuAsciiStream();
        s->mFormat = format;
		if (!s->setFilePointer(file, false)) {
			releaseStream((NxuInputStream*)s);
			return NULL;
		}
        stream = s;
    }
    else if (format == NxuStreamFactory::COLLADA)
    {
        NxuColladaStream* s = new NxuColladaStream();
        s->mFormat = format;
		if (!s->setFilePointer(file, false)) {
			releaseStream((NxuInputStream*)s);
			return NULL;
		}
        stream = s;
    }
    return stream;
}

NxuOutputStream* NxuStreamFactory::getOutputStream(NxuStreamFactory::FormatType format, FILE* file)
{
    NxuOutputStream* stream = NULL;
    if (format == NxuStreamFactory::BINARY || format == NxuStreamFactory::BINARY_LITTLE_END || format == NxuStreamFactory::BINARY_BIG_END) 
    {
        NxuBinaryStream* s = new NxuBinaryStream();
        if (littleEndian() && format == NxuStreamFactory::BINARY_BIG_END
            || !littleEndian() && format == NxuStreamFactory::BINARY_LITTLE_END)
        {
            s->mismatch = true;
        }
        if (format == NxuStreamFactory::BINARY)
        {
            if (littleEndian())
            {
                s->mFormat = NxuStreamFactory::BINARY_LITTLE_END;
            }
            else
            {
                s->mFormat = NxuStreamFactory::BINARY_BIG_END;
            }
        }
        else
        {
            s->mFormat = format;
        }
        s->setSDKVersion(getSDKVersion());
		if (!s->setFilePointer(file, true)) {
			releaseStream((NxuOutputStream*)s);
			return NULL;
		}
        stream = s;
    }
    else if (format == NxuStreamFactory::ASCII)
    {
        NxuAsciiStream* s = new NxuAsciiStream();
        s->mFormat = format;
        s->setSDKVersion(getSDKVersion());
		if (!s->setFilePointer(file, true)) {
			releaseStream((NxuOutputStream*)s);
			return NULL;
		}
        stream = s;
    }
    else if (format == NxuStreamFactory::COLLADA)
    {
        NxuColladaStream* s = new NxuColladaStream();
        s->mFormat = format;
        s->setSDKVersion(getSDKVersion());
		if (!s->setFilePointer(file, true)) {
			releaseStream((NxuOutputStream*)s);
			return NULL;
		}
        stream = s;
    }
    return stream;
}

NxuInputStream* NxuStreamFactory::getInputStream(NxuStreamFactory::FormatType format, const char* filename)
{
    NxuInputStream* stream = NULL;
    if (format == NxuStreamFactory::BINARY || format == NxuStreamFactory::BINARY_LITTLE_END || format == NxuStreamFactory::BINARY_BIG_END) 
    {
        NxuBinaryStream* s = new NxuBinaryStream();
		if (!s->openFile(filename, false)) {
			releaseStream((NxuInputStream*)s);
			return NULL;
		}
        if (s->mismatch)
        {
            if (littleEndian())
            {
                s->mFormat = NxuStreamFactory::BINARY_BIG_END;
            }
            else
            {
                s->mFormat = NxuStreamFactory::BINARY_LITTLE_END;
            }
        }
        else
        {
            if (littleEndian())
            {
                s->mFormat = NxuStreamFactory::BINARY_LITTLE_END;
            }
            else
            {
                s->mFormat = NxuStreamFactory::BINARY_BIG_END;
            }
        }
        stream = s;
    }
    else if (format == NxuStreamFactory::ASCII)
    {
        NxuAsciiStream* s = new NxuAsciiStream();
        s->mFormat = format;
		if (!s->openFile(filename, false)) {
			releaseStream((NxuInputStream*)s);
			return NULL;
		}
        stream = s;
    }
    else if (format == NxuStreamFactory::COLLADA)
    {
        NxuColladaStream* s = new NxuColladaStream();
        s->mFormat = format;
		if (!s->openFile(filename, false)) {
			releaseStream((NxuInputStream*)s);
			return NULL;
		}
        stream = s;
    }
    return stream;
}

NxuOutputStream* NxuStreamFactory::getOutputStream(NxuStreamFactory::FormatType format, const char* filename)
{
    NxuOutputStream* stream = NULL;
	if (format == NxuStreamFactory::BINARY || format == NxuStreamFactory::BINARY_LITTLE_END || format == NxuStreamFactory::BINARY_BIG_END) 
    {
        NxuBinaryStream* s = new NxuBinaryStream();
        s->mismatch = false;
        if (littleEndian() && format == NxuStreamFactory::BINARY_BIG_END
            || !littleEndian() && format == NxuStreamFactory::BINARY_LITTLE_END)
        {
            s->mismatch = true;
        }
        if (format == NxuStreamFactory::BINARY)
        {
            if (littleEndian())
            {
                s->mFormat = NxuStreamFactory::BINARY_LITTLE_END;
            }
            else
            {
                s->mFormat = NxuStreamFactory::BINARY_BIG_END;
            }
        }
        else
        {
            s->mFormat = format;
        }
        s->setSDKVersion(getSDKVersion());
		if (!s->openFile(filename, true)) {
			releaseStream((NxuOutputStream*)s);
			return NULL;
		}
        stream = s;
    }
    else if (format == NxuStreamFactory::ASCII)
    {
        NxuAsciiStream* s = new NxuAsciiStream();
        s->mFormat = format;
		if (!s->openFile(filename, true)) {
			releaseStream((NxuOutputStream*)s);
			return NULL;
		}
        stream = s;
    }
    else if (format == NxuStreamFactory::COLLADA)
    {
        NxuColladaStream* s = new NxuColladaStream();
        s->mFormat = format;
        s->setSDKVersion(getSDKVersion());
		if (!s->openFile(filename, true)) {
			releaseStream((NxuOutputStream*)s);
			return NULL;
		}
        stream = s;
    }
    return stream;
}

void NxuStreamFactory::releaseStream(NxuInputStream* stream)
{
    if (stream) delete stream;
}

void NxuStreamFactory::releaseStream(NxuOutputStream* stream)
{
    if (stream) delete stream;
}


//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND
