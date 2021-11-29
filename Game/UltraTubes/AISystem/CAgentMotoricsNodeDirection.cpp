/*
	Filename:	CAgentMotoricsNodeDirection.cpp
	Author:		Goran Bogdanovic
*/


#include <CAgentMotoricsNodeDirection.h>
#include <CAgent.h>
#include <CInputMessage.h>
#include <CGamePlayerConfig.h>
#include <CProfile.h>
#include <CTuber.h>
#include <WePhysics.h>
#include <WePathNode.h>
#include <CGame.h>
#include <WeLoger.h>

#define DEFAULT_STRICT_ANGLE	-10.0f


CAgentMotoricsNodeDirection::CAgentMotoricsNodeDirection(CAgent* Owner)
{
	Parent = Owner;
	SetDefaultSettings();
}


CAgentMotoricsNodeDirection::~CAgentMotoricsNodeDirection()
{
}


void CAgentMotoricsNodeDirection::Tick(CInputMessage *Msg)
{
	CTuber* Avatar = Parent->GetAvatarConfig()->User->Tuber;
	if (!Avatar) return;

	WeVector AX, AY, AZ;
	Avatar->GetLocalAxis(AX, AY, AZ);
	D3DXVec3Normalize(&AX, &AX);
	D3DXVec3Normalize(&AY, &AY);
	D3DXVec3Normalize(&AZ, &AZ);

	int MotionH = (int)(MAX_RX * AngularCoef);
	int MotionV = (int)(MAX_RY * AngularCoef);

	//WePathNode *NewNodeOne = Avatar->GetWorld()->FindClosestDirectionPathNode(Avatar->RefPathNode->Location);
	//WePathNode *NewNodeTwo = NULL;
	//if (NewNodeOne)
	//	NewNodeTwo = Avatar->GetWorld()->FindClosestDirectionPathNode(NewNodeOne->Location);
	//WePathNode *NewNode = NewNodeTwo ? NewNodeTwo : NewNodeOne;

	WePathNode *NewNode = Avatar->RefPathNode->NextNode;
	if (NewNode->NextNode->NodeType != NODETYPE_TELEPORTED)
		NewNode = NewNode->NextNode;

	WeVector Out, Dist;
	float Dot;
	if (NewNode)
	{
		Dist = (Avatar->Location - NewNode->Location);
		D3DXVec3Normalize(&Dist, &Dist);
		D3DXVec3Cross(&Out, &AZ, &Dist);
		D3DXVec3Normalize(&Out, &Out);
		Dot = D3DXVec3Dot(&(-Dist), &AZ);

		if ( (StrictAngle == DEFAULT_STRICT_ANGLE) || (Dot < 0.999f) )
		{
			Msg->rx = Out.y < 0 ? MotionH : -MotionH;
			Msg->ry = Out.x < 0 ? MotionV : -MotionV;
		}
		//if (StrictAngle == DEFAULT_STRICT_ANGLE)
		//{
		//	Msg->rx *= (1.0f - Dot);
		//	Msg->ry *= (1.0f - Dot);
		//}
	}

	if ( (StrictAngle == DEFAULT_STRICT_ANGLE) || (Dot > StrictAngle) )
	{
		float RayFront = -1.0f;
		RayFront = Parent->QuickRay(Avatar->Location, AZ, 10.0f);
		if ( (RayFront == -1.0f) && (Avatar->CurrentSpeed <= SpeedLimit) )
		{
			Msg->MoveForward = true;
		}
		else
		{
			float RayBack = -1.0f;
			RayBack = Parent->QuickRay(Avatar->Location, -AZ, 5.0f);
			if (RayBack == -1.0f)
			{
				Msg->MoveBackward = true;
			}
			else
			{
				// just in case, move somewhere
				Msg->MoveForward = true;
			}
		}
	}

	CAgentMotorics::Tick(Msg);
}


void CAgentMotoricsNodeDirection::SetSpeedLimit(float Value)
{
	SpeedLimit = Value;
}

float CAgentMotoricsNodeDirection::GetSpeedLimit()
{
	return SpeedLimit;
}

void CAgentMotoricsNodeDirection::SetAngularCoef(float Value)
{
	AngularCoef = Value;
}

float CAgentMotoricsNodeDirection::GetAngularCoef()
{
	return AngularCoef;
}

void CAgentMotoricsNodeDirection::SetDefaultSettings()
{
	SpeedLimit = 75.0f;
	AngularCoef = 1.0f;
	StrictAngle = DEFAULT_STRICT_ANGLE;
}


void CAgentMotoricsNodeDirection::SetStrictAngle(float Value)
{
	StrictAngle = Value;
}

float CAgentMotoricsNodeDirection::GetStrictAngle()
{
	return StrictAngle;
}
