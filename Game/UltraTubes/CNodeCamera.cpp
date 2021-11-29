/*
       File: CNodeCamera.cpp
	   Author: Goran Bogdanovic
*/


#include <CNodeCamera.h>
#include <WeTimer.h>
#include <WePhysics.h>
#include <WePathNode.h>
#include <CMenuPhase.h>
#include <WeLight.h>

#define	DEFAULT_SPEED	50.0f


CNodeCamera::CNodeCamera()
{
	Location = WeVector(0.0f, 0.0f, 0.0f);
	Direction = WeVector(0.0f, 0.0f, 1.0f);
	Up = WeVector(0.0f, 1.0f, 0.0f);
	
	LerpS = 0.0f;
	LookDirectionSpeed = 0.0f;
	MoveDirection = WeVector(0.0f, 0.0f, 0.0f);
	
	Node = NULL;
	NextNode = NULL;
	Speed = DEFAULT_SPEED;

	Light = new WeLight();
	WeLightState ls;
	ls.Range = 80.0f;
	ls.Diffuse = WeColor(1.0f, 1.0f, 1.0f, 1.0f);
	Light->AddKeyState(ls, 1.0f);
	Light->IsDynamic = true;
	Light->AffectDynamicActors = true;
	Light->Location = Location;
	CMenuPhase::Instance->MenuWorld.AddActor(Light);
}


CNodeCamera::~CNodeCamera()
{
	SAFE_DELETE(Light);
}


void CNodeCamera::Update()
{
	float Time = WeTimer::Instance()->GetDeltaTimeF();

	//WePathNode *NewNode = NULL;
	//NewNode = CMenuPhase::Instance->MenuWorld.FindClosestDirectionPathNode(Location);

	//if (NewNode && (NewNode != Node))
	//{
	//	Node = NewNode;
	//	NextNode = CMenuPhase::Instance->MenuWorld.FindClosestDirectionPathNode(Node->Location);

	//	MoveDirection = (Node->Location - Location);
	//	D3DXVec3Normalize(&MoveDirection, &MoveDirection);

	//	float NodeLength = D3DXVec3Length(&(NextNode->Location - Node->Location));
	//	if (LookDirectionSpeed == 0.0f)
	//		LookDirectionSpeed = DEFAULT_SPEED / NodeLength;
	//	LerpS = 0.0f;
	//}

	//WeVector Out;
	//LerpS += (LookDirectionSpeed * Time);
	//D3DXVec3Lerp(&Out, &Node->Location, &NextNode->Location, LerpS);
	//D3DXVec3Normalize(&Direction, &(Out - Location));

	//----------------

	//static float a = 0.0f;
	//static float r = (1080 / D3DX_PI);
	//a += (Time / 8);

	//Location = WeVector((-r + (r * -sin(D3DX_PI + a))), 0.0f, ((r * -cos(a))));

	//WeVector loc = Location;
	//loc.x += r;
	//D3DXVec3Cross(&Direction, &loc, &WeVector(0.0f, 1.0f, 0.0f));
	//D3DXVec3Normalize(&Direction, &Direction);

	//----------------

	//----------------

	static float a = 0.0f;
	static float r = 243.5f;
	a += (Time / 8);

	Location = WeVector((-r + (r * -sin(D3DX_PI + a))), 0.0f, ((r * -cos(a)))) + WeVector(59.0f, 0.0f, 36.0f);

	WeVector loc = Location;
	loc.x += r;
	D3DXVec3Cross(&Direction, &loc, &WeVector(0.0f, 1.0f, 0.0f));
	D3DXVec3Normalize(&Direction, &Direction);

	//----------------

	Light->Location = Location;
	WeCamera::Update();
}
