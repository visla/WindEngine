/*
	Filename:	WeSoundAttenuator.h
	Author:		Goran Bogdanovic
*/


#ifndef _WESOUNDATTENUATOR_H
#define _WESOUNDATTENUATOR_H


#include <CString.h>


#define DEFAULT_ATTENUATOR_FILE_NAME	"sbalance.txt"


class WeSound;

class WeSoundAttenuationDesc
{
public:
	WeSoundAttenuationDesc();
	~WeSoundAttenuationDesc();

	String									SoundName;
	float									VolumeMultiplier;

	void									Deserialize(FILE *f);

};


class WINDENGINE_API WeSoundAttenuator
{
protected:
	String									SourceFile;

	bool									Initialized;
	CSmartList<WeSoundAttenuationDesc*>		SoundDescriptors;

public:
	WeSoundAttenuator();
	~WeSoundAttenuator();

	static WeSoundAttenuator				*Instance;

	bool									IsInitialized();

	bool									Initialize(String AttenuatorFileName);

	void									ProcessSound(WeSound *VibrationThingy);

	String									GetSourceFile();

	void									Clean();
	void									Refresh();

};


#endif