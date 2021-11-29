/*
   COptionsAudioPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _COptionsAudioPhase_H
#define _COptionsAudioPhase_H

#include <CAbstractPhase.h>
#include <CString.h>
#include <WeCEGUI.h>

class WeSound;

class COptionsAudioPhase: public CAbstractPhase
{
protected:
	bool Created;

	bool WNDFadeIn;
	float WNDAlpha;

	WeSound* ButtonSound;

	static bool ManualScrollSet;

public:
	static COptionsAudioPhase*	Instance;

						COptionsAudioPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

	static bool OnBack(const CEGUI::EventArgs& e);
	static bool OnMusicVolumeChanged(const CEGUI::EventArgs& e);   
	static bool OnSoundVolumeChanged(const CEGUI::EventArgs& e);
	static bool OnSoundQualityChanged(const CEGUI::EventArgs& e);
};


#endif