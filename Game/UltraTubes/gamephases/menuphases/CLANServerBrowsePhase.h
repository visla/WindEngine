/*
   File    : CLANServerBrowsePhase.h
   Autor   : Vuk Vranjkovic
   Date    : 23.3.2006.
   Version : 1.0
*/
#ifndef _CLANServerBrowsePhase_H
#define _CLANServerBrowsePhase_H

#include <CAbstractPhase.h>
#include <CServerInfo.h>
#include <WeCEGUI.h>

class WeNetworkMessage;

class CLANServerBrowsePhase: public CAbstractPhase
{
public:
	static CLANServerBrowsePhase*	Instance;

						CLANServerBrowsePhase();

	void				Tick();
	void				Start();
	void				End();
	void				Render();




private:
	void				CreateServerTable();

	//Button callbacks
	static bool			OnJoin(const CEGUI::EventArgs &);
	static bool			OnRefresh(const CEGUI::EventArgs &);
	static bool			OnCreate(const CEGUI::EventArgs &);
	static bool			OnBack(const CEGUI::EventArgs &);


	//MultiColumnList callbacks
	static bool			OnServerTableSelect(const CEGUI::EventArgs &);
	static bool			OnServerTableDoubleClick(const CEGUI::EventArgs &);
	
	void				AddServer();

	bool				bRefreshServers;
	bool				bServerChoosen;
	float				Time;
	void				RefreshServers();

	CServerInfo		   *pServerInfo;

	friend class CGameClient;
};

#endif