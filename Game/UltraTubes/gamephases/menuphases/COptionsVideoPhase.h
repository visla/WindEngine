/*
   COptionsVideoPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _COptionsVideoPhase_H
#define _COptionsVideoPhase_H

#include <CAbstractPhase.h>
#include <CString.h>
#include <WeCEGUI.h>


class COptionsVideoPhase: public CAbstractPhase
{
protected:

	bool WNDFadeIn;
	float WNDAlpha;

public:
	static COptionsVideoPhase*	Instance;

	bool Created;

						COptionsVideoPhase();
						~COptionsVideoPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

	static bool OnBack(const CEGUI::EventArgs& e);
	static bool OnResolutionChanged(const CEGUI::EventArgs& e);
	static bool OnEffectChanged(const CEGUI::EventArgs& e);
	static bool OnTextureChanged(const CEGUI::EventArgs& e);
	static bool OnPhysicsChanged(const CEGUI::EventArgs& e);
	static bool OnPostProcessChanged(const CEGUI::EventArgs& e);
	static bool OnParticlesChanged(const CEGUI::EventArgs& e);
	static bool OnGlowChanged(const CEGUI::EventArgs& e);
	static bool OnShadowsChanged(const CEGUI::EventArgs& e);
	static bool OnProjektorsChanged(const CEGUI::EventArgs& e);
	static bool OnFlaresChanged(const CEGUI::EventArgs& e);
	static bool OnMotionBlurChanged(const CEGUI::EventArgs& e);
	static bool OnAntialiasingChanged(const CEGUI::EventArgs& e);
	static bool OnAnisotropicChanged(const CEGUI::EventArgs& e);
	static bool OnSpecularChanged(const CEGUI::EventArgs& e);
	static bool OnBumpingChanged(const CEGUI::EventArgs& e);
	static bool OnRefractionChanged(const CEGUI::EventArgs& e);

	static void UpdateOptions();
};


#endif