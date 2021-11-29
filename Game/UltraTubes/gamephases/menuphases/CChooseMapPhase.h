/*
   File    : CChooseMapPhase.h
   Autor   : Vuk Vranjkovic
   Date    : 27.3.2006.
   Version : 1.0
*/
#ifndef _CChooseMapPhase_H
#define _CChooseMapPhase_H

#include <CAbstractPhase.h>
#include <WeCEGUI.h>
#include <CString.h>
#include <WeTexture.h>

class CChooseMapPhase: public CAbstractPhase
{
public:
	static CChooseMapPhase*	Instance;

						CChooseMapPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();




private:
	WeTexture* MapDescImage;

	// Combobox callbacks
	static bool OnCreatesCombobox(const CEGUI::EventArgs &);
	static bool OnWeaponCombobox(const CEGUI::EventArgs &);
	static bool OnMoneyCombobox(const CEGUI::EventArgs &);
	static bool OnLapCountCombobox(const CEGUI::EventArgs &);
	static bool OnMapTypeCombobox(const CEGUI::EventArgs &);
	static bool OnMapNameCombobox(const CEGUI::EventArgs &);

	// Button callbacks
	static bool OnBack(const CEGUI::EventArgs &);
	static bool OnCreate(const CEGUI::EventArgs &);

	void CreateOptionsComboboxes();
};


#endif

