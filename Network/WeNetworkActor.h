/*

    WeNetworkActor.h

	Autor: Nebojsa Vislavski
	Date: 20.9.2005.
	Version: 1.0

	This is network actor main class. You should inherit this if you want to make
	player or something similar. It has all functionality to work with network 
	with prediction. 

*/


#ifndef _WENETWORKACTOR_H
#define _WENETWORKACTOR_H


#include <WeActor.h>
#include <WeNetworkDevice.h>
#include <WeNetworkMessage.h>
#include <WeVector.h>
#include <SmartList.h>
#include <WindEngine.h>
#include <WeNetworkActorState.h>

class WeNetworkActor;
class WeNetworkClient;

typedef CSmartList <WeNetworkActor *> WeNetworkActors;

#define WENETMESSAGE_NA_REPLICATE	1


class WINDENGINE_API WeNetworkActor: public WeActor
{
friend WeNetworkClient;
friend WeServer;
friend WeClient;

// stuff for interpolaing locations
#define WEINTERPOLATION_NONE 0
#define WEINTERPOLATION_MOVE 1
#define WEINTERPOLATION_MOVEANDSTOP 2

protected:

	// DO NOT modify these two lines yourself. Server and client will modify this
	WeNetworkActorID			NetworkID; // this is ID on network
    bool						ServerSideActor; // if this actor is on server side. This sets NetworkClient automaticly when having avatar
	float						PredictionFactor; // this is from 0..1 and usually 1 unless some chaotic movement
	int							iInterpolatingType;
	float						InterpolationStartTime; // when interpolation started
	float						InterpolationTime; // for how long you should interpolate

	WeVector					RefVelocity; // used to save us previous delta velocity
	WeVector					RefAngularVelocity;
	WeVector					TmpVelocity; // to store for some "behind the wall" calculations
	WeVector					TmpAngularVelocity;	
	bool						ReceivedDeltaCommand;


	bool						Interpolating;
	WeVector					LocationDestination; 
	WeVector					LocationStarted;
	WeQuaternion				OrientationStarted;
	WeQuaternion				OrientationDestination;

	WeVector					OldLocation; // what is old location we have received. We need this to check if player is standing still maybe
	int							Priority;  // how often should replication occur. 1 means every server tick. 10 means every 10th server tick, -1 means never to replicate
	int							TickCounter; // when ticks to priority then it is time to send msg. Tick counter is initialized with NetworkID

	bool						IsInputPlayer; // if this is input player

	// these are states of actor that are having different priorities
	// you have to manually add States here
	CSmartList <WeNetworkActorState *> States; 

public:
								WeNetworkActor();
								~WeNetworkActor();

	void						Initialize(); // init defualt state
	bool						ClientDoesPrediction; // for input clients it is usually false

	// call this to handle replication message
	virtual void				Client_HandleReplication(WeNetworkMessage *Msg);
	// this is called to process msgs for network actor
	 // you should call Reading commands and not call BeginReading
	 // if Type = WENETMESSAGE_NA_REPLICATE then it is state replication message	
	virtual void				Client_HandleGameMessage(WeNetworkMessageType Type, WeNetworkMessage *Msg) {};
	// You need to override this if you wish to make delta command
	virtual void				Client_MakeDeltaCommand(WeNetworkMessage *Msg) { };
	
	// This will handle spawn message. if this is supposed to be input player 
	// then IsInputPlayer is set to true. Handle this bool as whatever you like.
	virtual void				Client_HandleSpawnMessage(WeNetworkMessage *Msg, bool IsInputPlayer);

	virtual void				Server_HandleTickMessage(WeNetworkMessageType Type, WeNetworkMessage *Msg);
	// This method will determine if you need to send message at all. Avatar is object who is going to receive msg.
	// so one example would be not to send anything if Avatar can`t see you
	// return true if you wish replication to occur
	virtual bool				Server_ReadyToSend(WeNetworkActor *Avatar);
	// you should make replication message here. If you want you can determine what data to send
	// based on distance from avatar and similar stuff.
	// you dont have to call Msg->Init in Server_Replicate method or even set own NetworkActorID. It is all set. Just write data.
	virtual void				Server_Replicate(WeNetworkActor *Avatar, WeNetworkMessage *Msg);
	virtual void				Server_MakeSpawnMessage(WeNetworkMessage *Msg);
	// this will decompress delta command and usually call ProcessInput.
	// Delta command consists of pressed and whatever you want to send as delta command
	virtual void				Server_HandleDeltaCommand(WeNetworkMessage *Msg) { };
	

	// this will make predicted location according to starting location, current location
	// and latency time. This is calculated by runngin physics
	virtual WeVector			Client_GetPredictedLocation(WeVector &StartingLocation); 
	virtual void				Client_InterpolateLocation(WeVector &Destination, float InterpolationTime);
    virtual void				Client_UpdateInterpolations();
	// read location from network message and set interpolation or not
	virtual void				Client_ReadLocation(WeNetworkMessage *Msg, bool Interpolate = true);
	void						SetPredictionFactor(float Factor) { PredictionFactor = Factor; }
	float						GetPredictionFactor() { return PredictionFactor; }
	void						Tick();
	virtual WeNetworkActorID	GetID(); 
	virtual void				SetID(WeNetworkActorID ID);  // this is called by client or network client
	static WeNetworkActorID		LastNetworkActorID; // last network actor ID
	static WeNetworkActors		NetworkActors;  // list of all network actors present
	static WeNetworkActor*		GetNetworkActor(WeNetworkActorID ID); // gets network actor from ID
	static WeNetworkActorID		GetUniqueID() { LastNetworkActorID++; return LastNetworkActorID; }
};


#endif

