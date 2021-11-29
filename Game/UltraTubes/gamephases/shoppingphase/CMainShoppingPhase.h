/*
   CMainShoppingPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CMainShoppingPhase_H
#define _CMainShoppingPhase_H

#include <CAbstractPhase.h>
#include <WeCEGUI.h>
#include <CString.h>

class CMainShoppingPhase: public CAbstractPhase
{
protected:
	CEGUI::Window*	BuyShipText;
	float			BuyShipTime;

public:
	static CMainShoppingPhase*	Instance;

						CMainShoppingPhase();



    //bool				Created;
	bool				ShipBought;

	static bool			OnShipyard(const CEGUI::EventArgs& e);
	static bool			OnUpgradeShip(const CEGUI::EventArgs& e);
	static bool			OnStartRace(const CEGUI::EventArgs& e);
	static bool			OnBack(const CEGUI::EventArgs& e);

	void				Tick();
	void				Start();
	void				End();
	void				Render();

};


#endif

