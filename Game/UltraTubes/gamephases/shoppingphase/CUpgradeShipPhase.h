/*
   CUpgradeShipPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CUpgradeShipPhase_H
#define _CUpgradeShipPhase_H

#include <CAbstractPhase.h>
#include <WeCEGUI.h>
#include <SmartList.h>
#include <CString.h>


class CUpgradeItem;

class CUpgradeShipPhase: public CAbstractPhase
{
protected:
	CEGUI::Window* Cash;
	CEGUI::Window* BuyButton;

	CSmartList<CUpgradeItem*> UpgradeItems;
	void DoSelection(int x, int y);

	//bool BackEnters;
	//bool BackLeaves;
	//bool BuyEnters;
	//bool BuyLeaves;
	//bool StartEnters;
	//bool StartLeaves;

public:
	static CUpgradeShipPhase*	Instance;

						CUpgradeShipPhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();

	static bool OnBack(const CEGUI::EventArgs& e);
	static bool OnStartRace(const CEGUI::EventArgs& e);
    static bool OnBuy(const CEGUI::EventArgs& e);

	//bool OnBuyEnters(const CEGUI::EventArgs& e);
	//bool OnBuyLeaves(const CEGUI::EventArgs& e);
	//bool OnBackEnters(const CEGUI::EventArgs& e);
	//bool OnBackLeaves(const CEGUI::EventArgs& e);
	//bool OnStartEnters(const CEGUI::EventArgs& e);
	//bool OnStartLeaves(const CEGUI::EventArgs& e);
};


#endif

