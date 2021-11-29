/*

       File: WeMeshEditor.h
	   Author: Tomislav Kukic
	   Date: 09.11.2005.
	   Version: 1.0
*/


#ifndef _WEMESHEDITOR
#define _WEMESHEDITOR



#include <WeMesh.h>
#include <WindEngine.h>




class WINDENGINE_API WeMeshEditor : public WeMesh
{
public:
	WeMeshEditor();
	virtual ~WeMeshEditor();

	bool Save(StringParam HoleName, bool Replace = false);
	virtual void Destroy();
};




#endif
