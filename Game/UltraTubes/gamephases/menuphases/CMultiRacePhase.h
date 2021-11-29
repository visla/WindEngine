/*
   CMultiRacePhase.h

   Autor: Alexa
   Date: 21.09.2006
*/


#ifndef _CMultiRacePhase_H
#define _CMultiRacePhase_H

#include <CAbstractPhase.h>
#include <WeCEGUI.h>
#include <CString.h>
#include <CTableManager.h>
#include <WeTexture.h>


#define MRS_CHOOSEMAP   1
#define MRS_HOSTING     2
#define MRS_ACCEPTING   3
#define MRS_JOINING     4



class CMultiRacePhase : public CAbstractPhase
{
public:
	static CMultiRacePhase*	Instance;

							CMultiRacePhase();

	void					Tick();
	void					Start();
	void					End();
	void					Render();

	int						MultiRaceState; // MRS_XXX

	bool                    GameStarted;

	WeTexture*				MapDescImage;


protected:
	String					MapExt;
	int						CurrentMapIndex;

	void                    CreateOptionsComboboxes();
	void                    CreateRootComboboxes();
	void					MakeTableFromSettings();
	void					UpdateMapInfo();
	void					CheckLastOne();

	static bool				OnPrevMap(const CEGUI::EventArgs &e);
	static bool				OnNextMap(const CEGUI::EventArgs &e);

	static bool				OnRaceTypeCombobox(const CEGUI::EventArgs &e);
	static bool				OnWeaponCombobox(const CEGUI::EventArgs &e);
	static bool				OnCratesCombobox(const CEGUI::EventArgs &e);
	static bool				OnMoneyCombobox(const CEGUI::EventArgs &e);
	static bool				OnLapCountCombobox(const CEGUI::EventArgs &e);

	static bool				OnUserChange(const CEGUI::EventArgs &e);
	static bool				OnTeamChange(const CEGUI::EventArgs &e);

	static bool				OnCreate(const CEGUI::EventArgs &e);
	static bool				OnStart(const CEGUI::EventArgs &e);
	static bool				OnBack(const CEGUI::EventArgs &e);
	static bool				OnCancel(const CEGUI::EventArgs &e);

};

#endif