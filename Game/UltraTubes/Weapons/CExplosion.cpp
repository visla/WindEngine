/*
		File: CExplosion.cpp
		Author: Tomislav Kukic
		Date: 23.06.2006
		Version: 1.0
*/




#include <CExplosion.h>
#include <ActorTags.h>
#include <CTuber.h>
#include <CUberText.h>
#include <CMainGameInterface.h>
#include <WeModelManager.h>
#include <WeMesh.h>
#include <WeAudioDevice.h>
#include <WeSound.h>
#include <CMainGamePhase.h>


#define FRAG_SCORE	250

CExplosion::CExplosion()
{
	Type = ACTORTYPE_EXPLOSION;
}

CExplosion::~CExplosion()
{
}

void CExplosion::Tick()
{
	WeActor::Tick();
}


void CExplosion::Affect(CTuber *Victim)
{
	if (Victim->Dead) return;

	float TotalDamage = Damage * Pyromaniac->StrengthMultiplier;

	Victim->LastAttacker = Pyromaniac;
	Victim->LastDamageWeaponID = Creator;
	float RealDamage = Victim->TakeDamage(TotalDamage);

	LogicCalc(Victim, RealDamage);
}

void CExplosion::Affect(CTuber *Victim, float SpecificDamage)
{
	if (Victim->Dead) return;

	float TotalDamage = SpecificDamage * Pyromaniac->StrengthMultiplier;

	Victim->LastAttacker = Pyromaniac;
	Victim->LastDamageWeaponID = Creator;
	float RealDamage = Victim->TakeDamage(TotalDamage);

	LogicCalc(Victim, RealDamage);
}

void CExplosion::LogicCalc(CTuber *Victim, float DamageTaken)
{
	Pyromaniac->LastEnemyHit = Victim;

	if ( (!Victim->Invulnerable) && (Victim->FSDamageNotifyTimer >= FS_DAMAGE_NOTIFY_SPREAD) )
	{
		Victim->FSDamageNotifyTimer = 0.0f;
		Victim->FSDamageNotify = true;
		Victim->FSDamageNotifyReset = true;
	}

	if ( (Pyromaniac == Victim) || Pyromaniac->IsFriendWith(Victim) )
	{
		Pyromaniac->Score -= (int)DamageTaken;
		if (Pyromaniac->Score < 0) Pyromaniac->Score = 0;
		if (Victim->Dead)
		{
			Pyromaniac->Kills--;
			Pyromaniac->Score -= FRAG_SCORE;
		}
	}
	else
	{
		Pyromaniac->Score += (int)DamageTaken;
		if (Victim->Dead)
		{
			Pyromaniac->Kills++;
			Pyromaniac->Score += FRAG_SCORE;
		}
	}

	if (Pyromaniac->Kills < 0) Pyromaniac->Kills = 0;
	if (Pyromaniac->Score < 0) Pyromaniac->Score = 0;
}


void CExplosion::AddExplosiveRefraction(float Size, float Life)
{
	static float BasicRefractionParam4 = 0.0f;

	WeActor *Refraction = new WeActor();
	Refraction->Model = WeModelManager::Instance()->Load("Weapons.Explosions.ExplosionRefraction01");
	Refraction->Model->Mesh->Materials[0].First();

	if (Life > 0.0f)
	{
		Refraction->LifeTime = Life;
		if (BasicRefractionParam4 == 0.0f)
			BasicRefractionParam4 = Refraction->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4;
		Refraction->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4 = Refraction->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / Refraction->LifeTime;
	}
	else
	{
		if (BasicRefractionParam4 > 0.0f)
			Refraction->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4 = BasicRefractionParam4;
		Refraction->LifeTime = (Refraction->Model->Mesh->Materials[0].GetItem()->TextureAnimParam3 / Refraction->Model->Mesh->Materials[0].GetItem()->TextureAnimParam4);
	}

	//WeVector Distance = (Location - Pyromaniac->Location);
	//D3DXVec3Normalize(&Distance, &Distance);
	//Refraction->Location = Location + (4.0f * Distance);
	Refraction->Location = Location;
	Refraction->SetScaling(WeVector(Size, Size, Size));
	Pyromaniac->GetWorld()->AddActor(Refraction);
}


void CExplosion::AddDistantExplosion()
{
	float Distance = D3DXVec3LengthSq(&(CMainGamePhase::Instance->InputPlayer->Location - Location));

	if (Distance < 250000.0f) return;

	CMainGameInterface::Instance->AddDistantExplosion = true;
}
