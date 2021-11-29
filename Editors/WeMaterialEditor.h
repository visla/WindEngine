/*
        File: WeMaterialEditor.h
		Author: Tomislav Kukic
		Date: 15.12.2005.
		Version: 1.0
*/


#ifndef _WEMATERIALEDITOR
#define _WEMATERIALEDITOR


#include <WeMaterial.h>
#include <WindEngine.h>


class WINDENGINE_API WeMaterialEditor : public WeMaterial
{
public:
	WeMaterialEditor(){}
	~WeMaterialEditor();

	bool Save(StringParam HoleName, bool Replace = false);
	void Destroy();
};



#endif