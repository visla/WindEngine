/*
   CSingleRacePhase.h

   Autor: Vuk Vranjkovic
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CSingleRacePhase_H
#define _CSingleRacePhase_H

#include <CAbstractPhase.h>
#include <WeCEGUI.h>
#include <CString.h>
#include <CTableManager.h>
#include <WeTexture.h>



class CSingleRacePhase : public CAbstractPhase
{
public:
	static CSingleRacePhase*	Instance;

							CSingleRacePhase();

	void					Tick();
	void					Start();
	void					End();
	void					Render();


	bool                    GameStarted;

	static bool				OnStart(const CEGUI::EventArgs &e);
	static bool				OnBack(const CEGUI::EventArgs &e);

	WeTexture*				MapDescImage;


protected:
	String					MapExt;
	int						CurrentMapIndex;

	void                    CreateOptionsComboboxes();
	void                    CreateRootComboboxes();
	void					SetVisibleOtherPlayers(bool Visible);
	void					MakeTableFromSettings();
	void					UpdateMapInfo();
	void					CheckLastOne();
	void					CheckCatchUp();

	static bool				OnRaceTypeCombobox(const CEGUI::EventArgs &e);
	static bool				OnPrevMap(const CEGUI::EventArgs &e);
	static bool				OnNextMap(const CEGUI::EventArgs &e);
	static bool				OnUserChange(const CEGUI::EventArgs &e);
	static bool				OnTeamChange(const CEGUI::EventArgs &e);
	static bool				OnWeaponCombobox(const CEGUI::EventArgs &e);
	static bool				OnCratesCombobox(const CEGUI::EventArgs &e);
	static bool				OnUnlimitedSpeedCombobox(const CEGUI::EventArgs &e);
	static bool				OnCatchUpCombobox(const CEGUI::EventArgs &e);
	static bool				OnMoneyCombobox(const CEGUI::EventArgs &e);
	static bool				OnLapCountCombobox(const CEGUI::EventArgs &e);
	static bool				OnNameCombobox(const CEGUI::EventArgs &e);
};

#endif