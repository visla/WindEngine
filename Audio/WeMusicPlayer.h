/*
       File: WeMusicPlayer.h
	   Author: Tomislav Kukic
	   Date: 08.03.2006
	   Version: 1.0
*/



#ifndef _WeMUSICPLAYER
#define _WeMUSICPLAYER


#include <CString.h>
#include <WindEngine.h>


class WeMusicStream;

class WINDENGINE_API WeMusicPlayer
{
protected:
	WeMusicStream*	Music;

	String			MusicName;

	int				SavedVolume;

public:
	WeMusicPlayer();
	~WeMusicPlayer();

	bool Play(StringParam Name);
	bool Play();
	void Stop();
	void SetMusicVolume(int Vol);
	int GetMusicVolume();
	bool Playing();

	void Update(DWORD DeltaTime);
};




#endif