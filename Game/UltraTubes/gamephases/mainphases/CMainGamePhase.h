/*
   CMainGamePhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CMainGamePhase_H
#define _CMainGamePhase_H

#include <CAbstractPhase.h>
#include <CMap.h>
#include <CGameCamera.h>


enum Songs
{
	SongNone,
	SongMenu,
	SongGarage,
	SongRace,
	SongEndRace
};

class CTuber;

class CMainGamePhase: public CAbstractPhase
{
protected:
	Songs				CurrentSong;


public:
	CMap				Map;
	CGameCamera			Camera;
	CTuber				*InputPlayer;
	bool				Funny;   // oznacava se na true ako smo dosli do trenutka kada ovo treba da bude ukljuceno
	bool				bInitCamera;
	bool				bFixCameraDirection;
	bool				UseReverseCamera;

#ifdef _DEBUG
	String				DebugStr;
	String				DebugStr1;
	String				DebugStr2;
	String				DebugStr3;
#endif

	void				RenderStdScreen();  // renderuje std igru. Mapu na kojoj se igra


	static CMainGamePhase*	Instance;

						CMainGamePhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

	void				PlaySong(Songs SongSelection);
	Songs				GetCurrentSong();

};


#endif