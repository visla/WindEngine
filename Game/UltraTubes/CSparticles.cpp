/*
	CSparticles.cpp
*/

#include <CSparticles.h>
#include <CConfig.h>
#include <Helper.h>
#include <WeWorld.h>
#include <WeAudioDevice.h>
#include <WeSound.h>
#include <WeModelManager.h>
#include <WeLoger.h>
#include <math.h>



CSparticles::CSparticles(WeWorld *aOwnerWorld, StringParam SparticleModelName, StringParam aBaseConfigName, int aMaxGroups, int aMaxGroupCount)
{
	AutoDestroy = false;
	AutoUpdateTransformations = false;

	SparticleModel = WeModelManager::Instance()->Load(SparticleModelName, false);
	BaseConfigName = aBaseConfigName;

	MaxGroupCount = aMaxGroupCount;
	MaxGroups = aMaxGroups;

	Groups = new CSparticleGroup[MaxGroups];
	for (int j = 0; j < MaxGroups; ++j)
	{
		Groups[j].Visible = false;
		Groups[j].Info = new CSparticle[aMaxGroupCount];
	}

	Init();

	LastSpawnTime = -1000000.0f;

	OwnerWorld = aOwnerWorld;
	OwnerWorld->AddActor(this);
}



CSparticles::~CSparticles()
{
	for (int j = 0; j < MaxGroups; ++j)
		delete [] Groups[j].Info;
	delete [] Groups;
}



void CSparticles::Init()
{
	EmitRate = GetConfigFloat(BaseConfigName + ".EmitRate", 1.0f);
	if (EmitRate <= 0.001f)
		EmitRate = 0.001f;

	EmitCount = GetConfigInt(BaseConfigName + ".EmitCount", 10);
	EmitCountDelta = GetConfigInt(BaseConfigName + ".EmitCountDelta", 5);

	ALifeTime = GetConfigFloat(BaseConfigName + ".LifeTime", 1.0f);
	LifeTimeDelta = GetConfigFloat(BaseConfigName + ".LifeTimeDelta", 1.0f);

	Speed = GetConfigFloat(BaseConfigName + ".Speed", 1.0f);
	SpeedDelta = GetConfigFloat(BaseConfigName + ".SpeedDelta", 1.0f);

	SizeFrom.x = GetConfigFloat(BaseConfigName + ".SizeFromX", 1.0f);
	SizeFromDelta.x = GetConfigFloat(BaseConfigName + ".SizeFromXDelta", 1.0f);

	SizeFrom.y = GetConfigFloat(BaseConfigName + ".SizeFromY", 1.0f);
	SizeFromDelta.y = GetConfigFloat(BaseConfigName + ".SizeFromYDelta", 1.0f);

	SizeFrom.z = GetConfigFloat(BaseConfigName + ".SizeFromZ", 1.0f);
	SizeFromDelta.z = GetConfigFloat(BaseConfigName + ".SizeFromZDelta", 1.0f);

	SizeTo.x = GetConfigFloat(BaseConfigName + ".SizeToX", 1.0f);
	SizeToDelta.x = GetConfigFloat(BaseConfigName + ".SizeToXDelta", 1.0f);

	SizeTo.y = GetConfigFloat(BaseConfigName + ".SizeToY", 1.0f);
	SizeToDelta.y = GetConfigFloat(BaseConfigName + ".SizeToYDelta", 1.0f);

	SizeTo.z = GetConfigFloat(BaseConfigName + ".SizeToZ", 1.0f);
	SizeToDelta.z = GetConfigFloat(BaseConfigName + ".SizeToZDelta", 1.0f);

	FollowFactor = GetConfigFloat(BaseConfigName + ".FollowFactor", 0.0f);

	Spread = GetConfigFloat(BaseConfigName + ".Spread", 1.0f);

	Gravity.x = GetConfigFloat(BaseConfigName + ".GravityX", 0.0f);
	Gravity.y = GetConfigFloat(BaseConfigName + ".GravityY", -1.0f);
	Gravity.z = GetConfigFloat(BaseConfigName + ".GravityZ", 0.0f);
}



bool CSparticles::InitGroup(int iGroup, WeActor *aOwner, WeVector &aSpawnPos, WeVector &aSpawnDir)
{
	CSparticleGroup *Group = &Groups[iGroup];

	Group->Count = Rnd(EmitCount - EmitCountDelta, EmitCount + EmitCountDelta);
	if (Group->Count < 0)
		return false;
	if (Group->Count > MaxGroupCount)
		Group->Count = MaxGroupCount;

	Group->BornTime = WeTimer::Instance()->GetElapsedTimeF();

	Group->Owner = aOwner;
	Group->SpawnPos = aSpawnPos;
	Group->RelSpawnPos = Group->SpawnPos - Group->Owner->Location;
	D3DXVec3Normalize(&Group->SpawnDir, &aSpawnDir);

	Group->LifeTime = 0.0f;

	for (int i = 0; i < Group->Count; ++i)
	{
		Group->Info[i].Visible = true;

		Group->Info[i].LifeTime = RndF(ALifeTime - LifeTimeDelta, ALifeTime + LifeTimeDelta);
		if (Group->Info[i].LifeTime < 0.0f)
			Group->Info[i].LifeTime = 0.0f;
		if (Group->Info[i].LifeTime > Group->LifeTime)
			Group->LifeTime = Group->Info[i].LifeTime;

		Group->Info[i].SizeFrom.x = RndF(SizeFrom.x - SizeFromDelta.x, SizeFrom.x + SizeFromDelta.x);
		Group->Info[i].SizeFrom.y = RndF(SizeFrom.y - SizeFromDelta.y, SizeFrom.y + SizeFromDelta.y);
		Group->Info[i].SizeFrom.z = RndF(SizeFrom.z - SizeFromDelta.z, SizeFrom.z + SizeFromDelta.z);

		Group->Info[i].SizeTo.x = RndF(SizeTo.x - SizeToDelta.x, SizeTo.x + SizeToDelta.x);
		Group->Info[i].SizeTo.y = RndF(SizeTo.y - SizeToDelta.y, SizeTo.y + SizeToDelta.y);
		Group->Info[i].SizeTo.z = RndF(SizeTo.z - SizeToDelta.z, SizeTo.z + SizeToDelta.z);


		float aSpeed;
		aSpeed = RndF(Speed - SpeedDelta, Speed + SpeedDelta);

		WeVector V;
		V = aSpeed * Group->SpawnDir;
		V.x += RndF(-Spread, Spread);
		V.y += RndF(-Spread, Spread);
		V.z += RndF(-Spread, Spread);

		Group->Info[i].Speed = D3DXVec3Length(&V);
		D3DXVec3Normalize(&Group->Info[i].SpawnDir, &V);
	}

	return true;
}



bool CSparticles::IntersectFrustum(WeFrustum *frustum)
{
	return true;
}



void CSparticles::Tick()
{
	static WeVector V0 = WeVector(0.0f, 0.0f, 0.0f);
	static WeVector X0 = WeVector(1.0f, 0.0f, 0.0f);
	static WeVector Y0 = WeVector(0.0f, 1.0f, 0.0f);

	WeVector V, D, Up;
	WeMatrix MScale, MDir, M;
	WeVector Loc, Dir, Up1;

	float LifeFactor;
	WeVector G;

	CSparticleGroup *Group;

	for (int j = 0; j < MaxGroups; ++j)
	{
		Group = &Groups[j];
		if (!Group->Visible) continue;

		float Age;
		Age = WeTimer::Instance()->GetElapsedTimeF() - Group->BornTime;

		if (Age > Group->LifeTime)
		{
			Group->Visible = false;
			continue;
		}

		for (int i = 0; i < Group->Count; ++i)
		{
			if (Age > Group->Info[i].LifeTime)
			{
				Group->Info[i].Visible = false;
				continue;
			}

			Group->Info[i].PrevTM = Group->Info[i].TM;

			LifeFactor = Age;
			if (Group->Info[i].LifeTime > 0.001f)
				LifeFactor /= Group->Info[i].LifeTime;

			Loc = Group->SpawnPos - Age * Group->Info[i].Speed * Group->Info[i].SpawnDir +
				FollowFactor * (Group->Owner->Location + Group->RelSpawnPos - Group->SpawnPos);
			Dir = Group->Info[i].SpawnDir;

			G = Gravity * Age;

			Loc.x += G.x * abs(G.x);
			Loc.y += G.y * abs(G.y);
			Loc.z += G.z * abs(G.z);

			D = Dir - G / 2.0f;
			D3DXVec3Normalize(&D, &D);

			D3DXVec3Cross(&V, &Y0, &D);
			if (D3DXVec3LengthSq(&V) < 0.01f)
				Up1 = X0;
			else
			{
				D3DXVec3Normalize(&V, &V);
				D3DXVec3Cross(&Up1, &D, &V);
			}

			D3DXMatrixLookAtLH(&M, &D, &V0, &Up1);
			D3DXMatrixInverse(&MDir, NULL, &M);

			D3DXMatrixScaling(&MScale,
				Group->Info[i].SizeFrom.x + LifeFactor * (Group->Info[i].SizeTo.x - Group->Info[i].SizeFrom.x),
				Group->Info[i].SizeFrom.y + LifeFactor * (Group->Info[i].SizeTo.y - Group->Info[i].SizeFrom.y),
				Group->Info[i].SizeFrom.z + LifeFactor * (Group->Info[i].SizeTo.z - Group->Info[i].SizeFrom.z));

			D3DXMatrixMultiply(&Group->Info[i].TM, &MScale, &MDir);

			Group->Info[i].TM._41 = Loc.x;
			Group->Info[i].TM._42 = Loc.y;
			Group->Info[i].TM._43 = Loc.z;
		}
	}

	WeActor::Tick();
}



void CSparticles::OnRender(WeCamera* Cam)
{
	CSparticleGroup *Group;

	for (int j = 0; j < MaxGroups; ++j)
	{
		Group = &Groups[j];
		if (!Group->Visible) continue;

		for (int i = 0; i < Group->Count; ++i)
		{
			if (!Group->Info[i].Visible) continue;

			SparticleModel->PrevTM = Group->Info[i].PrevTM;
			SparticleModel->TM = Group->Info[i].TM;
			SparticleModel->Draw(World, Cam, this);
		}
	}
}



void CSparticles::destroy()
{
}



void CSparticles::Emit(WeActor *aOwner, WeVector &aSpawnPos, WeVector &aSpawnDir)
{
	float SpawnTime;
	float Time;

	SpawnTime = 1.0f / EmitRate;
	Time = WeTimer::Instance()->GetElapsedTimeF();

	if (Time - LastSpawnTime > SpawnTime)
	{
		for (int j = 0; j < MaxGroups; ++j)
		{
			if (!Groups[j].Visible)
			{
				Groups[j].Visible = InitGroup(j, aOwner, aSpawnPos, aSpawnDir);
				break;
			}
		}

		LastSpawnTime = Time;
	}
}
