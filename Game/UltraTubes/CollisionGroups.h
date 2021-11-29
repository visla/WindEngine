/*
       File: CollisionGroups.h
	   Author: Goran Bogdanovic
	   Date: 12.04.2006.
	   Version: 1.0
*/


#ifndef _COLLISIONGROUPS_H
#define _COLLISIONGROUPS_H


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// By adding any new collision groups here,
// you are also required to add those groups to
// CPlayGamePhase::CollisionBatch.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#define COLLISION_GROUP_PREFAB			0
#define COLLISION_GROUP_RESERVED		1
#define COLLISION_GROUP_WEAPON			2
#define COLLISION_GROUP_PICKUP			3
#define COLLISION_GROUP_EXPLOSION		4
#define COLLISION_GROUP_MINE			5
#define COLLISION_GROUP_FRAGMENT		6
#define COLLISION_GROUP_NOTIFY_TRIGGER	7
#define COLLISION_GROUP_SHELL			8
#define COLLISION_GROUP_TUBER			9
#define COLLISION_GROUP_TRANSMINE		10


#endif