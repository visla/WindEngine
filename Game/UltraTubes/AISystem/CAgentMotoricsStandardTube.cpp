/*
	Filename:	CAgentMotoricsStandardTube.cpp
	Author:		Goran Bogdanovic
*/


#include <CAgentMotoricsStandardTube.h>
#include <CAgent.h>
#include <CInputMessage.h>
#include <CGamePlayerConfig.h>
#include <CProfile.h>
#include <CTuber.h>
#include <WePhysics.h>
#include <CPlayGamePhase.h>
#include <Helper.h>
#include <CAIKernel.h>
#include <CGame.h>


CAgentMotoricsStandardTube::~CAgentMotoricsStandardTube()
{
}


void CAgentMotoricsStandardTube::Tick(CInputMessage *Msg)
{
	CTuber* Avatar = Parent->GetAvatarConfig()->User->Tuber;
	if (!Avatar) return;

	TickTurbo(Msg);

	WeVector AX, AY, AZ;
	Avatar->GetLocalAxis(AX, AY, AZ);
	D3DXVec3Normalize(&AX, &AX);
	D3DXVec3Normalize(&AY, &AY);
	D3DXVec3Normalize(&AZ, &AZ);

	TickSpeed(Msg);

	int MotionH = 5;
	int MotionV = 5;
	const float RAY_FRONT_DISTANCE = 500;
	const float RAY_QUICK_DISTANCE = 100;
	const float CRITICAL_DISTANCE = 3.0f;
	const float CRITICAL_MULTIPLIER = 1.5f;
	const float MANUVER_MULTIPLIER = 0.28f; //0.4f
	const float MAX_MANUVERABILITY = 1.5f;
	const float MIN_MANUVERABILITY = 0.4f;

	WeRaycastHit Hit, HitLeft, HitRight, HitUp, HitDown;

	float DistLeft = Parent->QuickRay(Avatar->Location, -AX, RAY_QUICK_DISTANCE);
    float DistRight = Parent->QuickRay(Avatar->Location, AX, RAY_QUICK_DISTANCE);
	float DistUp = Parent->QuickRay(Avatar->Location, AY, RAY_QUICK_DISTANCE);
    float DistDown = Parent->QuickRay(Avatar->Location, -AY, RAY_QUICK_DISTANCE);
	float FromLeft = 2.0f;
	float FromRight = 2.0f;
	float FromUp = 2.0f;
	float FromDown = 2.0f;
	float DistCornerUp = -1.0f;
	float DistCornerDown = -1.0f;
	float DistCornerLeft = -1.0f;
	float DistCornerRight = -1.0f;

	bool Center = true;

	//Center = WePhysics::Instance()->RaycastClosest(Avatar->Location, AZ, WE_SHAPE_TYPE_STATIC, Hit, 7, RAY_FRONT_DISTANCE / 2);

	WeVector HitNormal;
	int Sign = 0;

	if (Center)
	{
		bool Left, Right;
		WeVector DirVec;
		D3DXVec3Normalize(&DirVec, &(AZ - AX));
		DistCornerLeft = Parent->QuickRay(Avatar->Location, DirVec, RAY_QUICK_DISTANCE);
		Left = WePhysics::Instance()->RaycastClosest(Avatar->Location - (AX * FromLeft), AZ, WE_SHAPE_TYPE_STATIC, HitLeft, 7, RAY_FRONT_DISTANCE);

		D3DXVec3Normalize(&DirVec, &(AZ + AX));
		DistCornerRight = Parent->QuickRay(Avatar->Location, DirVec, RAY_QUICK_DISTANCE);
		Right = WePhysics::Instance()->RaycastClosest(Avatar->Location + (AX * FromRight), AZ, WE_SHAPE_TYPE_STATIC, HitRight, 7, RAY_FRONT_DISTANCE);

		bool OK = true;

		//if (Left && Right)
		//{
		//	WeVector RicLeft = GetRicochet(Avatar->Location - (AX * FromLeft), HitLeft.WorldImpact, HitLeft.WorldNormal);
		//	WeVector RicRight = GetRicochet(Avatar->Location + (AX * FromRight), HitRight.WorldImpact, HitRight.WorldNormal);
		//	float dot1 = D3DXVec3Dot(&AX, &RicLeft);
		//	float dot2 = D3DXVec3Dot(&AX, &RicRight);
		//	OK = ( ((dot1 > 0.0f) && (dot2 > 0.0f)) || ((dot1 < 0.0f) && (dot2 < 0.0f)) );
		//}

		if (Left && Right && abs(HitLeft.Distance - HitRight.Distance) < 5.0f) OK = false;

		Sign = 0;
		if (OK)
		{
			if (Left && !Right)
			{
				HitNormal = HitLeft.WorldNormal;
				Sign = 1;
			}
			else if (!Left && Right)
			{
				HitNormal = HitRight.WorldNormal;
				Sign = -1;
			}
			else if (Left && Right)
			{
				if (HitRight.Distance > HitLeft.Distance)
				{
					HitNormal = HitRight.WorldNormal;
					Sign = 1;
				}
				else if (HitLeft.Distance > HitRight.Distance)
				{
					HitNormal = HitLeft.WorldNormal;
					Sign = -1;
				}
				else
				{
					//frontal, shouldn't come to this in here, agent's fault
				}
			}

			if (Sign != 0)
			{
				float Angle = D3DXVec3Dot(&(-AZ), &HitNormal);
				MotionH = (int)( (1.0f - Angle) * MAX_RX * 
					( ((1.0f - MANUVER_MULTIPLIER) * (MAX_MANUVERABILITY - Avatar->YawManouver)) / (MAX_MANUVERABILITY - MIN_MANUVERABILITY) + MANUVER_MULTIPLIER ) );
				Msg->rx = Sign * MotionH;

			}
		}
	}
	else
	{
		//if ( (DistLeft > -1.0f) || (DistRight > -1.0f) )
		//{
		//	Msg->rx = DistLeft > DistRight ? -MotionH : MotionH;
		//}
	}

	if ( ((DistCornerLeft > -1.0f) && (DistCornerLeft < CRITICAL_DISTANCE)) || ((DistLeft > -1.0f) && (DistLeft < CRITICAL_DISTANCE)) )
	{
		//WeRaycastHit Hit;
		//WePhysics::Instance()->RaycastClosest(Avatar->Location, -AX, WE_SHAPE_TYPE_STATIC, Hit, 7, RAY_QUICK_DISTANCE);
		//float Angle = D3DXVec3Dot(&(AX), &Hit.WorldNormal);
		//MotionH = (int)((Angle * MAX_RX) * (Avatar->YawManouver * MANUVER_MULTIPLIER) * 0.5f);
		Msg->rx = (int)(Msg->rx + MotionH * CRITICAL_MULTIPLIER);
	}
	if ( ((DistCornerRight > -1.0f) && (DistCornerRight < CRITICAL_DISTANCE)) || ((DistRight > -1.0f) && (DistRight < CRITICAL_DISTANCE)) )
	{
		//WeRaycastHit Hit;
		//WePhysics::Instance()->RaycastClosest(Avatar->Location, AX, WE_SHAPE_TYPE_STATIC, Hit, 7, RAY_QUICK_DISTANCE);
		//float Angle = D3DXVec3Dot(&(-AX), &Hit.WorldNormal);
		//MotionH = (int)((Angle * MAX_RX) * (Avatar->YawManouver * MANUVER_MULTIPLIER) * 0.5f);
		Msg->rx = (int)(Msg->rx - MotionH * CRITICAL_MULTIPLIER);
	}

	if (Center)
	{
		bool Up, Down;
		WeVector DirVec;
		D3DXVec3Normalize(&DirVec, &(AZ + AY));
		DistCornerUp = Parent->QuickRay(Avatar->Location, DirVec, RAY_QUICK_DISTANCE);
		Up = WePhysics::Instance()->RaycastClosest(Avatar->Location + (AY * FromUp), AZ, WE_SHAPE_TYPE_STATIC, HitUp, 7, RAY_FRONT_DISTANCE);

		D3DXVec3Normalize(&DirVec, &(AZ - AY));
		DistCornerDown = Parent->QuickRay(Avatar->Location, DirVec, RAY_QUICK_DISTANCE);
		Down = WePhysics::Instance()->RaycastClosest(Avatar->Location - (AY * FromDown), AZ, WE_SHAPE_TYPE_STATIC, HitDown, 7, RAY_FRONT_DISTANCE);

		bool OK = true;

		//if (Up && Down)
		//{
		//	WeVector RicUp = GetRicochet(Avatar->Location + (AY * FromUp), HitUp.WorldImpact, HitUp.WorldNormal);
		//	WeVector RicDown = GetRicochet(Avatar->Location - (AY * FromDown), HitDown.WorldImpact, HitDown.WorldNormal);
		//	float dot1 = D3DXVec3Dot(&AX, &RicUp);
		//	float dot2 = D3DXVec3Dot(&AX, &RicDown);
		//	OK = ( ((dot1 > 0.0f) && (dot2 > 0.0f)) || ((dot1 < 0.0f) && (dot2 < 0.0f)) );
		//}

		if (Up && Down && abs(HitUp.Distance - HitDown.Distance) < 5.0f) OK = false;

		Sign = 0;
		if (OK)
		{
			if (Up && !Down)
			{
				HitNormal = HitUp.WorldNormal;
				Sign = 1;
			}
			else if (!Up && Down)
			{
				HitNormal = HitDown.WorldNormal;
				Sign = -1;
			}
			else if (Up && Down)
			{
				if (HitUp.Distance > HitDown.Distance)
				{
					HitNormal = HitUp.WorldNormal;
					Sign = -1;
				}
				else if (HitDown.Distance > HitUp.Distance)
				{
					HitNormal = HitDown.WorldNormal;
					Sign = 1;
				}
				else
				{
					//frontal, shouldn't come to this in here, agent's fault
				}
			}

			if (Sign != 0)
			{
				float Angle = D3DXVec3Dot(&(-AZ), &HitNormal);
				MotionV = (int)( (1.0f - Angle) * MAX_RY * 
					( ((1.0f - MANUVER_MULTIPLIER) * (MAX_MANUVERABILITY - Avatar->YawManouver)) / (MAX_MANUVERABILITY - MIN_MANUVERABILITY) + MANUVER_MULTIPLIER ) );
				Msg->ry = Sign * MotionV;
			}
		}

	}
	else
	{
		//if ( (DistUp > -1.0f) || (DistDown > -1.0f) )
		//{
		//	Msg->ry = DistUp > DistDown ? -MotionV : MotionV;
		//}
	}
	if ( ((DistCornerUp > -1.0f) && (DistCornerUp < CRITICAL_DISTANCE)) || ((DistUp > -1.0f) && (DistUp < CRITICAL_DISTANCE)) )
		Msg->ry = (int)(Msg->ry + MotionV * CRITICAL_MULTIPLIER);
	if ( ((DistCornerDown > -1.0f) && (DistCornerDown < CRITICAL_DISTANCE)) || ((DistDown > -1.0f) && (DistDown < CRITICAL_DISTANCE)) )
		Msg->ry = (int)(Msg->ry - MotionV * CRITICAL_MULTIPLIER);

	// rotation, use roll to accelerate faster
	if ( 
		(Parent->GetDifficulty() != AgentDifficultyEasy) && 
		(Avatar->CurrentSpeed < (Avatar->MaxSpeed * 0.9f)) )
	{
		if (Avatar->Position % 2 == 0)
			Msg->RotateRight = true;
		else
			Msg->RotateLeft = true;
	}

	CAgentMotorics::Tick(Msg);
}

void CAgentMotoricsStandardTube::TickTurbo(CInputMessage *Msg)
{
	if (Parent->GetDifficulty() == AgentDifficultyEasy) return;		//no turbo on easy

	CTuber* Avatar = Parent->GetAvatarConfig()->User->Tuber;

	if (Avatar->TurboCharges <= 0.0f) return;

	// use turbo to accelerate faster
	//if (Avatar->CurrentSpeed < Avatar->MaxSpeed * 0.9f)
	//{
	//	Msg->Turbo = true;
	//	return;
	//}

	bool Activate = false;

	if (Avatar->Position > 1)
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (CGameSettings::ServerInstance->tmpPlayers[i].User &&
				CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber &&
				CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->Position < Avatar->Position &&
				!CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->IsFriendWith(Avatar)
				)
			{
				Activate = true;
				break;
			}
		}

	if ( !Activate && (Parent->GetDifficulty() == AgentDifficultyGuru) && (Avatar->RefPathNode->SortedIndex > 9) )
	{
		float LapTime = CPlayGamePhase::Instance->RaceTime - Avatar->StartLapTime;
		float TimePerNode = LapTime / (float)(Avatar->RefPathNode->SortedIndex + 1);
		float TurboTimePerNode = TimePerNode * Avatar->MaxSpeed / Avatar->TurboMaxSpeed;
		int RemainingNodes = Avatar->GetWorld()->TotalPathNodes - (Avatar->RefPathNode->SortedIndex + 1);

		if ((float)RemainingNodes * TurboTimePerNode <= Avatar->TurboCharges / TURBO_USAGE_PER_SEC)
			Activate = true;
	}

	Msg->Turbo = Activate;
}


void CAgentMotoricsStandardTube::GetRicochet(WeVector &Result, WeVector &From, WeVector &Impact, WeVector &Normal)
{
	WeVector c, d, f;
	c = Impact - From;
	d = D3DXVec3Dot(&c, &Normal) * Normal;
	f = d + (d - From);
	Result = (f - Impact);
}


void CAgentMotoricsStandardTube::TickSpeed(CInputMessage *Msg)
{
	if (Parent->GetDifficulty() != AgentDifficultyEasy)
	{
		Msg->MoveForward = true;
		return;
	}

	CTuber* Avatar = Parent->GetAvatarConfig()->User->Tuber;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (CGameSettings::ServerInstance->tmpPlayers[i].User &&
			CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber &&
			CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->Position > Avatar->Position &&
			!CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber->IsFriendWith(Avatar) &&
			!CPlayGamePhase::Instance->GetAIKernel()->HasAnAgent(CGameSettings::ServerInstance->tmpPlayers[i].User->Tuber)
			)
		{
			int RandHandicap = Rnd((UINT)(0.2f * (float)Avatar->MaxSpeed));
			if (Avatar->CurrentSpeed < Avatar->MaxSpeed - RandHandicap)
				Msg->MoveForward = true;
			return;
		}
	}

	Msg->MoveForward = true;
}
