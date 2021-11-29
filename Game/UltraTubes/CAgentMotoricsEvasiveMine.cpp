/*
	Filename:	CAgentMotoricsEvasiveMine.cpp
	Author:		Goran Bogdanovic
*/


#include <CAgentMotoricsEvasiveMine.h>
#include <CAgent.h>
#include <CInputMessage.h>
#include <CGamePlayerConfig.h>
#include <CProfile.h>
#include <CTuber.h>
#include <WePhysics.h>
#include <WePathNode.h>
#include <CWeapon.h>
#include <CGame.h>


CAgentMotoricsEvasiveMine::CAgentMotoricsEvasiveMine(CAgent* Owner)
{
	Parent = Owner;
}


CAgentMotoricsEvasiveMine::~CAgentMotoricsEvasiveMine()
{
}


void CAgentMotoricsEvasiveMine::SetMineLocation(WeVector &MineLoc)
{
	MineLocation = MineLoc;
}


WeVector &CAgentMotoricsEvasiveMine::GetMineLocation()
{
	return MineLocation;
}


void CAgentMotoricsEvasiveMine::Tick(CInputMessage *Msg)
{
	CAgentMotoricsStandardTube::Tick(Msg);

	CTuber* Avatar = Parent->GetAvatarConfig()->User->Tuber;
	if (!Avatar) return;

	WeVector AX, AY, AZ;
	Avatar->GetLocalAxis(AX, AY, AZ);
	D3DXVec3Normalize(&AX, &AX);
	D3DXVec3Normalize(&AY, &AY);
	D3DXVec3Normalize(&AZ, &AZ);

	//WeVector AX, AY, AZ;
	//MineToEvade->GetLocalAxis(AX, AY, AZ);
	//D3DXVec3Normalize(&AX, &AX);
	//D3DXVec3Normalize(&AY, &AY);
	////D3DXVec3Normalize(&AZ, &AZ);

	//WeVector AAX, AAY, AAZ;
	//Avatar->GetLocalAxis(AAX, AAY, AAZ);
	////D3DXVec3Normalize(&AAX, &AAX);
	////D3DXVec3Normalize(&AAY, &AAY);
	//D3DXVec3Normalize(&AAZ, &AAZ);

	float MaxRayDistance = 40.0f;
	float RayLeft = -1.0f;
	float RayRight = -1.0f;
	float RayUp = -1.0f;
	float RayDown = -1.0f;
	RayLeft = Parent->QuickRay(MineLocation, -AX, MaxRayDistance);
	RayRight = Parent->QuickRay(MineLocation, AX, MaxRayDistance);
	RayUp = Parent->QuickRay(MineLocation, AY, MaxRayDistance);
	RayDown = Parent->QuickRay(MineLocation, -AY, MaxRayDistance);

	bool UseLeft = false;
	if ( ((RayLeft > -1.0f && RayRight > -1.0f) && (RayLeft > RayRight)) ||
		(RayRight == -1.0f && RayLeft > -1.0f) )
	{
		UseLeft = true;
	}
	bool UseUp = false;
	if ( ((RayUp > -1.0f && RayDown > -1.0f) && (RayUp > RayDown)) ||
		(RayUp == -1.0f && RayDown > -1.0f) )
	{
		UseUp = true;
	}
	WeVector EvasivePoint;
	if (UseLeft)
		EvasivePoint = MineLocation - (AX * RayLeft);
	else
		EvasivePoint = MineLocation + (AX * RayRight);
	if (UseUp)
		EvasivePoint += (AY * RayUp);
	else
		EvasivePoint -= (AY * RayUp);

	int RXAddon = 0;
	int RYAddon = 0;
	WeVector Out, Dist;
	Dist = (Avatar->Location - EvasivePoint);
	D3DXVec3Normalize(&Dist, &Dist);
	D3DXVec3Cross(&Out, &AZ, &Dist);
	D3DXVec3Normalize(&Out, &Out);

	float ManouverFactor = (0.5f / Avatar->YawManouver);
	int MotionH = (int)(MAX_RX * 0.2f * ManouverFactor);
	int MotionV = (int)(MAX_RY * 0.2f * ManouverFactor);
	RXAddon = Out.y < 0 ? MotionH : -MotionH;
	RYAddon = Out.x < 0 ? MotionV : -MotionV;

	Msg->rx += RXAddon;
	Msg->ry += RYAddon;
	return;
}
