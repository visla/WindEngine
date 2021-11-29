/*
     CMapHeader.h

	 Autor: Nebojsa Vislavski
	 Date: 20.3.2006.
	 Version: 1.0


*/


#ifndef _CMAPHEADER_H
#define _CMAPHEADER_H


#include <CString.h>
#include <WeFileStream.h>
#include <CProfile.h>
#include <CStartLocation.h>


class CMapHeader
{
public:
									CMapHeader();
	int								Version;
	String							MapDescription;
	String							MapDescriptionImage;
	String							LoadingImage;
	WeColor							AmbientColor;
	WeColor							SkyColor;
	WeColor							GroundColor;
	String							MusicFile;
	String							StatMusic;

	float							ClippingDistance;

	CSmartList<CStartLocation *>	StartLocations;

	float							BestRaceTime;
	String							BestRacePlayer;

	float							RaceTime;
	float							FogStart;
	float							FogEnd;
	bool							FogEnabled;
	WeColor							FogColor;

	float							FHDRI[3]; // dark, normal, bright

	void							Deserialize(WeFileStream &FS); // ovo ce da ucita header iz mape
	void							Serialize(WeFileStream &FS); // ovo ce da podesi header u mapu

	void							SetupRenderDevice();
};


#endif