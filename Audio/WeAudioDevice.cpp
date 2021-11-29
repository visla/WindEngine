/*
      File: WeAudioDevice.cpp
	  Author: Tomislav Kukic
	  Date: 15.09.2005.
	  Version: 1.0
*/


#include <d3d9.h>
#include <WeSound.h>
#include <WeAudioDevice.h>
#include <WeMusicPlayer.h>
#include <WeTimer.h>
#include <WeListener.h>


WeAudioDevice* WeAudioDevice::_instance = NULL;


WeAudioDevice::WeAudioDevice(void)
{
	Initialized = false;

	pDS = NULL;
	Listener = NULL;
	MusicPlayer = NULL;

	MusicVolume = 1.0f;
	SoundVolume = 1.0f;

	Sounds.Clear();
	DyingSounds.Clear();
	Clones.Clear();

	AutoLoadFromPackage = true;

	CurrentSongName = (String)"";
}


WeAudioDevice::~WeAudioDevice(void)
{
}


WeAudioDevice* WeAudioDevice::Instance()
{
	if (_instance) return _instance;
	_instance = new WeAudioDevice();
	return _instance;
}


//LRESULT WeAudioDevice::MsgHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//{
//	LRESULT tmp;
//	return tmp;
//}


bool WeAudioDevice::Initialize(HWND hWnd)
{
	if (Initialized) return true;

	if(D3D_OK != DirectSoundCreate8(NULL, &pDS, NULL))
		return false;

	if(D3D_OK != pDS->SetCooperativeLevel(hWnd, DSSCL_PRIORITY))
		return false;

	//SetPrimaryBufferFormat(2, 44100, 16);

	if (!CreateListener())
		return false;

	MusicPlayer = new WeMusicPlayer();

	Initialized = true;
	return true;
}


bool WeAudioDevice::SetPrimaryBufferFormat(	DWORD dwPrimaryChannels,
											DWORD dwPrimaryFreq,
											DWORD dwPrimaryBitRate )
{
    HRESULT             hr;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

	if( pDS == NULL )
        return false;

    // Get the primary buffer
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;

    if( FAILED( hr = pDS->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL ) ) )
        return false;

    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) );
    wfx.wFormatTag      = (WORD) WAVE_FORMAT_PCM;
    wfx.nChannels       = (WORD) dwPrimaryChannels;
    wfx.nSamplesPerSec  = (DWORD) dwPrimaryFreq;
    wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate;
    wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
    wfx.nAvgBytesPerSec = (DWORD) (wfx.nSamplesPerSec * wfx.nBlockAlign);

    if( FAILED( hr = pDSBPrimary->SetFormat(&wfx) ) )
        return false;

    SAFE_RELEASE( pDSBPrimary );

    return true;
}


void WeAudioDevice::ShutDown()
{
	if (MusicPlayer)
		MusicPlayer->Stop();

	SAFE_DELETE(Listener);
	SAFE_DELETE(MusicPlayer);
	SAFE_RELEASE(pDS);

	Initialized = false;
}


void WeAudioDevice::Update()
{
	if (!Initialized) return;

	MusicPlayer->Update(WeTimer::Instance()->GetDeltaTimeDW());

	if (Listener)
		Listener->Update();

	CheckForDyingSounds();
}


void WeAudioDevice::CheckForDyingSounds()
{
	if (!Initialized) return;
	for (DyingSounds.First(); !DyingSounds.End();)
		if (DyingSounds.GetItem()->IsPlaying())
		{
			DyingSounds.Next();
		}
		else
		{
			delete DyingSounds.GetItem();
			DyingSounds.Delete();
		}
}


WeSound* WeAudioDevice::LoadSound(StringParam name, bool b3DSound, bool FromPackage, bool ForceReload, DWORD Flags)
{
	if (!Initialized) return NULL;

	WeSound* tmp;

	if(!ForceReload)
		for(Sounds.First(); !Sounds.End(); Sounds.Next())
		{
			if(Sounds.GetItem()->Name == name)
			{
				return Clone(Sounds.GetItem());
			}
		}

    tmp = new WeSound(pDS);
	tmp->CreationFlags = Flags;
	if(!b3DSound)
	{
		if(tmp->Load(name, FromPackage))
		{
			Sounds.PushBack(tmp);
			tmp->Name = name;
			return Clone(tmp);
		}
		else
		{
			return NULL;
		}
	}

	if(tmp->Load3D(name, FromPackage))
	{
		Sounds.PushBack(tmp);
		tmp->Name = name;
		return Clone(tmp);
	}
	else
	{
		return NULL;
	}
}


WeSound* WeAudioDevice::LoadSoundAuto(String name, bool b3DSound)
{
	if (!AutoLoadFromPackage)
	{
		String ToLoad = ".\\";
		for (int i = 0; i < 3; ++i)
		{
			ToLoad += name.Extract('.', i);
			if (i < 2)
				ToLoad += "\\";
			else
				ToLoad += ".wav";
		}

		return WeAudioDevice::Instance()->LoadSound(ToLoad, b3DSound, false);
	}
	else
	{
		return WeAudioDevice::Instance()->LoadSound(name, b3DSound, true);
	}
}


WeSound* WeAudioDevice::Clone(WeSound* SourceSound)
{
	if(!SourceSound) return NULL;

	WeSound* tmp = new WeSound(pDS);
	tmp->Name = SourceSound->Name;
    pDS->DuplicateSoundBuffer(SourceSound->Buffer, &tmp->Buffer);
	tmp->CreateSecBuffer();
	if (SourceSound->Is3DSound) tmp->Init3D();
	tmp->SetVolume(SoundVolume);

	Clones.PushBack(tmp);

    return tmp;
}


bool WeAudioDevice::FreeSound(WeSound* Sound)
{
	if (!Initialized) return true;
	
	//for (Clones.First(); !Clones.End();)
	//{
	//	if (Clones.GetItem()->Name == Sound->Name)
	//	{
	//		delete Clones.GetItem();
	//		Clones.Delete();
	//	}
	//	else
	//		Clones.Next();
	//}

	if (Sound)
	{
		for(Sounds.First(); !Sounds.End(); Sounds.Next())
		{
			if(Sounds.GetItem()->Name == Sound->Name)
			{
				Sounds.Delete();
				break;
			}
		}

		delete Sound;
	}

	return true;
}


void WeAudioDevice::PlayMusic(StringParam MusicName)
{
	if (!Initialized) return;
	if (!MusicPlayer) return;

	MusicPlayer->Play(MusicName);

	CurrentSongName = MusicName;
}


void WeAudioDevice::StopMusic()
{
	if(!Initialized) return;
	if(!MusicPlayer) return;

	MusicPlayer->Stop();
}


void WeAudioDevice::SetMusicVolume(float Volume)
{
	if(!Initialized) return;
	if(!MusicPlayer) return;

	MusicVolume = Volume;
	MusicPlayer->SetMusicVolume(GetGain(Volume));

	if (Volume > 0.0f)
		MusicPlayer->Play();
	else
		MusicPlayer->Stop();
}


void WeAudioDevice::SetSoundVolume(float Volume)
{
	if (!Initialized) return;

	SoundVolume = Volume;

	for (Clones.First(); !Clones.End(); Clones.Next())
	{
		Clones.GetItem()->SetVolume(Volume);
	}
}


bool WeAudioDevice::CreateListener()
{
	DSBUFFERDESC dsbdesc;
	LPDIRECTSOUND3DLISTENER8 DSListener = NULL;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

    // Obtain primary buffer, asking it for 3D control
    ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
    if( FAILED( pDS->CreateSoundBuffer( &dsbdesc, &pDSBPrimary, NULL ) ) )
        return false;

    if( FAILED( pDSBPrimary->QueryInterface(	IID_IDirectSound3DListener,
													(VOID**)&DSListener ) ) )
	{
        SAFE_RELEASE( pDSBPrimary );
        return false;
    }

    SAFE_RELEASE( pDSBPrimary );

	Listener = new WeListener(DSListener);
	return true;
}



//convert linear amplitude to 100s of decibels
long WeAudioDevice::GetGain(float Gain)
{
	if (Gain <= 0.0f) return DSBVOLUME_MIN;
	if (Gain >= 1.0f) return DSBVOLUME_MAX;

	return (long)(2000.0f * log10f(Gain));
}


//convert 100s of decibels to linear amplitude
float WeAudioDevice::GetAmp(long Amp)
{
	if (Amp <= DSBVOLUME_MIN) return 0.0f;
	if (Amp >= DSBVOLUME_MAX) return 1.0f;

	return (float)(pow(10.0f, ((float)Amp / 2000.0f)));
}


void WeAudioDevice::SetAutoLoadFromPackage(bool Value)
{
	AutoLoadFromPackage = Value;
}

bool WeAudioDevice::GetAutoLoadFromPackage()
{
	return AutoLoadFromPackage;
}
