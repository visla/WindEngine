/*
      File: WeAudioDevice.h
	  Author: Tomislav Kukic
	  Date: 15.09.2005.
	  Version: 1.0
*/



#ifndef _WEAUDIODEVICE
#define _WEAUDIODEVICE


#include <SmartList.h>
#include <CString.h>
#include <Windows.h>
#include <mmreg.h>
#include <dsound.h>
#include <WindEngine.h>


class WeSound;
class WeEngine;
class WeMusicPlayer;
class WeListener;


class WINDENGINE_API WeAudioDevice
{
	friend class WeEngine;
	friend class WeMusicPlayer;
	friend class WeSound;

protected:
	static WeAudioDevice*		_instance;

								WeAudioDevice(void);
								~WeAudioDevice(void);

	CSmartList<WeSound*>		Sounds;
	CSmartList<WeSound*>		DyingSounds;
	CSmartList<WeSound*>		Clones;

	IDirectSound8*				pDS;
	WeMusicPlayer*				MusicPlayer;

	bool						Initialized;
	float						SoundVolume;
	float						MusicVolume;
	bool						AutoLoadFromPackage;

	WeSound*					Clone(WeSound* SourceSound = NULL);
	bool						CreateListener();
	void						CheckForDyingSounds();

	String						CurrentSongName;


public:

	static WeAudioDevice*		Instance();

	WeListener*					Listener;

	bool						Initialize(HWND hWnd);
	void						ShutDown();
	void						Update();

	bool						SetPrimaryBufferFormat(	DWORD dwPrimaryChannels,
														DWORD dwPrimaryFreq,
														DWORD dwPrimaryBitRate );

	//LRESULT					MsgHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	WeSound*					LoadSound(StringParam name, bool b3DSound, bool FromPackage = false, bool ForceReload = false, DWORD Flags = 0);
	WeSound*					LoadSoundAuto(String name, bool b3DSound = true);
	bool						FreeSound(WeSound* Sound);

	void						PlayMusic(StringParam MusicName);
	void						StopMusic();

	void						SetMusicVolume(float Volume);
	void						SetSoundVolume(float Volume);

	void						SetAutoLoadFromPackage(bool Value);
	bool						GetAutoLoadFromPackage();

	float						GetSoundVolume()	{ return SoundVolume; }
	float						GetMusicVolume()	{ return MusicVolume; }

	void						ClearClones();

	static long					GetGain(float Gain);
	static float				GetAmp(long Amp);

	String						GetSongName() { return CurrentSongName; }

};



#endif
