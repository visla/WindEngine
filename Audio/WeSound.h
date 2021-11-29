/*
     File: WeSound.h
	 Author: Tomislav Kukic
	 Date: 15.09.2005.
	 Version: 1.0
*/




#ifndef _WeSOUND
#define _WeSOUND

#include <Windows.h>
#include <mmreg.h>
#include <dsound.h>
#include <CString.h>
#include <WeVector.h>
#include <WindEngine.h>



class WeAudioDevice;


class WINDENGINE_API WeSound
{
	friend class WeAudioDevice;

protected:
	IDirectSound8* DSound;
	IDirectSoundBuffer* Buffer;
	IDirectSoundBuffer8* SecBuffer;
	IDirectSound3DBuffer8* Buffer3D;

	DWORD SoundMode;
	bool Playing;
	bool isLoaded;

	WAVEFORMATEX WaveFormat;
    bool BufferCreated;

	bool CreateBuffer();
	bool CreateSecBuffer();

	float AttenuationFactor;
	float Volume;

public:
	WeSound(){}
	WeSound(IDirectSound8*);
	virtual ~WeSound();

	String Name;
	bool Is3DSound;

	DWORD CreationFlags;

	DWORD GetFrequency();
	void  SetFrequency(DWORD _Freq);
	bool Load(StringParam HoleName, bool FromPackage = false);
	bool Load3D(StringParam HoleName, bool FromPackage = false);
	bool Save(StringParam HoleName, bool Replace = false);
	bool IsLoaded();
	bool Save();
	void Init3D();
	void Destroy();
	void Play(bool Looped, bool Reset = false);
	bool IsPlaying();
	void Stop();
	void Reset();
	void SetMaxDistance(float Dist);
	void Update3DParams(WeVector &Position, WeVector &Velocity, WeVector &ConeOrientation, DWORD ConeInsideAngle, DWORD ConeOutsideAngle);
	void Update(WeVector &Position, WeVector &Velocity);
	void Free();
	void DieOnStop();
	void InitStaticFX();

	void		SetAttenuationFactor(float Factor);
	float		GetAttenuationFactor();

	void		SetVolume(float NewVolume);
	float		GetVolume();


	LPVOID		MemoryBuffer;
	DWORD		MemorySize;	
};



#endif