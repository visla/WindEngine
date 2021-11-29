/*
     CTuberDescriptionManager.cpp

	 Autor: Nebojsa Vislavski
	 Date: 15.3.2006.
	 Version: 1.0

*/

#include <CTuberDescriptionManager.h>
#include <WeFileStream.h>

CTuberDescriptionManager *CTuberDescriptionManager::_Instance = NULL;

CTuberDescriptionManager *CTuberDescriptionManager::Instance()
{
	if (!_Instance)
		_Instance = new CTuberDescriptionManager;
	return _Instance;
}


CTuberDescriptionManager::CTuberDescriptionManager()
{
	LoadDescriptions();
}

void CTuberDescriptionManager::LoadDescriptions()
{
	CTuberDescription td;
	Descriptions.Clear();

    WeFileStream FS;
	if (!FS.Exists("tubers.cfg")) return;
	if (!FS.Open("tubers.cfg")) return;

	int count;
	count = FS.ReadInt();
	for (int i = 0; i < count; i++)
	{
		td.Deserialize(FS);
		Descriptions.PushBack(td);
	}

	FS.Close();
}


void CTuberDescriptionManager::AddDescription(CTuberDescription &desc)
{
	// proveri jel imamo vec tuber-a tog imena
	for (Descriptions.First(); !Descriptions.End(); Descriptions.Next())
	{
		if (Descriptions.GetItem().Name == desc.Name)
		{
			//SAFE_DELETE(Descriptions.GetItem());
			Descriptions.Delete();
			break;
		}
	}

	Descriptions.PushBack(desc);
}


void CTuberDescriptionManager::RemoveDescription(CTuberDescription &desc)
{
	for (Descriptions.First(); !Descriptions.End(); Descriptions.Next())
	{
		if (Descriptions.GetItem().Name == desc.Name)
		{
			Descriptions.Delete();
			break;
		}
	}	
	SaveDescriptions();
}

void CTuberDescriptionManager::SaveDescriptions()
{
    WeFileStream FS;
	if (!FS.OpenNew("tubers.cfg")) return;

	FS.WriteInt(Descriptions.GetItemsCount());
	for (Descriptions.First(); !Descriptions.End(); Descriptions.Next())
	{
		Descriptions.GetItem().Serialize(FS);        
	}
	FS.Close();
}