/*
	File: CTuberSparks.cpp
	Author: Goran Bogdanovic
*/


#include <CTuberSparks.h>
#include <CTuber.h>
#include <WeLight.h>
#include <WeParticleSystem.h>
#include <WeAudioDevice.h>
#include <WeSound.h>
#include <WeTimer.h>
#include <CMainGamePhase.h>

#include <WeTrail.h>
#include <WeMaterialManager.h>
#include <WePhysics.h>
#include <CollisionGroups.h>
#include <Helper.h>


#define	ACTIVITY_TIME	0.1f


WeParticleSystem	*CTuberSparks::StaticPS		= NULL;
WeLight				*CTuberSparks::StaticLight	= NULL;

CTuberSparks::CTuberSparks(CTuber *MyOwner, WeMatrix &RelMatrix, WeVector &HitNormal)
{
	Owner = MyOwner;
	Update(RelMatrix, HitNormal);
	Owner->GetWorld()->AddActor(this);

	//if (!StaticLight)
	//{
	//	StaticLight = new WeLight();
	//	WeLightState ls1, ls2;
	//	ls1.Range = 20.0f;
	//	ls1.Diffuse = WeColor(1.0f, 1.0f, 1.0f, 1.0f);
	//	ls1.Intensity = 0.3f;
	//	ls2.Range = 0.0f;
	//	StaticLight->AddKeyState(ls1, 0.03f);
	//	StaticLight->AddKeyState(ls2, 0.03f);
	//	StaticLight->IsDynamic = true;
	//	StaticLight->AffectDynamicActors = true;
	//}

	Sparks = new WeParticleSystem();
	StaticPS->Clone(Sparks);
	Sparks->Initialize();

	//Sparks->Renderable = false;
	Sparks->Properties.ParticleDirection = -Owner->Direction + HitNormal;	
	Owner->GetWorld()->AddActor(Sparks);
	//Sparks = NULL;

	//Light = new WeLight();
	//StaticLight->Clone(Light);
	//Light->Location = Location;
	//Owner->GetWorld()->AddActor(Light);
	Light = NULL;

	char alphazulu[100];
	sprintf(alphazulu, "Sounds.Tuber.Scratch%d", Rnd(1, 3));
	SoundScratch = WeAudioDevice::Instance()->LoadSoundAuto(String(alphazulu), true);
	if (SoundScratch)
		SoundScratch->SetMaxDistance(100.0f);

	Active = true;
	DeactivationTimer = 0.0f;
}

CTuberSparks::~CTuberSparks()
{
	if (SoundScratch)
	{
		SoundScratch->Stop();
		SAFE_DELETE(SoundScratch);
	}

	if (!CMainGamePhase::Instance->Map.IsUnloading())
	{
		if (Light)
			Light->LifeTime = 0.0001f;
		if (Sparks)
			Sparks->LifeTime = 0.0001f;
		Owner->Sparks = NULL;
	}
}

void CTuberSparks::Tick()
{
	DeactivationTimer += WeTimer::Instance()->GetDeltaTimeF();
	bool NewActiveState = (DeactivationTimer < ACTIVITY_TIME);

	Sparks->Location = Owner->Location + RelativeVector + HitNormal*0.1f;
	Sparks->Properties.ParticleDirection = Owner->Direction + HitNormal;
	Sparks->Properties.Spread = 0.2f;
	Sparks->Properties.StartVelocity = Owner->CurrentSpeed;
	Sparks->Properties.StartVelocityRandomness = Owner->CurrentSpeed / 3.0f;

	if (Sparks->Properties.StartVelocity < 2.0f) Sparks->Properties.StartVelocity = 2.0f;

	if (SoundScratch)
	{
		SoundScratch->Update(Owner->Location, WeVector(0.0f, 0.0f, 0.0f));
	}


	if (NewActiveState)
	{
		if (SoundScratch && !SoundScratch->IsPlaying())
			SoundScratch->Play(true);
		//WeMatrix MyMatrix, OwnerMatrix;
		//Owner->GetTransformation(OwnerMatrix);

		//D3DXMatrixMultiply(&MyMatrix, &OwnerMatrix, &RelativeTransMatrix);
		//SetTransformation(MyMatrix);

		/*if (TrailSparks.GetItemsCount() < 97)
		{
			for (int i = 0; i < 3; i++)
			{
				WeVector RandomVector = WeVector(-0.5f, -0.5f, -0.5f) + WeVector((float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f);
				D3DXVec3Normalize(&RandomVector, &RandomVector);
				WeVector SparkDirection = -Owner->Direction + (RandomVector / 3.0f);
				D3DXVec3Normalize(&SparkDirection, &SparkDirection);

				CTrailSpark *NewSpark = new CTrailSpark(this, Location, SparkDirection);
				NewSpark->LifeTime = ACTIVITY_TIME;
				TrailSparks.PushBack(NewSpark);
			}
		}*/

		//Light->Location = Location;


	}
	else if (Active)
	{
		SetActive(false);
	}
	
	Active = NewActiveState;

	WeActor::Tick();
}


void CTuberSparks::SetActive(bool Activate)
{
	if (!Active && Activate)
	{
		Sparks->Properties.EmitRate = StaticPS->Properties.EmitRate;
		//Light = new WeLight();
		//StaticLight->Clone(Light);
		//Light->Location = Location;
		//Owner->GetWorld()->AddActor(Light);
	}

	if (Active && !Activate)
	{
		Sparks->Properties.EmitRate = 0;
		//Light->LifeTime = 0.00001f;
		if (SoundScratch)
		{
			SoundScratch->Stop();
		}
	}
	else
	{
		DeactivationTimer = 0.0f;
	}

	Active = Activate;
}

bool CTuberSparks::IsValid()
{
	return Active;
}

void CTuberSparks::Update(WeMatrix &NewMatrix, WeVector &NewNormal)
{
	RelativeTransMatrix = NewMatrix;
	RelativeVector.x = RelativeTransMatrix._41;
	RelativeVector.y = RelativeTransMatrix._42;
	RelativeVector.z = RelativeTransMatrix._43;
	HitNormal = NewNormal;
}




CTrailSpark::CTrailSpark(CTuberSparks *WhereFrom, WeVector &WhereIs, WeVector &WhereTo)
{
	Location = WhereIs;
	FixedDirection = WhereTo;
	Parent = WhereFrom;
	SetWorld(WhereFrom->GetWorld());

	Trail = new WeTrail();
	Trail->MaxSegments = 8;
	Trail->Location = Location;
	Trail->StartFadeFactor = 0.5f;
	Trail->SegmentCreationTime = 0.01f;
	Trail->SegmentWidth = 0.025f;
	Trail->SegmentColor.r = 1.0f;
	Trail->SegmentColor.g = 1.0f;
	Trail->SegmentColor.b = 0.0f;
	Trail->Material = WeMaterialManager::Instance()->Load("Materials.Trails.Tuber");
	Trail->Initialize();
	GetWorld()->AddActor(Trail);

	PhysicHandle = WePhysics::Instance()->CreateBox(this, Location, WeVector(0.1f, 0.1f, 0.1f), COLLISION_GROUP_SHELL, 0.1f, false);
}

CTrailSpark::~CTrailSpark()
{
	if (CMainGamePhase::Instance->Map.IsUnloading()) return;

	for (Parent->TrailSparks.First(); !Parent->TrailSparks.End(); Parent->TrailSparks.Next())
		if (Parent->TrailSparks.GetItem() == this)
		{
			Parent->TrailSparks.Delete();
			break;
		}

	Trail->LifeTime = (float)((Trail->MaxSegments / 2) * Trail->SegmentCreationTime);
}

void CTrailSpark::Tick()
{
	Trail->Location = Location;

	PhysicHandle->SetLinearVelocity((15.0f * FixedDirection));

	WeActor::Tick();
}
