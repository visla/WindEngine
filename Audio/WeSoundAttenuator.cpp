/*
	Filename:	WeSoundAttenuator.cpp
	Author:		Goran Bogdanovic
*/


#include <WeSoundAttenuator.h>
#include <WeSound.h>


WeSoundAttenuator *WeSoundAttenuator::Instance = new WeSoundAttenuator();


//---WeSoundAttenuationDesc---

WeSoundAttenuationDesc::WeSoundAttenuationDesc()
{
}


WeSoundAttenuationDesc::~WeSoundAttenuationDesc()
{
}

void WeSoundAttenuationDesc::Deserialize(FILE *f)
{
	char temp[100];

	fscanf(f, "%f %s\n", &VolumeMultiplier, temp);
	SoundName = String(temp);
}



//---WeSoundAttenuator---


WeSoundAttenuator::WeSoundAttenuator()
{
	Initialized = Initialize(DEFAULT_ATTENUATOR_FILE_NAME);
}


WeSoundAttenuator::~WeSoundAttenuator()
{
	Clean();
}


bool WeSoundAttenuator::Initialize(String AttenuatorFileName)
{
	FILE *f;
	f = fopen(AttenuatorFileName, "rt+");
	if (!f) return false;

	while (!feof(f))
	{
		WeSoundAttenuationDesc *a = new WeSoundAttenuationDesc();
		a->Deserialize(f);
		SoundDescriptors.PushBack(a);
	}

	fclose(f);

	SourceFile = AttenuatorFileName;

	return true;
}

void WeSoundAttenuator::ProcessSound(WeSound *VibrationThingy)
{
	if (!Initialized) return;
	if (!VibrationThingy) return;

	for (SoundDescriptors.First(); !SoundDescriptors.End(); SoundDescriptors.Next())
	{
		if (SoundDescriptors.GetItem()->SoundName == VibrationThingy->Name)
			VibrationThingy->SetAttenuationFactor(SoundDescriptors.GetItem()->VolumeMultiplier);
	}
}

void WeSoundAttenuator::Clean()
{
	for (SoundDescriptors.First(); !SoundDescriptors.End(); )
	{
		delete SoundDescriptors.GetItem();
		SoundDescriptors.Delete();
	}
}

bool WeSoundAttenuator::IsInitialized()
{
	return Initialized;
}

void WeSoundAttenuator::Refresh()
{
	Clean();
	if (Initialized)
		Initialized = Initialize(SourceFile);
}
