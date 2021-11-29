/*
       File: WeTextureEditor.h
	   Author: Tomislav Kukic
	   Date: 13.09.2005.
	   Version: 1.0
*/




#ifndef _WETEXTUREEDITOR
#define _WETEXTUREEDITOR



#include <CString.h>
#include <WeTexture.h>
#include <WindEngine.h>


class WINDENGINE_API WeTextureEditor :	public WeTexture
{
public:
	WeTextureEditor(void);
	~WeTextureEditor(void);

	bool Save(StringParam SaveName, bool Replace);
	virtual void Destroy();

};


#endif
