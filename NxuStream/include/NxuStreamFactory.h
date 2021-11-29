/*----------------------------------------------------------------------
		Copyright (c) 2005 AGEIA Technologies, Inc.

		NxuStreamFactory.h

		This factory class creates stream objects for exporting to known file formats.

		Changelist
		----------

*/
#ifndef EX_STREAM_FACTORY_H
#define EX_STREAM_FACTORY_H

#include <stdio.h>

#include "NxuStream.h"

/**
\brief Factory for generating standard stream types.

This class can produce a stream for writing to a file that is useable for import or export.

@see NxuPhysicsExport NxuPhysicsImport

*/
class NxuStreamFactory
{
protected:
    NxuStreamFactory() {} //Static class, should not be instantiated
    ~NxuStreamFactory() {} 

public:
/**
\brief Enumerated list of supported file formats.
BINARY -- binary format using endian byte ordering of local OS
BINARY_LITTLE_END -- binary format using little-endian byte ordering for efficient loading on Win32
BINARY_BIG_END - binary format using big-endian byte ordering for efficient loading on XENON/Mac/*nix etc.
ASCII - text format for human readable output that mirrors the stucture of the binary formats
COLLADA - XML format based on COLLADA digital asset schema
*/
    enum FormatType
    {
        BINARY, //Will use default endianness of operating system
        BINARY_LITTLE_END, //Force writing in little endian, same as BINARY for InputStream
        BINARY_BIG_END,    //Force writing in big endian, same as BINARY for InputStream
        ASCII,
        COLLADA
    };

    /**
    \brief Generate an input stream for reading from the specified file.

    \param[in] format The format of the file.
    \param[in] filename The name of the file to open, including relative or absolute path.

    \return A stream able to read the file or a NULL pointer if the file couldn't be opened.
    */
    static NxuInputStream* getInputStream(FormatType format, const char* filename);
    /**
    \brief Generate an output stream for writing to the specified file.

    \param[in] format The format of the file.
    \param[in] filename The name of the file to open, including relative or absolute path.

    \return A stream able to write the file or a NULL pointer if the file couldn't be opened.
    */
    static NxuOutputStream* getOutputStream(FormatType format, const char* filename);

    /**
    \brief Generate an input stream for reading from the specified file pointer.

    \param[in] format The format of the file.
    \param[in] file A file pointer to an open file for reading.

    \return A stream able to read the file or a NULL pointer if the pointer wasn't usable.
    */
    static NxuInputStream* getInputStream(FormatType format, FILE* file);

    /**
    \brief Generate an input stream for writing to the specified file pointer.

    \param[in] format The format of the file.
    \param[in] file A file pointer to an open file for writing.

    \return A stream able to write the file or a NULL pointer if the pointer wasn't usable.
    */
    static NxuOutputStream* getOutputStream(FormatType format, FILE* file);

    /**
    \brief Release an input stream created by this factory.

    \param[in] inputStream The stream created by a call to getInputStream().
    */
    static void releaseStream(NxuInputStream* inputStream);

    /**
    \brief Release an output stream created by this factory.

    \param[in] outputStream The stream created by a call to getOutputStream().
    */
    static void releaseStream(NxuOutputStream* outputStream);

};


#endif //EX_STREAM_FACTORY_H


//AGCOPYRIGHTBEGIN
///////////////////////////////////////////////////////////////////////////
// Copyright © 2005 AGEIA Technologies.
// All rights reserved. www.ageia.com
///////////////////////////////////////////////////////////////////////////
//AGCOPYRIGHTEND
