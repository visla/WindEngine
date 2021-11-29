/*

    WeMusicStream.cpp

        Author: Nikola Kuzmanovic
        Date: 18.07.2005
        Version: 1.0

		History:
		1.0	It works!!!
	
 */


#include <WeMusicStream.h>


WeMusicStream::WeMusicStream(LPDIRECTSOUND8 Device, DWORD fadein, DWORD fadeout)
{
	MusicState = MS_EMPTY;
	ElapsedTime = 0;
	ElapsedTimeFromStop = 0;
	MaxMusicVolume = 0;
	FadeInTime = fadein;
	FadeOutTime = fadeout;
	StopingTime = false;
	FileLoaded = false;
	WritePosition = 0;
	TotalPosition = 0;
	BufferSize = 0;
	updateTime = 0;

	pDevice = Device;
	SoundBuffer = NULL;

	WaveFormatSpecifiers.wFormatTag = WAVE_FORMAT_PCM;
    WaveFormatSpecifiers.wBitsPerSample = 16;
	WaveFormatSpecifiers.cbSize = 0;

	if ((fadein == 0) && (fadeout == 0))
		FadeEnabled = false;
	else
		FadeEnabled = true;
}

WeMusicStream::~WeMusicStream()
{
	Release();
}

bool WeMusicStream::Load(StringParam FileName)
{
	Release();

	FILE* oggFile;

	if(!(oggFile = fopen(FileName,"rb")))
	{
		MusicState = MS_FAILED_TO_LOAD_FILE;
		return false;
	}

    int result = -1;

	result = ov_open(oggFile, &oggStream, NULL, 0);

    if(result < 0)
	{
		MusicState = MS_FAILED_TO_OPEN_OGG_STREAM;
        return false;
	}

	if(!ov_seekable(&oggStream))
	{
		MusicState = MS_OGG_STREAM_NOT_SEEKABLE;
		return false;
	}

    vorbisInfo = ov_info(&oggStream, -1);
    vorbisComment = ov_comment(&oggStream, -1);

    if(vorbisInfo->channels == 1)
        WaveFormatSpecifiers.nChannels = 1;
    else
        WaveFormatSpecifiers.nChannels = 2;
	WaveFormatSpecifiers.nSamplesPerSec = vorbisInfo->rate;
	WaveFormatSpecifiers.nBlockAlign = (WaveFormatSpecifiers.wBitsPerSample * WaveFormatSpecifiers.nChannels) / 8;
    WaveFormatSpecifiers.nAvgBytesPerSec = WaveFormatSpecifiers.nSamplesPerSec * WaveFormatSpecifiers.nBlockAlign;

	
	MakeBuffer();

	if (MusicState != MS_OK)
		return false;

	FileLoaded = true;
	return true;

}

void WeMusicStream::MakeBuffer() 
{ 
	DSBUFFERDESC dsbdesc; 
	LPDIRECTSOUNDBUFFER pDsb = NULL;
	 
	// Set up DSBUFFERDESC structure. 
	 
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); 
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 

	dsbdesc.dwFlags = DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLVOLUME; // | DSBCAPS_GLOBALFOCUS;
			
	BufferSize = 10 * WaveFormatSpecifiers.nAvgBytesPerSec;

	dsbdesc.dwBufferBytes = BufferSize; 
	dsbdesc.lpwfxFormat = &WaveFormatSpecifiers; 
	 
	// Create buffer. 
	 
	if(FAILED(pDevice->CreateSoundBuffer(&dsbdesc, &pDsb, NULL)))
	{
		MusicState = MS_D3S_ERROR;
		return;	
	}
	if(FAILED(pDsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*) &SoundBuffer)))
	{
		MusicState = MS_D3S_ERROR;
		return;	
	}
	SAFE_RELEASE(pDsb);
	MusicState = MS_OK;
} 

void WeMusicStream::RestoreSoundBuffer( bool* pbWasRestored )
{
    HRESULT hr;

	if( !FileLoaded )
	{
		MusicState = MS_EMPTY;
        return;
	}
    if( pbWasRestored )
        *pbWasRestored = FALSE;

	if (SoundBuffer == NULL)
	{
		MusicState = MS_D3S_ERROR;
		return;
	}

    DWORD dwStatus;
    if(FAILED(SoundBuffer->GetStatus( &dwStatus )))
	{
		MusicState = MS_D3S_ERROR;
		return;
	}
       
    if( dwStatus & DSBSTATUS_BUFFERLOST )
    {
        do 
        {
            hr = SoundBuffer->Restore();
            if( hr == DSERR_BUFFERLOST )
                Sleep( 10 );
        }
        while( ( hr = SoundBuffer->Restore() ) == DSERR_BUFFERLOST );

        if( pbWasRestored != NULL )
            *pbWasRestored = TRUE;
	}
}


bool WeMusicStream::Release()
{
	if (SoundBuffer != NULL)
	{
		if(FAILED(SoundBuffer->Stop()))
		{
			MusicState = MS_D3S_ERROR;
			return false;
		}

		SAFE_RELEASE(SoundBuffer);

		ov_clear(&oggStream);
	}
	ElapsedTime = 0;
	ElapsedTimeFromStop = 0;
	MaxMusicVolume = 0;
	FileLoaded = false;
	WritePosition = 0;
	TotalPosition = 0;
	BufferSize = 0;
	updateTime = 0;

	MusicState = MS_EMPTY;
	FileLoaded = false;
	return true;
}



bool WeMusicStream::Play()
{
	if( (MusicState != MS_OK) && (MusicState != MS_STOPPED) ) return false;

	if(Playing())
	{
		MusicState = MS_OK;
        return true;
	}

	if (SoundBuffer == NULL)
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}

	if((FadeInTime > 0) && FadeEnabled )
	{
		if(FAILED(SoundBuffer->SetVolume(-10000)))
		{
			MusicState = MS_D3S_ERROR;
			return false;
		}
	}
	else
		if(FAILED(SoundBuffer->SetVolume(MaxMusicVolume)))
		{
			MusicState = MS_D3S_ERROR;
			return false;
		}

	if(!Stream(0))
	{
		MusicState = MS_STREAMING_ERROR;
		return false;
	}
	
	if(FAILED(SoundBuffer->Play(0,0,DSBPLAY_LOOPING)))
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}
    
	MusicState = MS_OK;
    return true;
}




bool WeMusicStream::Playing()
{
    DWORD dwStatus;
	if (SoundBuffer == NULL)
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}

	if(FAILED(SoundBuffer->GetStatus( &dwStatus )))
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}
	return ( ( dwStatus & DSBSTATUS_PLAYING ) != 0 );
}


bool WeMusicStream::Update(DWORD deltaTime)
{
	if(!FileLoaded)
	{
		MusicState = MS_EMPTY;
		return false;
	}
	if(MusicState == MS_STOPPED)
		return false;

	if (SoundBuffer == NULL)
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}

	ElapsedTime += deltaTime;

	LONG Volume = 0;

	if((ElapsedTime < FadeInTime) && FadeEnabled)
		//Volume = (float)ElapsedTime / (float)FadeInTime;
		Volume = (long)( 1 - ( (float)ElapsedTime / (float)FadeInTime ) ) * -10000;

	if(StopingTime && FadeEnabled)
	{
		ElapsedTimeFromStop += deltaTime;
		Volume += (long)( (float)ElapsedTimeFromStop / (float)FadeOutTime ) * -10000;
		if(Volume < -10000)
		{
			MusicState = MS_STOPPED;

			if(FAILED(SoundBuffer->Stop()))
			{
				MusicState = MS_D3S_ERROR;
				return false;
			}
			if(FAILED(SoundBuffer->SetCurrentPosition(0)))
			{
				MusicState = MS_D3S_ERROR;
				return false;
			}

			WritePosition = 0; 
			TotalPosition = 0;
			ElapsedTime = 0;
			ElapsedTimeFromStop = 0;
			updateTime = 0;

			if(ov_time_seek(&oggStream, 0) != 0)
			{
				MusicState = MS_OGG_STREAM_ERROR;
				return false;
			}
			return true;
			
		}
	}

	if(Volume > MaxMusicVolume)
	{
		if(FAILED(SoundBuffer->SetVolume(MaxMusicVolume)))
		{
			MusicState = MS_D3S_ERROR;
			return false;
		}
	}
	else
        if(FAILED(SoundBuffer->SetVolume(Volume)))
		{
			MusicState = MS_D3S_ERROR;
			return false;
		}

	updateTime += deltaTime;

	if(updateTime < 250)
	{
		if(Playing())
		{
			MusicState = MS_OK;
			return true;
		}

		if(FAILED(SoundBuffer->Play(0,0,DSBPLAY_LOOPING)))
		{
			MusicState = MS_D3S_ERROR;
			return false;
		}
		MusicState = MS_OK;
		return true;
	}

    if(!Stream(updateTime))
	{
		Release();
		return false;
	}

	updateTime = 0;

	if(Playing())
	{
		MusicState = MS_OK;
        return true;
	}

	if(FAILED(SoundBuffer->Play(0,0,DSBPLAY_LOOPING)))
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}

	MusicState = MS_OK;
    return true;
}




bool WeMusicStream::Stream(DWORD deltaTime)
{
    char* pcm;
    UINT	size = 0;
	UINT	pomBuffSize = 0;
	int	result;
	int section;
    VOID*   pDSLockedBuffer = NULL;
    VOID*   pDSLockedBuffer2 = NULL;
    DWORD   dwDSLockedBufferSize = 0;
    DWORD   dwDSLockedBufferSize2 = 0;
	DWORD tempWritePosition = 0;
	DWORD tempPos = 0;

	
	if (SoundBuffer == NULL)
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}

	if(FAILED(SoundBuffer->GetCurrentPosition(&tempWritePosition,&tempPos)))
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}

	if ((TotalPosition == 0) && (deltaTime == 0))
		pomBuffSize = BufferSize;
	else
		if ((deltaTime < 3000) && !((WritePosition > tempWritePosition) && (WritePosition < tempPos)))
			if ( WritePosition > tempWritePosition )
				pomBuffSize = BufferSize - ( WritePosition - tempWritePosition );
			else 
				if ( WritePosition < tempWritePosition )
					pomBuffSize = tempWritePosition - WritePosition;
				else
				{
					MusicState = MS_OK;
					return true;
				}
		else
		{
			if(FAILED(SoundBuffer->Stop()))
			{
				MusicState = MS_D3S_ERROR;
				return false;
			}
			if(FAILED(SoundBuffer->SetCurrentPosition(0)))
			{
				MusicState = MS_D3S_ERROR;
				return false;
			}
			WritePosition = 0;
			pomBuffSize = BufferSize;
		}
	

	pcm = (char*) malloc( sizeof(char) * pomBuffSize );

    while(size < pomBuffSize)
    {
        result = ov_read(&oggStream, pcm + size, pomBuffSize - size, 0, 2, 1, &section);
    
        if(result > 0)
            size += result;
        else
            if(result < 0)
			{
				MusicState = MS_OGG_STREAM_ERROR;
                return false;
			}
            else
				if(ov_time_seek(&oggStream, 0) != 0)
				{
					MusicState = MS_OGG_STREAM_ERROR;
					return false;
				}

    }

	HRESULT hr = SoundBuffer->Lock(WritePosition, size, &pDSLockedBuffer, 
		&dwDSLockedBufferSize, &pDSLockedBuffer2, &dwDSLockedBufferSize2, 0);
	
	if (DSERR_BUFFERLOST == hr) 
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}
	else 
		if(hr == DSERR_INVALIDPARAM)
		{
			MusicState = MS_D3S_INVALID_PARAM;
			return false;
		}
		else
		{
			CopyMemory(pDSLockedBuffer, pcm, dwDSLockedBufferSize); 
			if (NULL != pDSLockedBuffer2) 
			{ 
				CopyMemory(pDSLockedBuffer2, pcm + dwDSLockedBufferSize, dwDSLockedBufferSize2); 
			}
		}

	if(FAILED(SoundBuffer->Unlock(pDSLockedBuffer, 
				dwDSLockedBufferSize, pDSLockedBuffer2, dwDSLockedBufferSize2)))
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}
	WritePosition = (WritePosition + size) % BufferSize;
	TotalPosition += size;

	free(pcm);
    MusicState = MS_OK;
    return true;
}


bool WeMusicStream::Stop() 
{
	if(!FileLoaded)
		return false;
	if(FadeEnabled)
	{
		StopingTime = true;
		return false;
	}
	MusicState = MS_STOPPED;

	if (SoundBuffer == NULL)
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}

	if(FAILED(SoundBuffer->Stop()))
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}
	if(FAILED(SoundBuffer->SetCurrentPosition(0)))
	{
		MusicState = MS_D3S_ERROR;
		return false;
	}

	WritePosition = 0; 
	TotalPosition = 0;
	ElapsedTime = 0;
	ElapsedTimeFromStop = 0;
	updateTime = 0;

	if(ov_time_seek(&oggStream, 0) != 0)
		{
			MusicState = MS_OGG_STREAM_ERROR;
			return false;
		}
	return true;
}

	
	



