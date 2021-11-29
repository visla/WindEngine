/*
       File: WeMusicPlayer.cpp
	   Author: Tomislav Kukic
	   Date: 08.03.2006
	   Version: 1.0
*/




#include <WeMusicPlayer.h>
#include <WeMusicStream.h>
#include <WeAudioDevice.h>


WeMusicPlayer::WeMusicPlayer()
{
	Music = NULL;
	SavedVolume = 0;
}



WeMusicPlayer::~WeMusicPlayer()
{
	if(Music)
	{
		if(Music->Playing()) Music->Stop();
		Music->Release();
		SAFE_DELETE(Music);
	}
}



bool WeMusicPlayer::Play(StringParam Name)
{
	if(Music)
	{
		if(Music->Playing())
			if(MusicName == Name) return true;

		Music->Stop();
		SAFE_DELETE(Music);
	}

	Music = new WeMusicStream(WeAudioDevice::Instance()->pDS);
	if (!Music->Load(Name)) return false;
	MusicName = Name;

	SetMusicVolume(SavedVolume);

	bool Result = false;
	if (SavedVolume != DSBVOLUME_MIN)
		Result = Music->Play();

	return Result;
}

bool WeMusicPlayer::Play()
{
	if (!Music) return false;
	if (Music->Playing()) return false;

	SetMusicVolume(SavedVolume);

	bool Result = false;
	if (SavedVolume != DSBVOLUME_MIN)
		Result = Music->Play();

	return Result;
}


void WeMusicPlayer::Update(DWORD DeltaTime)
{
	if(!Music) return;
	if(!Music->Playing()) return;

	Music->Update(DeltaTime);
}


bool WeMusicPlayer::Playing()
{
	if(!Music) return false;

	return Music->Playing();
}


void WeMusicPlayer::Stop()
{
	if(!Music) return;
	if(!Music->Playing()) return;

	Music->Stop();
}


void WeMusicPlayer::SetMusicVolume(int Vol)
{
	SavedVolume = Vol;

	if(!Music) return;

	Music->SetMusicVolume(Vol);
}



int WeMusicPlayer::GetMusicVolume()
{
	if(!Music) return 0;

	return Music->GetMusicVolume();
}

