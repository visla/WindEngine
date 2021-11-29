/*
     File: WeSoundEditor.h
	 Author: Tomislav Kukic
	 Date: 10.01.2006.
	 Version: 1.0
*/




#ifndef WESOUNDEDITOR
#define WESOUNDEDITOR


#include <WeSound.h>
#include <CString.h>
#include <WindEngine.h>


class WINDENGINE_API WeSoundEditor : public WeSound
{
public:
	WeSoundEditor();

	bool Save(StringParam HoleName, bool Replace = false);
};



#endif
