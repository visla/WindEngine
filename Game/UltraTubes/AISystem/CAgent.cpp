/*
	Filename:	CAgent.cpp
	Author:		Goran Bogdanovic
*/

#include <CAgent.h>
#include <CAgentMotorics.h>
#include <CAgentMotoricsStandardTube.h>
#include <CAgentMotoricsNodeDirection.h>
#include <CAgentMotoricsEvasiveMine.h>
#include <CAgentArmory.h>
#include <CAgentArmoryBolts.h>
#include <CAgentArmoryClassicMines.h>
#include <CAgentArmoryHomingMissiles.h>
#include <CAgentArmoryPositionMissiles.h>
#include <CAgentArmoryPositionMines.h>
#include <CAgentArmoryNukeMine.h>
#include <CGamePlayerConfig.h>
#include <CTuber.h>
#include <CPlayGamePhase.h>
#include <WePathNode.h>
#include <CWeaponFabric.h>
#include <WeTimer.h>
#include <CWeapon.h>
#include <CAIKernel.h>
#include <WeClient.h>


#define BUMP_TEST_DISTANCE_SQ	150.0f
#define	BUMP_TEST_TIME			1.0f


CAgent::CAgent(CGamePlayerConfig *OwnerConfig)
{
	AvatarConfig = OwnerConfig;
	Motorics = NULL;
	MotoricsStandardTube = new CAgentMotoricsStandardTube(this);
	MotoricsNodeDirection = new CAgentMotoricsNodeDirection(this);
	MotoricsEvasiveMine = new CAgentMotoricsEvasiveMine(this);
	ArmoryPrimary = NULL;
	ArmorySecondary = NULL;
	ArmoryBolts = new CAgentArmoryBolts(this);
	ArmoryClassicMines = new CAgentArmoryClassicMines(this);
	ArmoryHomingMissiles = new CAgentArmoryHomingMissiles(this);
	ArmoryPositionMissiles = new CAgentArmoryPositionMissiles(this);
	ArmoryPositionMines = new CAgentArmoryPositionMines(this);
	ArmoryNukeMine = new CAgentArmoryNukeMine(this);

	MotoricsSelectedTimer = 0.0f;
	MotoricsSelectedTime = 0.0f;
	MotoricsSelected = NULL;

	ForcedMotorics = NULL;

	ArmoryEnabled = true;

	BumpTimer = 0.0f;
	PreviousBumpLocation = WeVector(0.0f, 0.0f, 0.0f);

	DeathOccurred = false;
}

CAgent::~CAgent()
{
	AvatarConfig = NULL;
	Avatar = NULL;
	Motorics = NULL;
	MotoricsSelected = NULL;
	SAFE_DELETE(MotoricsStandardTube);
	SAFE_DELETE(MotoricsNodeDirection);
	SAFE_DELETE(MotoricsEvasiveMine);
	ArmoryPrimary = NULL;
	ArmorySecondary = NULL;
	SAFE_DELETE(ArmoryBolts);
	SAFE_DELETE(ArmoryClassicMines);
	SAFE_DELETE(ArmoryHomingMissiles);
	SAFE_DELETE(ArmoryPositionMissiles);
	SAFE_DELETE(ArmoryPositionMines);
	SAFE_DELETE(ArmoryNukeMine);
}


void CAgent::Tick()
{
	if (!AvatarConfig->User) return;
	if (!AvatarConfig->User->Tuber) return;
	Avatar = AvatarConfig->User->Tuber;
	if (!Avatar) return;

	if (GetDifficulty() == AgentDifficultyGuru)
	{
		Avatar->MaxSpeed = Avatar->Description.MaxSpeed * 1.03f;
		Avatar->TurboMaxSpeed = Avatar->Description.MaxSpeed * 1.236f;
	}

	if (!ForcedMotorics)
	{
		if (CPlayGamePhase::Instance->Status != PLAYGAME_RACE ||
			Avatar->Dead ||
			Avatar->RaceFinished)
		{
			DeathOccurred = Avatar->Dead;

			TickEmpty();
			return;
		}
	}

	CInputMessage *Msg = new CInputMessage();

	if (CPlayGamePhase::Instance->GetAIKernel()->GetMotoricsStatus())
		ChooseMotorics();
	if (Motorics)
	{
		Motorics->Tick(Msg);
		Motorics = NULL;
	}

	Avatar->ProcessInput(Msg);

	if (ArmoryEnabled && CPlayGamePhase::Instance->GetAIKernel()->GetArmoryStatus())
		ChooseArmories();
	if (ArmoryPrimary)
	{
		ArmoryPrimary->Tick(Msg);
		ArmoryPrimary = NULL;
	}
	if (ArmorySecondary)
	{
		ArmorySecondary->Tick(Msg);
		ArmorySecondary = NULL;
	}

	Avatar->InputMessage = *Msg;
	WeClient::Instance()->SendDeltaCommand(Avatar);

	SAFE_DELETE(Msg);
}

void CAgent::TickEmpty()
{
	CInputMessage *Msg = new CInputMessage();
	AvatarConfig->User->Tuber->ProcessInput(Msg);
	SAFE_DELETE(Msg);
}

void CAgent::ChooseMotorics()
{
	if (ForcedMotorics)
	{
		MotoricsStandardTube->SetForcedFactor(0.0f);
		Motorics = ForcedMotorics;
		return;
	}

	float time = WeTimer::Instance()->GetDeltaTimeF();

	// use time selected motorics
	if (MotoricsSelected)
	{
		MotoricsSelectedTimer += time;
		if (MotoricsSelectedTimer < MotoricsSelectedTime)
		{
			Motorics = MotoricsSelected;
			return;
		}
		else
		{
			MotoricsSelected = NULL;
		}
	}

	MotoricsStandardTube->SetForcedFactor(0.0f);

	if (DeathOccurred || TestRightDirection() || TestWallBump())
	{
		float Duration = 2.0f;
		if (DeathOccurred)
			Duration = 0.5f;
		
		DeathOccurred = false;
		SelectMotorics(MotoricsNodeDirection, Duration);
		MotoricsNodeDirection->SetDefaultSettings();
		Motorics = MotoricsSelected;
		return;
	}

	CWeapon *DangerMine = NULL;
	DangerMine = TestMineProximity();
	if (DangerMine)
	{
		if ((DangerMine->WeaponID == WEAPONTYPE_BLURMINE) ||
			(DangerMine->WeaponID == WEAPONTYPE_SMOKEMINE))
		{
			MotoricsStandardTube->SetForcedFactor(0.05f);
			SelectMotorics(MotoricsStandardTube, 2.0f);
			Motorics = MotoricsSelected;
		}
		else
		{
			MotoricsEvasiveMine->SetMineLocation(DangerMine->Location);
			SelectMotorics(MotoricsEvasiveMine, 0.1f);
			Motorics = MotoricsSelected;
		}

		return;
	}

	if (TestNodeForcedMotorics()) return;

	Motorics = MotoricsStandardTube;
}

void CAgent::ChooseArmories()
{
	WEAPONTYPE IDPrimary, IDSecondary;

	if (!CGameSettings::ServerInstance->GetWeapons())
		return;	// get out if weapons are disabled

	if (Avatar->PrimaryWeaponAddonID != WEAPONTYPE_NONE)
		IDPrimary = Avatar->PrimaryWeaponAddonID;
	else
		IDPrimary = Avatar->PrimaryWeaponID;

	if (Avatar->SecondaryWeaponAddonID != WEAPONTYPE_NONE)
		IDSecondary = Avatar->SecondaryWeaponAddonID;
	else
		IDSecondary = Avatar->SecondaryWeaponID;

	if (CWeaponFabric::Instance->AbleToFire(Avatar, true) != WEAPONTYPE_NONE)
	{
		switch (IDPrimary)
		{
		case WEAPONTYPE_STANDARDLASER:
		case WEAPONTYPE_DOUBLELASER:
		case WEAPONTYPE_QUADLASER:
		case WEAPONTYPE_QUANTUMLASER:
			ArmoryPrimary = ArmoryBolts;
			break;

		case WEAPONTYPE_STANDARDROCKET:
		case WEAPONTYPE_MINIROCKETS:
			ArmoryPrimary = ArmoryHomingMissiles;
			break;

		case WEAPONTYPE_SLOWINGROCKET:
		case WEAPONTYPE_TRANSLOCATOR:
			ArmoryPrimary = ArmoryPositionMissiles;
			break;
		}
	}

	if (CWeaponFabric::Instance->AbleToFire(Avatar, false) != WEAPONTYPE_NONE)
	{
		switch (IDSecondary)
		{
		case WEAPONTYPE_STANDARDMINE:
		case WEAPONTYPE_ELECTRICMINE:
		case WEAPONTYPE_HOMINGMINE:
		case WEAPONTYPE_INVISIBLEMINE:
		case WEAPONTYPE_MOVINGMINE:
		case WEAPONTYPE_PROXIMITYMINE:
			ArmorySecondary = ArmoryClassicMines;
			break;

		case WEAPONTYPE_BLURMINE:
		case WEAPONTYPE_GRAVITYMINE:
		case WEAPONTYPE_SLOWINGMINE:
		case WEAPONTYPE_SMOKEMINE:
			ArmorySecondary = ArmoryPositionMines;
			break;

		case WEAPONTYPE_SUPERNUKEMINE:
			ArmorySecondary = ArmoryNukeMine;
			break;
		}
	}
}


float CAgent::QuickRay(WeVector &From, WeVector &Dir, float Dist, WeShapesType TType)
{
	WeRaycastHit Hit;
	if (WePhysics::Instance()->RaycastClosest(From, Dir, TType, Hit, 7, Dist))
	{
		return Hit.Distance;
	}
	else
	{
		return -1.0f;
	}
}


AgentDifficulty CAgent::GetDifficulty()
{
	switch (AvatarConfig->Status)
	{
	case PCFG_AIEASY:
		return AgentDifficultyEasy;
	case PCFG_AIMEDIUM:
		return AgentDifficultyMedium;
	case PCFG_AIHARD:
		return AgentDifficultyGuru;
	default:
		return AgentDifficultyMedium;
	}
}


void CAgent::SelectMotorics(CAgentMotorics *FixedTimeMotorics, float Duration)
{
	MotoricsSelected = FixedTimeMotorics;
	MotoricsSelectedTimer = 0.0f;
	MotoricsSelectedTime = Duration;
}


// test for alignment with path nodes
bool CAgent::TestRightDirection()
{
	WePathNode *Node = Avatar->GetWorld()->FindClosestDirectionPathNode(Avatar->Location);

	if (Node)
	{
		if (Avatar->WrongWay) //(D3DXVec3Dot(&Node->Direction, &Avatar->Direction) < 0.2f)
		{
			return true;
		}
	}

	return false;
}

// test for in place bump
bool CAgent::TestWallBump()
{
	float time = WeTimer::Instance()->GetDeltaTimeF();

	if ( D3DXVec3LengthSq(&(Avatar->Location - PreviousBumpLocation)) < BUMP_TEST_DISTANCE_SQ )
	{
		BumpTimer += time;
		if (BumpTimer >= BUMP_TEST_TIME)
		{
			BumpTimer = 0.0f;
			return true;
		}
	}
	else
	{
		PreviousBumpLocation = Avatar->Location;
		BumpTimer = 0.0f;
	}

	return false;
}

CWeapon* CAgent::TestMineProximity()
{
	// easy AI does not evade mines
	if (GetDifficulty() == AgentDifficultyEasy)
		return false;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (CGameSettings::ServerInstance->tmpPlayers[i].User &&
			CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber &&
			CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->DeployedMines.GetItemsCount() > 0
			)
		{
			CTuber *Gari = CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber;
			for (Gari->DeployedMines.First(); !Gari->DeployedMines.End(); Gari->DeployedMines.Next())
			{
				switch (Gari->DeployedMines.GetItem()->WeaponID)
				{
				case WEAPONTYPE_INVISIBLEMINE:
					if (GetDifficulty() != AgentDifficultyGuru)	// only hard AI can see this one
						break;
				case WEAPONTYPE_STANDARDMINE:
				case WEAPONTYPE_SMOKEMINE:
				case WEAPONTYPE_PROXIMITYMINE:
				case WEAPONTYPE_SLOWINGMINE:
				case WEAPONTYPE_HOMINGMINE:
				case WEAPONTYPE_GRAVITYMINE:
				case WEAPONTYPE_BLURMINE:
				case WEAPONTYPE_MOVINGMINE:
				case WEAPONTYPE_ELECTRICMINE:
					float MineDistSq = D3DXVec3LengthSq(&(Avatar->Location - Gari->DeployedMines.GetItem()->Location));
					if ( (Gari->DeployedMines.GetItem()->ObjectAge >= Gari->DeployedMines.GetItem()->GetTimeToActivate(Gari->DeployedMines.GetItem()->WeaponID)) &&
						 (MineDistSq < 0.4f * (Avatar->TurboMaxSpeed * Avatar->TurboMaxSpeed)) )
					{
						if ( GetNodeDistance(Avatar->GetWorld()->FindNextDirectionPathNode(Gari->DeployedMines.GetItem()->Location)->SortedIndex, Avatar->RefPathNode->SortedIndex) < 10 )
						{
							return Gari->DeployedMines.GetItem();
						}
					}
					break;
				}
			}
		}
	}

	return NULL;
}

int CAgent::GetNodeDistance(int FirstNodeIndex, int SecondNodeIndex)
{
	return ((FirstNodeIndex + Avatar->GetWorld()->TotalPathNodes - SecondNodeIndex) % Avatar->GetWorld()->TotalPathNodes);
}


void CAgent::ForceNodeMotorics(float SpeedLimit, float AngularCoef, float StrictAngle)
{
	ForcedMotorics = MotoricsNodeDirection;
	MotoricsNodeDirection->SetSpeedLimit(SpeedLimit);
	MotoricsNodeDirection->SetAngularCoef(AngularCoef);
	MotoricsNodeDirection->SetStrictAngle(StrictAngle);
}

void CAgent::ForcedMotoricsOff()
{
	if (ForcedMotorics == MotoricsNodeDirection)
		MotoricsNodeDirection->SetDefaultSettings();

	ForcedMotorics = NULL;
}

void CAgent::SetArmoryEnabled(bool Value)
{
	ArmoryEnabled = Value;
}

bool CAgent::GetArmoryEnabled()
{
	return ArmoryEnabled;
}

bool CAgent::TestNodeForcedMotorics()
{
	WePathNode *Node = Avatar->GetWorld()->FindClosestDirectionPathNode(Avatar->Location);

	if (!Node) return false;

	if (Node->FollowTime > 0.0f)
	{
		SelectMotorics(MotoricsNodeDirection, Node->FollowTime);
		MotoricsNodeDirection->SetDefaultSettings();
		MotoricsNodeDirection->SetSpeedLimit(Avatar->MaxSpeed);
		Motorics = MotoricsSelected;

		return true;
	}

	return false;
}
