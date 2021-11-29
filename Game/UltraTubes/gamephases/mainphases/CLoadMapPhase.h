/*
   CLoadMapPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0
*/


#ifndef _CLoadMapPhase_H
#define _CLoadMapPhase_H

#include <CAbstractPhase.h>
#include <WeTexture.h>
#include <WeTextureManager.h>

class CProgressBar;


class CLoadMapPhase: public CAbstractPhase
{
protected:
	void				LoadMaterials(int i);
	void				LoadTextures(int i);
	void				LoadStaticModels(int i);
	void				LoadSounds(int i);
	void				LoadTubers(int i);
	void				LoadPSs();

	float				SavedSoundVolume;

public:
	static CLoadMapPhase*	Instance;
	static void				OnLoadProgress(float Progress, bool &StopLoading);

						CLoadMapPhase();

	WeTexture			*Background;	// slika kao pozadina pri ucitavanju
	CProgressBar		*ProgressBar;	// this is a progress bar... not a cow, but a progress bar!
	bool				MapLoaded;
	bool				PreLoaded;
	int					ShitLoaded;
	int					PartLoaded;

	float				PercentLoaded;
	float				StaticsPercentLoaded;

	bool				InfoLoaded;
	String				LoadingImage;
	String				StoryText;


	void				Tick();
	void				Start();
	void				End();
	void				Render();

};


#endif