/*
     File: WeSound.cpp
	 Author: Tomislav Kukic
	 Date: 10.01.2006.
	 Version: 1.1

	 History:
	 1.1	Load is changed to work with WeFileStream    (Toma)
*/





#include <d3d9.h>
#include <WePackage.h>
#include <WeAudioDevice.h>
#include <WeFileStream.h>
#include <WeSound.h>
#include <WeSoundAttenuator.h>

#define MIN_SOUND_DISTANCE	10.0f
#define MAX_SOUND_DISTANCE	2000.0f


WeSound::WeSound(IDirectSound8* DirectSound)
{
	if(!DirectSound) return;

	DSound = DirectSound;

	BufferCreated = false;
	Buffer = NULL;
	SecBuffer = NULL;
	Buffer3D = NULL;
	Playing = false;
	Is3DSound = false;

	CreationFlags = 0;
	
	AttenuationFactor = 1.0f;
}


bool WeSound::CreateBuffer()
{
	DSBUFFERDESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(DSBUFFERDESC));

    DWORD dwSize = sizeof(DSBUFFERDESC);
	BufferDesc.dwSize = dwSize;
	BufferDesc.dwBufferBytes = MemorySize;
	BufferDesc.dwReserved = 0;
	if (CreationFlags == 0)
		BufferDesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_LOCSOFTWARE | DSBCAPS_MUTE3DATMAXDISTANCE | DSBCAPS_CTRLVOLUME;
	else
		BufferDesc.dwFlags = CreationFlags;
	BufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
	BufferDesc.lpwfxFormat = &WaveFormat;

	if(D3D_OK != DSound->CreateSoundBuffer(&BufferDesc, &Buffer, NULL))
	{
        SecBuffer = NULL;
		return false;
	}

	return CreateSecBuffer();
}

bool WeSound::CreateSecBuffer()
{
	if (!Buffer) return false;

	if(D3D_OK != Buffer->QueryInterface(IID_IDirectSoundBuffer8, (VOID**)&SecBuffer))
	{
		SecBuffer = NULL;
		return false;
	}

	if(D3D_OK != SecBuffer->QueryInterface(IID_IDirectSound3DBuffer8,(VOID**)&Buffer3D))
	{
		Buffer3D = NULL;
	}
	else
	{
		Buffer3D->SetMode(DS3DMODE_DISABLE, DS3D_IMMEDIATE);
	}

	return true;
}


WeSound::~WeSound()
{
	Free();
	for (WeAudioDevice::Instance()->Clones.First(); !WeAudioDevice::Instance()->Clones.End(); WeAudioDevice::Instance()->Clones.Next())
	{
		if (this == WeAudioDevice::Instance()->Clones.GetItem())
		{
			WeAudioDevice::Instance()->Clones.Delete();
			break;
		}
	}
}


bool WeSound::Load(StringParam HoleName, bool FromPackage)
{
	WeFileStream FileStream;
	if(FromPackage) 
	{
		if(!WePackage::Instance()->BeginReading(HoleName, 4)) return false;
		FileStream = WePackage::Instance()->DataStream;
		FileStream.ReadDWord();
	}
	else
	{
		if(!FileStream.Open(HoleName)) return false;
	}

	char id[6]="    \0";
    DWORD size,format_length; //32 bit value to hold file size 

	FileStream.ReadData((void*)id, sizeof(char)*4);
    if (!strcmp(id, "RIFF")) 
	{
		size = FileStream.ReadDWord();
        FileStream.ReadData((void*)id, sizeof(char)*4);
        if (!strcmp(id,"WAVE")) 
        { 
            FileStream.ReadData((void*)id, sizeof(char)*4);
			format_length  = FileStream.ReadDWord();
			FileStream.ReadData((void*)&(WaveFormat.wFormatTag), sizeof(short));
			if(WaveFormat.wFormatTag != 1)
			{
				if(FromPackage)
					WePackage::Instance()->EndReading();
				else
					FileStream.Close();

				return false;
			}
			FileStream.ReadData((void*)&(WaveFormat.nChannels), sizeof(short));
			if(WaveFormat.nChannels != 1)
			{
				if(FromPackage)
					WePackage::Instance()->EndReading();
				else
					FileStream.Close();

				return false;
			}
			WaveFormat.nSamplesPerSec = FileStream.ReadDWord();
            WaveFormat.nAvgBytesPerSec = FileStream.ReadDWord();
			FileStream.ReadData((void*)&(WaveFormat.nBlockAlign), sizeof(short));
			FileStream.ReadData((void*)&(WaveFormat.wBitsPerSample), sizeof(short));
			WaveFormat.cbSize = 0;
			FileStream.ReadData((void*)id, sizeof(char)*4); 
			MemorySize = FileStream.ReadDWord(); 
			MemoryBuffer = (char *) malloc (sizeof(char) * (MemorySize));
			FileStream.ReadData(MemoryBuffer, sizeof(char)*MemorySize);

			CreateBuffer();

			//----------
			LPVOID lpvWrite;
			//DWORD  dwLength;

			if (DS_OK == SecBuffer->Lock(
				0,          // Offset at which to start lock.
				MemorySize, // Size of lock;
				&lpvWrite,  // Gets address of first part of lock.
				&MemorySize,  // Gets size of first part of lock.
				NULL,       // Address of wraparound not needed. 
				NULL,       // Size of wraparound not needed.
				DSBLOCK_ENTIREBUFFER))  // Flag.
			{
				memcpy(lpvWrite, MemoryBuffer, MemorySize);
				if(D3D_OK != SecBuffer->Unlock(
												lpvWrite,   // Address of lock start.
												MemorySize,   // Size of lock.
												NULL,       // No wraparound portion.
												0))         // No wraparound size.
				{
					SecBuffer = NULL;
					return false;
				}
			}
			else
			{
				SecBuffer = NULL;
				return false;
			}
			//-------------

			if(FromPackage)
				WePackage::Instance()->EndReading();
			else
				FileStream.Close();

			Name = HoleName;
			return true;
		} 
        else
		{
			if(FromPackage)
				WePackage::Instance()->EndReading();
			else
				FileStream.Close();
            return false;
		}
    }
    else
	{
		if(FromPackage)
			WePackage::Instance()->EndReading();
		else
			FileStream.Close();
        return false;
	}
}




bool WeSound::Load3D(StringParam HoleName, bool FromPackage)
{
	if(Load(HoleName, FromPackage))
	{
		Init3D();
		return true;
	}
	else
		return false;
}




bool WeSound::IsLoaded()
{
	return isLoaded;
}




bool WeSound::Save()
{
	return true;
}




void WeSound::Destroy()
{
}




void WeSound::Play(bool Looped, bool Reset)
{
	if (WeAudioDevice::Instance()->SoundVolume == DSBVOLUME_MIN) return;
	if(!SecBuffer) return;
	if (!Reset && IsPlaying()) return;

	HRESULT hr;
	SecBuffer->SetCurrentPosition(0);

	if(Looped)
		hr = SecBuffer->Play(0,0,DSBPLAY_LOOPING); 
	else
		hr = SecBuffer->Play(0,0,0); 


	if(hr != S_OK)
	{
		Playing = false;
	}

	Playing = true;
}





bool WeSound::IsPlaying()
{
	if (!SecBuffer) return false;
	DWORD Status;
	SecBuffer->GetStatus(&Status);
	return (Status & DSBSTATUS_PLAYING);
}




void WeSound::Stop()
{
	if(!SecBuffer) return;
	if(!Playing) return;

	SecBuffer->Stop();

	Playing = false;
}




void WeSound::Reset()
{
}



void WeSound::SetVolume(float NewVolume)
{
	if(!SecBuffer) return;

	WeSoundAttenuator::Instance->ProcessSound(this);
	Volume = NewVolume;

	SecBuffer->SetVolume(WeAudioDevice::GetGain(NewVolume * AttenuationFactor));
}


float WeSound::GetVolume()
{
	return Volume;
}


void WeSound::Update3DParams(WeVector &Position, WeVector &Velocity, WeVector &ConeOrientation, DWORD ConeInsideAngle, DWORD ConeOutsideAngle)
{
	if (!Buffer3D) return;

	DWORD Mode;
	Buffer3D->GetMode(&Mode);
	if(Mode == DS3DMODE_DISABLE) return;

	Buffer3D->SetPosition(Position.x, Position.y, Position.z, DS3D_DEFERRED);
	Buffer3D->SetVelocity(Velocity.x, Velocity.y, Velocity.z, DS3D_DEFERRED);
	Buffer3D->SetConeOrientation(ConeOrientation.x, ConeOrientation.y, ConeOrientation.z, DS3D_DEFERRED);
	Buffer3D->SetConeAngles(ConeInsideAngle, ConeOutsideAngle, DS3D_DEFERRED);
}


void WeSound::SetMaxDistance(float Dist)
{
	Buffer3D->SetMaxDistance(Dist, DS3D_DEFERRED);
}

void WeSound::Update(WeVector &Position, WeVector &Velocity)
{
	if (!Buffer3D) return;
	Buffer3D->SetPosition(Position.x, Position.y, Position.z, DS3D_DEFERRED);
	Buffer3D->SetVelocity(Velocity.x, Velocity.y, Velocity.z, DS3D_DEFERRED);
}


void WeSound::DieOnStop()
{
	if (this == NULL) return;

	for (WeAudioDevice::Instance()->DyingSounds.First(); !WeAudioDevice::Instance()->DyingSounds.End(); WeAudioDevice::Instance()->DyingSounds.Next())
		if (WeAudioDevice::Instance()->DyingSounds.GetItem() == this)
			return;

	WeAudioDevice::Instance()->DyingSounds.PushBack(this);
}


void WeSound::Free()
{
	if(IsPlaying()) Stop();

	SAFE_RELEASE(Buffer3D);
	SAFE_RELEASE(SecBuffer);
	SAFE_RELEASE(Buffer);
}


bool WeSound::Save(StringParam HoleName, bool Replace)
{
	//if(Replace) WePackage::Instance()->Remove(HoleName);

	FILE *f = fopen(Name, "rb");
	fseek(f,0,SEEK_END);
	DWORD Size = ftell(f);
	fclose(f);

	f = fopen(Name, "rb");

	char* SoundData;
	SoundData = new char[Size];
	fread((void*)SoundData, Size, 1, f);
	fclose(f);

	if(WePackage::Instance()->BeginWriting(HoleName, 4, Replace))
	{
		WePackage::Instance()->DataStream.WriteDWord(Size);
		WePackage::Instance()->DataStream.WriteData((void*)SoundData, (int)Size);

	}else return false;

	WePackage::Instance()->EndWriting();

	SAFE_DELETE(SoundData);
	return true;
}



void WeSound::SetFrequency(DWORD _Freq)
{
	if(!SecBuffer) return;
	SecBuffer->SetFrequency(_Freq);
}




DWORD WeSound::GetFrequency()
{
	if(!SecBuffer) return 0;
	DWORD tmpFreq = 0;
	SecBuffer->GetFrequency(&tmpFreq);
    return tmpFreq;
}


void WeSound::Init3D()
{
	if (!Buffer3D) return;
	
	Buffer3D->SetMode(DS3DMODE_NORMAL, DS3D_IMMEDIATE);
	Buffer3D->SetMinDistance(MIN_SOUND_DISTANCE, DS3D_IMMEDIATE);
	Buffer3D->SetMaxDistance(MAX_SOUND_DISTANCE, DS3D_IMMEDIATE);
	Is3DSound = true;
}


void WeSound::SetAttenuationFactor(float Factor)
{
	AttenuationFactor = Factor;
}

float WeSound::GetAttenuationFactor()
{
	return AttenuationFactor;
}
