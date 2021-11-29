/*

    CMusicStream.cpp

        Author: Nikola Kuzmanovic
        Date: 18.07.2005
        Version: 1.0

		History:
		1.0	It works!!!
	
 */
#ifndef We_MUSIC
#define We_MUSIC

#include <CString.h>
#include <mmreg.h>
#include <dsound.h>
#include <ogg.h>
#include <vorbisfile.h>
#include <math.h>

#include <WindEngine.h>


enum MUSIC_STATES
{
	MS_OK,
	MS_BUFFERS_FULL,
	MS_FAILED_TO_LOAD_FILE,
	MS_FAILED_TO_OPEN_OGG_STREAM,
	MS_OGG_STREAM_NOT_SEEKABLE,
	MS_OGG_STREAM_ERROR,
	MS_D3S_ERROR,
	MS_D3S_INVALID_PARAM,
	MS_STREAMING_ERROR,
	MS_STOPPED,
	MS_STOPPING,
	MS_EMPTY
};

class WINDENGINE_API WeMusicStream
{
	public:

		WeMusicStream(LPDIRECTSOUND8 Device, DWORD fadein = 0, DWORD fadeout = 0);
		~WeMusicStream();

		void EnableFade() { FadeEnabled = true; }
		void DiseableFade() { FadeEnabled = false; }

        bool Load(StringParam FileName);// File name with extension!!!
        bool Release();
        bool Play();
        bool Playing();

		bool Stop();
		bool Stoping() { return StopingTime; }

        bool Update(DWORD deltaTime);
		int GetMusicState() { return MusicState; }

		DWORD GetFadeOutTime() { return FadeOutTime; }
		void SetFadeOutTime(DWORD newFadeOutTime)
		{
			if(newFadeOutTime > 0)
				FadeOutTime = newFadeOutTime;
		}

		DWORD GetFadeInTime() { return FadeInTime; }
		void SetFadeInTime(DWORD newFadeInTime)	
		{
			if(newFadeInTime > 0)
				FadeInTime = newFadeInTime; 
		}

		int GetMusicVolume() { return MaxMusicVolume; }
		void SetMusicVolume(int newMusicVolume)
		{
			if( (newMusicVolume <= 0) )//&& (newMusicVolume > -10000))
			{
				MaxMusicVolume = newMusicVolume;
			}
		}

    protected:

        bool Stream(DWORD deltaTime);
		void RestoreSoundBuffer( bool* pbWasRestored );
		void MakeBuffer();

    private:

		LPDIRECTSOUND8 pDevice;

        OggVorbis_File  oggStream;
        vorbis_info*    vorbisInfo;
        vorbis_comment* vorbisComment;

        LPDIRECTSOUNDBUFFER8 SoundBuffer;
		WAVEFORMATEX WaveFormatSpecifiers;

		bool FadeEnabled;
		DWORD ElapsedTimeFromStop;
		DWORD ElapsedTime;
		DWORD FadeOutTime;
		DWORD FadeInTime;
		bool StopingTime;
		int MaxMusicVolume;
		bool FileLoaded;

		DWORD WritePosition;
		DWORD TotalPosition;

		DWORD updateTime;

		DWORD BufferSize;

		MUSIC_STATES MusicState;
};

#endif
