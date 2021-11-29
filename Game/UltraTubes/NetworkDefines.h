/*
	Author  : Vuk Vranjkovic
	File    : NetworkDefines.h
	Date    : 22.03.2006.
	Version : 1.00

	Purpose : defines Network messages and other network stuff
*/
#ifndef _NETWORK_DEFINES_H_
#define _NETWORK_DEFINES_H_

#define CREATE_GAME			101		//
#define ADD_SERVER			102		//
#define JOINED				103		//
#define JOIN_GAME			104		//
#define REFUSE_JOIN			105		//
#define PLAYER_INFO			106		//
#define KICK_PLAYER			107		//
//#define	GAME_INFO			107
#define START_GAME			108
#define GAME_STARTED		109
#define HOST_LEFT			110
#define TUBER_DESC			111
#define READY_FOR_RACE		112
#define START_RACE			113
#define RACE_FINISHED		114
#define RACE_INFO			115
#define CHANGE_TEAM 		116
#define TEAM_CHANGED		117
//#define GET_SERVER_LIST		117
#define SERVER_LIST			118
#define FIND_LAN_SERVER		119
#define I_AM_LAN_SERVER		120
#define CLEAR_SERVER		121
#define TABLE_INFO			122
#define GAMEFINISHED		123
#define CLIENT_RACEFINISHED	124
#define ADD_AI				125
#define GAME_SETTINGS		60

#ifndef WESERVER_PORT
#define WESERVER_PORT 10200
#endif

#endif