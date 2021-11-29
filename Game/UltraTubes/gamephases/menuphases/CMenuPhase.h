/*
   CMenuPhase.h

   Autor: Nebojsa Vislavski
   Date: 13.3.2006.
   Version: 1.0

*/


#ifndef _CMenuPhase_H
#define _CMenuPhase_H

#include <CAbstractPhase.h>
#include <CMap.h>
#include <CMenuCamera.h>

class WeWorld;
class WeSound;


enum MenuSoundType
{
	MenuSoundStdButton,
	MenuSoundAltButton,
	MenuSoundDenied,
	MenuSoundKeyPress,
	MenuSoundVolumeTest
};


class CMenuPhase: public CAbstractPhase
{
friend class CLoadMenuPhase;

protected:
	WeSound*			SoundButton;
	WeSound*			SoundButtonAlt;
	WeSound*			SoundDenied;
	WeSound*			SoundKeyPress;
	WeSound*			SoundVolumeTest;

public:
	static CMenuPhase*	Instance;
	IDirect3DStateBlock9* ppSB;

	CMenuCamera*		MenuCamera;
	CMap				MenuWorld;
	float				BlurFactor;

						CMenuPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();
	void				SaveStates(bool Befoure = true);

	void				RenderStdScreen();

	void				PlayMenuSound(MenuSoundType TypeOfTehSoundToPlayForMenu);

};


#endif

