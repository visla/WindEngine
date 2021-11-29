/*
       File: CPickUp.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/


#include <CActorTypesSupplier.h>
#include <CPickUp.h>
#include <WePhysics.h>
#include <WePhysicHandle.h>
#include <WeTimer.h>
#include <CollisionGroups.h>
#include <WeAudioDevice.h>
#include <WeSound.h>

#define  PICKUP_RADIUS	4.0f


CPickUp::CPickUp()
{
	ModelName = "PickUps.Models.Mine";
	ShouldTerminate = false;
	Active = true;
	RespawnTime = 30.0f;
	LevitationParam = (float)(rand()%100);
	NewPreset = 0;
	Timer = 0.0f;
	LockFactor = 0.0f;
	MaterialPreset = PickupMatPresetNormal;
}



void CPickUp::Update(CTuber *tuber)
{
}



void CPickUp::Initialize()
{	
	if (InEditor)
	{
		PhysicModel.Type = PHYSICMODEL_STATIC_SHAPE;
	}
	WeActor::Initialize();
	if(InEditor) return;

	if(PhysicHandle) SAFE_DELETE(PhysicHandle);
	PhysicHandle = WePhysics::Instance()->CreateSphere(this, Location, PICKUP_RADIUS, COLLISION_GROUP_PICKUP, 0.1f, false, WETRIGGER_ON_ENTER);
	PhysicHandle->SetDisableGravity(true);

	OriginalPosition = Location;

	RespawnSound = WeAudioDevice::Instance()->LoadSoundAuto("Sounds.Pickup.PickupRespawn", true);
	if (RespawnSound)
		RespawnSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));

	SetTransformation(TM);
}



void CPickUp::OnTrigger(WeActor* ByObject)
{
	if(RespawnTime > 0.0f)
	{
		switch (Type)
		{
		case PICKUPTYPE_COINSMALL:
		case PICKUPTYPE_COINMEDIUM:
		case PICKUPTYPE_COINLARGE:
			Renderable = false;
			break;
		default:
			MaterialPreset = PickupMatPresetAlpha;
			break;
		}

		Active = false;
		PhysicHandle->SetResponse(false);
		//LevitationParam = 0.0f;
		//PhysicHandle->SetLinearVelocity(WeVector(0.0f,0.0f,0.0f));
		//PhysicHandle->SetAngularVelocity(WeVector(0.0f,0.0f,0.0f));
	}
	else if (RespawnTime < 0.0f)
	{
		LifeTime = 0.0001f;
	}
}


void CPickUp::Tick()
{
	if(!Active)
	{
		Timer += WeTimer::Instance()->GetDeltaTimeF();

		if (Timer >= RespawnTime)
		{
			switch (Type)
			{
			case PICKUPTYPE_COINSMALL:
			case PICKUPTYPE_COINMEDIUM:
			case PICKUPTYPE_COINLARGE:
				Renderable = true;
				break;
			default:
				MaterialPreset = PickupMatPresetNormal;
				break;
			}

			Active = true;
			PhysicHandle->SetResponse(true);
			Timer = 0.0f;
			Location = OriginalPosition;

			if (RespawnSound)
			{
				RespawnSound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
				RespawnSound->Play(false);
			}
		}
	}
	else
	{
		MaterialPreset = NewPreset;
	}

	float Time = WeTimer::Instance()->GetDeltaTimeF();
	WeVector Vel, AVel;

	LevitationParam += Time;
	Vel = WeVector(0.0f, (1.0f * sin(4 * LevitationParam)), 0.0f);
	AVel = WeVector(0.0f, D3DX_PI / 2.0f, 0.0f);

	PhysicHandle->SetLinearVelocity(Vel);
	PhysicHandle->SetAngularVelocity(AVel);

	WeActor::Tick();
}



void CPickUp::Serialize(WeFileStream &FS)
{
	WeActor::Serialize(FS);
	FS.WriteBool(Active);
	FS.WriteFloat(RespawnTime);	
}



void CPickUp::Deserialize(WeFileStream &FS)
{
	WeActor::Deserialize(FS);
	Active = FS.ReadBool();
	RespawnTime = FS.ReadFloat();	
}



