/*
   CStatisticsPhase.h

   Autor: Nebojsa Vislavski
   Date: 10.3.2006.
   Version: 1.0

*/


#ifndef _CStatisticsPhase_H
#define _CStatisticsPhase_H

#include <CAbstractPhase.h>
#include <WeTextureManager.h>
#include <WeCEGUI.h>

class CProfile;


class CStatisticsPhase: public CAbstractPhase
{
protected:
    WeTexture	*Background;
	float		Ypos;
	float		Alpha;

public:
	static CStatisticsPhase*	Instance;

						CStatisticsPhase();

	CEGUI::MultiColumnList* StatisticList;

	void				Tick();
	void				Start();
	void				End();
	void				Render();
	void				OnReturn();

	static bool			OnBackToMenu(const CEGUI::EventArgs& e);
};


#endif