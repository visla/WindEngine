/*
    CEditorResourceManager.h

	Autor: Nebojsa Vislaski
	Date: 14.6.2005.
	Version: 1.0

	this class is resource manager. It loads editors.dll and frees that library when 
	requested. Object of this type should only exist in Initialize phases.

	User should use this ResourceManager declared here and not create some others


*/


#ifndef _EDITORRESOURCEMANAGER
#define _EDITORRESOURCEMANAGER



#include <windows.h>
#include "..\\..\\Editors\\MapEditorResource\\resource.h"


extern class CEditorResourceManager EditorResourceManager;


class CEditorResourceManager
{
protected:
   bool bStarted;
   HMODULE hResource;
	 
   void SetStarted(bool Started) { bStarted = Started; }

public:
   CEditorResourceManager();
   ~CEditorResourceManager();

   bool Start();
   void Stop();

   bool GetStarted() { return bStarted; }
   HMODULE GetResourceInstance() { return hResource; }

};



#endif
