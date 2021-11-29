/*
   WeNetworkActorState.h

   Autor: Nebojsa Vislavski
   Date: 5.1.2006.
   Version: 1.0

   this is Network state or simply group of properties that have special network priority.
   ex. Player could update its positions every frame but HP doesnt have to be updated every frame.

   You could inherit this class and make it to be

   CPlayer::CAddOnProps 
     int HP
	 int Ammo

   

  

*/

#ifndef _WENETWORKACTORSTATE_H
#define _WENETWORKACTORSTATE_H

#include <WindEngine.h>

class WeNetworkActor;
class WeNetworkMessage;

class WINDENGINE_API WeNetworkActorState
{
public:
	int	 Priority;
	WeNetworkActorState(int priority) { Priority = priority; }

	virtual bool Server_ReadyToSend(WeNetworkActor *Avatar) { return true; }
	virtual void Server_Replicate(WeNetworkActor *Avatar, WeNetworkMessage *Msg) {};
};


#endif