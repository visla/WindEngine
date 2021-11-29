/*
    CEditorResourceManager.cpp

	Autor: Nebojsa Vislaski
	Date: 14.6.2005.
	Version: 1.0

	this class is resource manager. It loads editors.dll and frees that library when 
	requested. Object of this type should only exist in Initialize phases.


*/

#include <CEditorResourceManager.h>
#include <stdio.h>


CEditorResourceManager EditorResourceManager;


CEditorResourceManager::CEditorResourceManager()
{
	bStarted = false;
	hResource = NULL;
}

CEditorResourceManager::~CEditorResourceManager()
{
  Stop();
}


bool CEditorResourceManager::Start()
{
	if (GetStarted()) return true;

	hResource = LoadLibrary("MapEditorResource.dll");
	if (hResource == NULL) return false;

	SetStarted(true);
	return true;
}

void CEditorResourceManager::Stop()
{
  if (!GetStarted()) return;

  FreeLibrary(hResource);
  hResource = NULL;
  SetStarted(false);
}