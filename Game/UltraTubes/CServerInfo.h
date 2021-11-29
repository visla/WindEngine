/*
	Author  : Vuk Vranjkovic
	File    : CServerInfo.h
	Date    : 22.03.2006.
	Version : 1.00
*/
#ifndef _CServerInfo_h_
#define _CServerInfo_h_

#include <CString.h>

class CServerInfo
{
public:
	bool		FreeForHosting;
	bool		Playing;
	bool		Ladder;
	String		Name;
	String		IP;
	int			CurrentPlayers;
	int			MaxPlayer;
	String		Map;
	bool		LAN;

	CServerInfo();
};

inline CServerInfo::CServerInfo(){FreeForHosting=Ladder=true;Playing=false;}

#endif