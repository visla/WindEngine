/*

    WeNetworkActor.cpp

	Autor: Nebojsa Vislavski
	Date: 20.9.2005.
	Version: 1.0

	This is network actor main class. You should inherit this if you want to make
	player or something similar. It has all functionality to work with network 
	with prediction. 

*/

#include <WeNetworkActor.h>
#include <WeTimer.h>
#include <WeClient.h>
#include <WeNetworkClient.h>
#include <WeServer.h>

WeNetworkActorID WeNetworkActor::LastNetworkActorID = 0;
WeNetworkActors	 WeNetworkActor::NetworkActors;

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ProcessMessage
//         Input Parametars :  Msg
//         Output Parametars:  N/A
//         Comments         :  This is processing function. It parses arrived message
//
//////////////////////////////////////////////////////////////////////////
WeNetworkActor::WeNetworkActor()
{
	NetworkID			=	-1;
	ServerSideActor		=	false;
	PredictionFactor	=	1;
	Priority			=	1;
	Type				=	WETYPE_NETWORKACTOR;

	Interpolating = false;
	IsNetworkActor = true;
	ClientDoesPrediction = true;
	RefVelocity = WeVector(0,0,0);
	RefAngularVelocity = WeVector(0,0,0);	
	ReceivedDeltaCommand = false;
	IsInputPlayer = false;

	NetworkID = GetUniqueID();
	NetworkActors.PushBack(this);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ProcessMessage
//         Input Parametars :  Msg
//         Output Parametars:  N/A
//         Comments         :  This is processing function. It parses arrived message
//
//////////////////////////////////////////////////////////////////////////
WeNetworkActor::~WeNetworkActor()
{
	// remove it as Avatar if it someone`s avatar
	for (WeServer::Instance()->NetworkClients.First(); !WeServer::Instance()->NetworkClients.End(); WeServer::Instance()->NetworkClients.Next())
	{
		if (WeServer::Instance()->NetworkClients.GetItem()->GetAvatar() == this)
		{
			WeServer::Instance()->NetworkClients.GetItem()->SetAvatar(NULL);
		}
	}		
	

	for (States.First(); !States.End(); States.Delete())
	{
		SAFE_DELETE(States.GetItem());
	}

	for (NetworkActors.First(); !NetworkActors.End(); NetworkActors.Next())
	{
		if (NetworkActors.GetItem() == this)
		{
			NetworkActors.Delete();
			return;
		}
	}
}







/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ProcessMessage
//         Input Parametars :  Msg
//         Output Parametars:  N/A
//         Comments         :  This is processing function. It parses arrived message
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkActor::Initialize()
{
	WeActor::Initialize();
//	NetworkID = -1;
	TickCounter = -1;
	PredictionFactor = 1;
	InterpolationTime = 100;	
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ProcessMessage
//         Input Parametars :  Msg
//         Output Parametars:  N/A
//         Comments         :  This is processing function. It parses arrived message
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkActor::Server_HandleTickMessage(WeNetworkMessageType Type, WeNetworkMessage *Msg)
{
}


void WeNetworkActor::Client_HandleReplication(WeNetworkMessage *Msg)
{
	WeVector V, AV, tmpL;
	WeQuaternion quat;


	Msg->ReadVector(tmpL);
	if (PhysicHandle)
	{
		Msg->ReadQuaternion(quat);
	} else
	{
	}

	Msg->ReadVector(V);
	Msg->ReadVector(AV);

        

	if (IsInputPlayer)
	{
		Interpolating = false;
		float d, d2;
		d = D3DXVec3LengthSq(&WeVector(Location - tmpL));
		d2 = D3DXVec3LengthSq(&Velocity);
		if (d > d2*5 + 5) // treba teleportovati ako je presao 
		{
			MoveTo(tmpL);
			if (PhysicHandle)
				PhysicHandle->SetOrientationQuat(quat);
		}
	} else
	{
		if (PhysicHandle)
		{
			PhysicHandle->GetOrientationQuat(OrientationStarted);
			OrientationDestination = quat;
		}

		Velocity = V;
		AngularVelocity = AV;

		//InterpolationStartTime = WeTimer::Instance()->GetElapsedTimeF();
		//InterpolationTime = (float)WeClient::Instance()->GetPing() / 2000.0f;

		//LocationStarted = Location;
		//LocationDestination = tmpL;
		//if (LocationDestination != Location) Interpolating = true;
		//if (InterpolationTime < 0.1f) 
	//	{
			MoveTo(tmpL);			
			if (PhysicHandle)
				PhysicHandle->SetOrientationQuat(quat);

			//Interpolating = false;
	//	}
	}    
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  GetNetworkActor
//         Input Parametars :  ID
//         Output Parametars:  N/A
//         Comments         :  Returns actor with specified ID or NULL if actor not found
//
//////////////////////////////////////////////////////////////////////////
WeNetworkActor*	 WeNetworkActor::GetNetworkActor(WeNetworkActorID ID)
{
	for (NetworkActors.First(); !NetworkActors.End(); NetworkActors.Next())
	{
		if (NetworkActors.GetItem()->GetID() == ID)
		{
			return NetworkActors.GetItem();
		}
	}
	return NULL;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Client_GetPredictedLocation
//         Input Parametars :  StartingLocation
//         Output Parametars:  WeVector
//         Comments         :  This will predict new location accoring to latency 
//							   and current location. It will run physics for latency time to 
//								make correct prediction. For start it will not
//
//////////////////////////////////////////////////////////////////////////
WeVector WeNetworkActor::Client_GetPredictedLocation(WeVector &StartingLocation)
{
	WeVector Res;

	// TODO: Add physic prediction instead of this simple prediction

	if (iInterpolatingType == WEINTERPOLATION_MOVE)
	{
		// interpolating to predicted location if actor is going to stay on its course
//		Res = StartingLocation + (PredictionFactor*(StartingLocation - Location));
		Res = StartingLocation;  // TEMP REMOVE THIS AFTER THES
	}
	if (iInterpolatingType == WEINTERPOLATION_MOVEANDSTOP)
	{
		// interpolating to location we passes as param cause we need to interpolate to that 
		// location and stop
		Res = StartingLocation; 
		
	}
	return Res;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Client_InterpolateLocation
//         Input Parametars :  Destination, InterpolationTime
//         Output Parametars:  N/A
//         Comments         :  This will turn on interpolation from current location to Destination with interpolation time
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkActor::Client_InterpolateLocation(WeVector &Destination, float InterpolationTime)
{	
	//InterpolatingLocationStartTime = WeTimer::Instance()->GetElapsedTimeDW();
	//InterpolatingLocationTime = InterpolationTime;
	//LocationDestination = Destination;
	//LocationStarted = Location; // to now from what to interpolate

}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Client_UpdateInterpolations
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This will do interpolations set
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkActor::Client_UpdateInterpolations()
{
	// do location interpolation by finding correct velocity to give to object
	if (Interpolating)
	{        
		if (WeTimer::Instance()->GetElapsedTimeF() > InterpolationStartTime + InterpolationTime)
		{
			Velocity = WeVector(0, 0, 0);
			AngularVelocity = WeVector(0, 0, 0);
			Interpolating = false;
		} else
		{
			Velocity = ((LocationDestination - LocationStarted)) / InterpolationTime;
			WeQuaternion quat;
			D3DXQuaternionSlerp(&quat, &OrientationStarted, &OrientationDestination, (WeTimer::Instance()->GetElapsedTimeF()-InterpolationStartTime) / InterpolationTime);
			PhysicHandle->SetOrientationQuat(quat);			
		}		
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Client_ReadLocation
//         Input Parametars :  Message, InterpolationTime
//         Output Parametars:  N/A
//         Comments         :  This will read location from network msg, and set interpolation
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkActor::Client_ReadLocation(WeNetworkMessage *Msg, bool Interpolate)
{
	WeVector NewLocation;

		
	Msg->ReadVector(NewLocation);
	if (NewLocation == OldLocation)
	{
		iInterpolatingType = WEINTERPOLATION_MOVEANDSTOP;
	} else
	{
		iInterpolatingType = WEINTERPOLATION_MOVE;
	}
	OldLocation = NewLocation;

	// if we dont do prediction for this client just set his location
	if (!ClientDoesPrediction || !Interpolate)
	{
		//Velocity = NewLocation - Location;
		MoveTo(NewLocation);
		//Location = NewLocation;
		//if (PhysicHandle) PhysicHandle->SetLocation(NewLocation);
		iInterpolatingType = WEINTERPOLATION_NONE;
	} else
	{
		// set interpolation
		NewLocation = Client_GetPredictedLocation(NewLocation);
		// set interpolation

		// Just for testing
		//Client_InterpolateLocation(NewLocation, WeClient::Instance()->GetLatency());
		Client_InterpolateLocation(NewLocation, WeClient::Instance()->GetPing()/2.0f);
		
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Server_ReadyToSend
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  If you should send replication message at all
//
//////////////////////////////////////////////////////////////////////////
bool WeNetworkActor::Server_ReadyToSend(WeNetworkActor *Avatar)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Server_MAkeSpawnMessage
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This will fill Message with data for client
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkActor::Server_MakeSpawnMessage(WeNetworkMessage *Msg)
{
	Msg->WriteNetworkActorID(NetworkID);
	Msg->WriteVector(Location);
	Msg->WriteMatrix(TM);
	Msg->WriteFloat(Yaw);
	Msg->WriteFloat(Pitch);
	Msg->WriteFloat(Roll);
	Msg->WriteBool(Renderable);	
	Msg->WriteString(ModelName);
	Msg->WriteString(PhysicModelName);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Server_HandleSpawnMessage
//         Input Parametars :  Msg, IsInputPlayer
//         Output Parametars:  N/A
//         Comments         :  This will fill Message with data for client
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkActor::Client_HandleSpawnMessage(WeNetworkMessage *Msg, bool IsInputPlayer)
{
	NetworkID = Msg->ReadNetworkActorID();
	Msg->ReadVector(Location);
	Msg->ReadMatrix(TM);
	SetTransformation(TM);

	Yaw = Msg->ReadFloat();
	Pitch = Msg->ReadFloat();
	Roll = Msg->ReadFloat();
	Renderable = Msg->ReadBool();	
	ModelName = Msg->ReadString();
	PhysicModelName = Msg->ReadString();
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Server_Replicate
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This will fill Message with data for client
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkActor::Server_Replicate(WeNetworkActor *Avatar, WeNetworkMessage *Msg)
{
    // We should replicate location, and orientations 	
	Msg->WriteVector(Location);
	if (PhysicHandle)
	{	
		WeQuaternion quat;
		PhysicHandle->GetOrientationQuat(quat);
		Msg->WriteQuaternion(quat);
	}
	else
	{
		Msg->WriteFloat(Yaw);
		Msg->WriteFloat(Pitch);
		Msg->WriteFloat(Roll);
	}
	Msg->WriteVector(Velocity);
	Msg->WriteVector(AngularVelocity);
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Tick
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This will do interpolations and similar stuff
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkActor::Tick()
{
	// if this is server side this will do nothing
	Client_UpdateInterpolations(); // to get new location if prediction is on

	TickCounter++; 
	WeActor::Tick();
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Server_Replicate
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This will fill Message with data for client
//
//////////////////////////////////////////////////////////////////////////
WeNetworkActorID WeNetworkActor::GetID()
{
	return NetworkID;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Server_Replicate
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  This will fill Message with data for client
//
//////////////////////////////////////////////////////////////////////////
void WeNetworkActor::SetID(WeNetworkActorID ID)
{
	NetworkID = ID;
	TickCounter = ID; // some kind of incremental Tick. To ensure no NA is starting with same counter. 
}


