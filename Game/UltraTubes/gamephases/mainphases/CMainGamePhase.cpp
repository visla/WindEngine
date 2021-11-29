/*
    CMainGamePhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CMainGamePhase.h>
#include <CGame.h>
#include <AllPhases.h>
#include <WeAudioDevice.h>

CMainGamePhase *CMainGamePhase::Instance = new CMainGamePhase;


CMainGamePhase::CMainGamePhase()
{
//	Camera = NULL;
	InputPlayer = NULL;
	CurrentSong = SongNone;
}

void CMainGamePhase::Start()
{
	Funny = false;
	InputPlayer = NULL;
	bInitCamera = false;
	bFixCameraDirection = false;
	Camera.Init();
	SetPhase(CLoadMapPhase::Instance);
}


void CMainGamePhase::RenderStdScreen()
{
}

void CMainGamePhase::End()
{	
	CAbstractPhase::End();
	//InputPlayer = NULL;
	//if (CurrentPhase) CurrentPhase->End(); // to ensure we end some subphase.
}

void CMainGamePhase::Tick()
{
	CAbstractPhase::Tick();
}

void CMainGamePhase::Render()
{
	if (CurrentPhase) CurrentPhase->Render();
}


void CMainGamePhase::PlaySong(Songs SongSelection)
{
	String SongFileName = "";

	CurrentSong = SongSelection;

	switch (CurrentSong)
	{
	case SongMenu:
		SongFileName = "Menu.ogg";
		break;
	case SongGarage:
		SongFileName = "Garage.ogg";
		break;
	case SongRace:
		SongFileName = CMainGamePhase::Instance->Map.MapHeader.MusicFile;
		break;
	case SongEndRace:
		SongFileName = CMainGamePhase::Instance->Map.MapHeader.StatMusic;
		break;
	case SongNone:
		WeAudioDevice::Instance()->StopMusic();
		return;
	}

	if (SongFileName != (String)"")
		WeAudioDevice::Instance()->PlayMusic(SongFileName);
	else
		WeAudioDevice::Instance()->StopMusic();
}

Songs CMainGamePhase::GetCurrentSong()
{
	return CurrentSong;
}
