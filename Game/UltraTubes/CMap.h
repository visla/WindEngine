/*

      File: CMap.h
	  Author: Tomislav Kukic
	  Date: 25.11.2005.
	  Version: 1.0
*/




#ifndef _CMAP_H
#define _CMAP_H

#include <WeWorld.h>
#include <SmartList.h>
#include <CMapHeader.h>


class CPickUp;


class CMap : public WeWorld
{
protected:

public:			
	CMap();
	~CMap();

	CMapHeader	MapHeader;

	CSmartList<CPickUp*>	Pickups;

	float		FastesLap;
	float		FastestTuberTime[1000];

	WeActor*	OnLoadMapItem(int Type, WeFileStream &FS);	// this is called when maps needs to load some unknown item

	void		OnLoadMapHeader(WeFileStream &FS);				// This will load map header
	void		OnSaveMapHeader(WeFileStream &FS);				// This will save map header

};



#endif