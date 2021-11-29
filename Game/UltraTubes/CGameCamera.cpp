/*
       File: CGameCamera.cpp
	   Author: Tomislav Kukic
	   Date: 23.06.2006
	   Version: 1.0
*/



#include <CGameCamera.h>
#include <CTuber.h>
#include <CMainGamePhase.h>
#include <WeTimer.h>
#include <WePhysics.h>
#include <CMainGamePhase.h>
#include <CConfig.h>
#include <Helper.h>
#include <CTuberPart.h>


CGameCamera::CGameCamera()
{
	Location = WeVector(0, 0, 0);
	Direction = WeVector(0, 0, 1);
	Up = WeVector(0, 1, 0);
	NearClip = 0.2f;
	FarClip = 3000.0f;
	FinishedAngle = 0;
	Distance = 0.0f;	
	
	CurrentState = GameCameraStateDefault;
	StateDefaultingTime = 0.0f;
	StateTimer = 0.0f;

	CameraCount = 0;
	CurrentCameraIndex = 0;
	CurrentCamera = NULL;
}



void CGameCamera::ReadCameraConfig(StringParam CameraName, CameraConfig *cc)
{
	cc->BackDistance = GetConfigFloat(CameraName + ".BackDistance", 5.0f);
	cc->UpDistance = GetConfigFloat(CameraName + ".UpDistance", 1.5f);
	cc->Distance = sqrt(cc->BackDistance * cc->BackDistance + cc->UpDistance * cc->UpDistance);
	cc->AccelFactor = GetConfigFloat(CameraName + ".Accel", 1.0f);
	cc->MinHitDistance = GetConfigFloat(CameraName + ".MinHitDistance", 1.0f);
	cc->MinVisibleDistance = GetConfigFloat(CameraName + ".MinVisibleDistance", 0.0f);
	cc->ViewDirection = GetConfigFloat(CameraName + ".ViewDirection", 1.0f);
	cc->Speed = GetConfigFloat(CameraName + ".Speed", 5.0f);
	cc->AngularSpeed = GetConfigFloat(CameraName + ".AngularSpeed", 5.0f);
	cc->NoShaking = GetConfigInt(CameraName + ".NoShaking", 0) != 0;
	cc->FPS = GetConfigInt(CameraName + ".FPS", 0) != 0;
	cc->Centered = GetConfigInt(CameraName + ".Centered", 0) != 0;
	cc->Stopping = GetConfigInt(CameraName + ".Stopping", 0) != 0;
	cc->LookFactor = GetConfigFloat(CameraName + ".LookFactor", 0.0f);
	cc->NoTrail = GetConfigInt(CameraName + ".NoTrail", 0) != 0;
	cc->MinDistance = GetConfigFloat(CameraName + ".MinDistance", 2.0f);
	cc->MaxDistance = GetConfigFloat(CameraName + ".MaxDistance", 50.0f);
	cc->MinFOV = GetConfigFloat(CameraName + ".MinFOV", 90.0) / 180.0f * D3DX_PI;
	cc->MaxFOV = GetConfigFloat(CameraName + ".MaxFOV", 120.0) / 180.0f * D3DX_PI;
	cc->TurboFOV = GetConfigFloat(CameraName + ".TurboFOV", 150.0) / 180.0f * D3DX_PI;
	cc->MinFOVSpeed = GetConfigFloat(CameraName + ".MinFOVSpeed", 0.0f);
	cc->MaxFOVSpeed = GetConfigFloat(CameraName + ".MaxFOVSpeed", 0.5f);
	cc->NoHitCheck = GetConfigInt(CameraName + ".NoHitCheck", 0) != 0;
}


void CGameCamera::Init()
{
	CameraCount = GetConfigInt("cameras.Init.Count", 1);

	for (int i = 0; i < CameraCount; ++i)
	{
		char tmp[50];
		sprintf(tmp, "cameras.Camera%d", (i + 1));
		ReadCameraConfig(tmp, &CameraCfg[i]);
	}

	ReadCameraConfig("cameras.FinishingCamera", &FinishingCamera);
	ReadCameraConfig("cameras.ReverseCamera", &ReverseCamera);
}



void CGameCamera::UpdateCameraLocation(WeVector &Location, WeVector &Direction, int i)
{
	WeRaycastHit Hit;
	if (WePhysics::Instance()->RaycastClosest(Location + CurrentCamera->MinHitDistance * Direction, -Direction, 
		WE_SHAPE_TYPE_STATIC, Hit, 7, CurrentCamera->MinHitDistance))
	{
		if (!Hit.ActorHit->GhostObject)
			Location = Location + Direction * (Hit.Distance - CurrentCamera->MinHitDistance);
	}
}


void CGameCamera::Update()
{
	static float RefreshTime = 0.0f;
	RefreshTime += WeTimer::Instance()->GetDeltaTimeF();
	if (RefreshTime > 1.0f)
	{
		Init();
		RefreshTime = 0.0f;
	}

	float time = WeTimer::Instance()->GetDeltaTimeF();

	if (CurrentState != GameCameraStateDefault)
	{
		StateTimer += time;
		if (StateTimer >= StateDefaultingTime)
		{
			StateTimer = 0.0f;
			StateDefaultingTime = 0.0f;
			CurrentState = GameCameraStateDefault;
		}
	}

	UpdateCurrentCamera();

	CTuber* Player = CMainGamePhase::Instance->InputPlayer;

	if (Player)
	{
		WeActor *LookTarget = Player;
		//if (Player->Dead)
			//LookTarget = Player->FirstPart;

		if (LookTarget)
		{
			WeVector Lx, Ly, Lz;
			LookTarget->GetLocalAxis(Lx, Ly, Lz);

			static bool LastCameraWasReverse = false;
			bool ReversingChanged = false;

			if (LastCameraWasReverse != (CurrentCamera == &ReverseCamera))
				ReversingChanged = true;

			LastCameraWasReverse = (CurrentCamera == &ReverseCamera);


			// ako je Dead, prati FirstPart

			if (CurrentCamera->Stopping)
			{
			}

			else if (CurrentCamera->Centered)
			{
				WePathNode *Node1, *Node2;
				float d1, d2;
				WeVector RefPos;

				CMainGamePhase::Instance->Map.GetRefPathNodes(LookTarget->Location, Node1, Node2, d1, d2, RefPos);

				WeVector W;
				D3DXVec3Scale(&W, &Node1->Direction, CurrentCamera->BackDistance);
				D3DXVec3Subtract(&Location, &RefPos, &W);

				D3DXVec3Subtract(&W, &LookTarget->Location, &Location);
				D3DXVec3Normalize(&W, &W);

				Direction = CurrentCamera->LookFactor * W + (1.0f - CurrentCamera->LookFactor) * Node1->Direction;
				D3DXVec3Normalize(&W, &W);

				Up = Ly;

				WeVector Right;
				D3DXVec3Cross(&Right, &Up, &Direction);
				D3DXVec3Normalize(&Right, &Right);
				D3DXVec3Cross(&Up, &Direction, &Right);
			}

			else
			{
				WeVector			DesiredDirection;
				WeVector			DesiredUp;
				WeVector			DesiredLocation;
				float				DesiredDistance;
				float				DesiredBackDistance;
				float				DesiredUpDistance;

				DesiredUp = Ly;

				// Calculate DesiredDirection
				//if (Player->RaceFinished)
				//{
				//	WeMatrix M;
				//	D3DXMatrixRotationAxis(&M, &Up, FinishedAngle);
				//	FinishedAngle += WeTimer::Instance()->GetDeltaTimeF()*3.14f/4.0f;

				//	WeVector v;
				//	v.x = 0;
				//	v.y = 0;
				//	v.z = 1;

				//	D3DXVec3TransformNormal(&v, &v, &M);            			
				//	DesiredDirection = v;
				//}
				//else
				//{
				//	DesiredDirection = Lz * CurrentCamera->ViewDirection;
				//	D3DXVec3Normalize(&DesiredDirection, &DesiredDirection);
				//}

				DesiredDirection = Lz * CurrentCamera->ViewDirection;
				D3DXVec3Normalize(&DesiredDirection, &DesiredDirection);

				// Calculate directional vector from player to camera
				WeVector CameraDirection;
				CameraDirection = -CurrentCamera->BackDistance * Lz + CurrentCamera->UpDistance * Ly;

				if (D3DXVec3Length(&CameraDirection) < 0.01f)
					CameraDirection = -Lz;
				else
					D3DXVec3Normalize(&CameraDirection, &CameraDirection);

				DesiredBackDistance = CurrentCamera->BackDistance;
				DesiredUpDistance = CurrentCamera->UpDistance;

				// Calculate camera distance
				if (CMainGamePhase::Instance->bInitCamera || CurrentCamera->NoShaking)
				{
					LastPlayerSpeed = 0.0f;
				}

				float CurrentSpeed;
				CurrentSpeed = Player->CurrentSpeed;
				if (CurrentSpeed > Player->TurboMaxSpeed)
					CurrentSpeed = Player->TurboMaxSpeed;

				float DivFactor, AccelFactor;
				DivFactor = time * Player->Acceleration;
				if (abs(DivFactor) < 0.001f)
					AccelFactor = 0.0f;
				else
					AccelFactor = (CurrentSpeed - LastPlayerSpeed) / DivFactor;

				DesiredDistance = CurrentCamera->Distance + AccelFactor * CurrentCamera->AccelFactor;
				if (DesiredDistance < CurrentCamera->MinDistance)
					DesiredDistance = CurrentCamera->MinDistance;
				if (DesiredDistance > CurrentCamera->MaxDistance)
					DesiredDistance = CurrentCamera->MaxDistance;

				LastPlayerSpeed = Player->CurrentSpeed;


				float kSpeed;
				kSpeed = time * CurrentCamera->Speed;
				if (kSpeed > 1.0f)
					kSpeed = 1.0f;

				float kAngularSpeed;
				kAngularSpeed = time * CurrentCamera->AngularSpeed;
				if (kAngularSpeed > 1.0f)
					kAngularSpeed = 1.0f;


				// Update Direction, Up, Distance
				if (CMainGamePhase::Instance->bInitCamera || CurrentCamera->NoShaking || ReversingChanged)
				{
					kSpeed = 1.0f;
					kAngularSpeed = 1.0f;
				}

				if (CMainGamePhase::Instance->bFixCameraDirection)
				{
					Direction = DesiredDirection;
					CMainGamePhase::Instance->bFixCameraDirection = false;
				}


				Direction += (DesiredDirection - Direction) * kAngularSpeed;
				Up += (DesiredUp - Up) * kAngularSpeed;
				Distance += (DesiredDistance - Distance) * kSpeed;
				BackDistance += (DesiredBackDistance - BackDistance) * kSpeed;
				UpDistance += (DesiredUpDistance - UpDistance) * kSpeed;

				//WeLoger::DebugLog("Camera %d   Direction = <%.3f %.3f %.3f>   Up = <%.3f %.3f %.3f>   Distance = %.3f : %.3f : %.3f\n",
					//CurrentCameraIndex + 1, Direction.x, Direction.y, Direction.z, Up.x, Up.y, Up.z, Distance, BackDistance, UpDistance);

				// Update Location
				float scale;
				if (abs(CurrentCamera->Distance) < 0.001)
					scale = 1.0f;
				else
					scale = Distance / CurrentCamera->Distance;

				//WeLoger::DebugLog("scale = %.3f\n", scale);

				/*
				WeVector Right;
				D3DXVec3Cross(&Right, &Up, &Direction);

				WeRaycastHit Hit;
				if (WePhysics::Instance()->RaycastClosest(Player->Location, CameraDirection, 
					WE_SHAPE_TYPE_STATIC, Hit, 7, Distance + CurrentCamera->MinHitDistance + 2.0f) &&
					!Hit.ActorHit->GhostObject)
				{
					//WeLoger::DebugLog("CAMERA: %.3f -> %.3f\n", Distance, Hit.Distance);
					float Normalscale, tdist, Reflscale;
					WeVector refl, tmpV;

					refl = Hit.WorldNormal*0.4f + CameraDirection;
					D3DXVec3Normalize(&refl, &refl);

					tdist = Hit.Distance - (Distance + CurrentCamera->MinHitDistance);

					if (tdist < 0.0f) tdist = 0.0f;
					if (tdist > 2.0f) tdist = 2.0f;

					Normalscale = -0.745f*tdist + 1.5f; // minimal scale is 0.01f and max is 1 			
								
					Reflscale = (Distance + CurrentCamera->MinHitDistance+2.0f) - Hit.Distance;			
					Distance = Hit.Distance - tdist;

					if (Distance >= CurrentCamera->MinHitDistance)
						Distance = Distance - CurrentCamera->MinHitDistance;
					else
						Distance = 0.0f;

					Location = Player->Location - Direction*scale*CurrentCamera->BackDistance + Up * scale * CurrentCamera->UpDistance 
						+ Hit.WorldNormal*Normalscale + refl*Reflscale*0.5f;


					//UpdateCameraLocation(Location, Up, 0);
					//UpdateCameraLocation(Location, -Up, 1);
					//UpdateCameraLocation(Location, Right, 2);
					//UpdateCameraLocation(Location, -Right, 3);
				}
				else
				{
					Location = Player->Location - Direction * scale * CurrentCamera->BackDistance + Up * scale * CurrentCamera->UpDistance;
				}
				*/

				//WeVector DesiredLocation;
				//DesiredLocation = LookTarget->Location - Direction * scale * CurrentCamera->BackDistance + Up * scale * CurrentCamera->UpDistance;
				//Location += (DesiredLocation - Location) * kSpeed;

				Location = LookTarget->Location - Direction * scale * BackDistance + Up * scale * UpDistance;

				// ------ limit Location in respect to distance from RefPathNode ------
				WePathNode *Node1, *Node2;
				float d1, d2;
				WeVector RefPos;

				CMainGamePhase::Instance->Map.GetRefPathNodes(Location, Node1, Node2, d1, d2, RefPos);

				WeVector vDir, vDirNorm;
				float d, r;

				vDir = Location - RefPos;
				D3DXVec3Normalize(&vDirNorm, &vDir);

				d = D3DXVec3Length(&vDir);
				r = (d1 * Node2->Radius - d2 * Node1->Radius) / (d1 - d2);
				r *= 0.99f;

#ifdef _DEBUG
				//char ttt[100];
				//sprintf(ttt, "Camera: %d <%3.3f %3.3f %3.3f> : <%3.3f %3.3f %3.3f>\n<%3.3f %3.3f %3.3f> %3.3f <%3.3f %3.3f %3.3f> [%d : %3.3f] [%d : %3.3f]",
				//	Node1->SequenceNumber,
				//	Node1->Location.x, Node1->Location.y, Node1->Location.z,
				//	Node1->Direction.x, Node1->Direction.y, Node1->Direction.z,
				//	RefPos.x, RefPos.y, RefPos.z,
				//	d, vDirNorm.x, vDirNorm.y, vDirNorm.z,
				//	Node1->SequenceNumber, d1, Node2->SequenceNumber, d2);
				//CMainGamePhase::Instance->DebugStr = ttt;
#endif

				/*
				if (d > r)
				{
					Location = RefPos + vDirNorm * r;
					Direction = LookTarget->Location - Location;
					D3DXVec3Normalize(&Direction, &Direction);
				}
				*/

				if (!CurrentCamera->NoHitCheck)
				{
					bool hit;
					WeVector Loc = RefPos;
					float BaseD = 0.0f;
					float CheckD = d + CurrentCamera->MinHitDistance;

					WeRaycastHit Hit;
					while ((hit = WePhysics::Instance()->RaycastClosest(Loc, vDirNorm, 
						WE_SHAPE_TYPE_STATIC, Hit, 7, CheckD)) && Hit.ActorHit->GhostObject)
					{
#ifdef _DEBUG
						//sprintf(ttt, " \n[%s %.3f]", Hit.ActorHit->ModelName.GetString(), Hit.ActorHit->GhostObject, BaseD + Hit.Distance);
						//CMainGamePhase::Instance->DebugStr += ttt;
#endif

						float D = Hit.Distance + 0.001f;
						Loc += D * vDirNorm;
						BaseD += D;
						CheckD -= D;
					}

#ifdef _DEBUG
					//if (hit)
					//{
					//	sprintf(ttt, "\n=====[%s%s %.3f]", Hit.ActorHit->ModelName.GetString(), (Hit.ActorHit->GhostObject ? " <GHOST>" : ""), BaseD + Hit.Distance);
					//	CMainGamePhase::Instance->DebugStr += ttt;
					//}
#endif

					if (hit && !Hit.ActorHit->GhostObject)
					{
						if (!Hit.ActorHit->GhostObject)
						{
							float distance = BaseD + Hit.Distance;

							if (distance >= CurrentCamera->MinHitDistance)
								distance = distance - CurrentCamera->MinHitDistance;
							else
								distance = 0.0f;

							Location = RefPos + vDirNorm * distance;

							//Direction = LookTarget->Location - Location;
							//D3DXVec3Normalize(&Direction, &Direction);
						}
					}
				}
				// ------ ------ ------ ------ ------ ------ ------ ------ ------ ------


				//WeLoger::DebugLog("Distance = %.3f\n", Distance);
			}

			WeVector RandomVector = WeVector(RndF(-0.5f, +0.5f), RndF(-0.5f, +0.5f), RndF(-0.5f, +0.5f));
			D3DXVec3Normalize(&RandomVector, &RandomVector);
 			if (!Player->Dead && Player->OnTurbo)
			{
				Direction = Direction + (RandomVector / 100.0f);
			}
			else if (CurrentState == GameCameraStateShaking)
			{
				Direction = Direction + (RandomVector / 50.0f);
			}
			else if (CurrentState == GameCameraStateShakingSofting)
			{
				Direction = Direction + (RandomVector / (200.0f * (0.5f + (StateTimer / StateDefaultingTime))));
			}
			D3DXVec3Normalize(&Direction, &Direction);

			if (!Player->Dead)
				Player->Renderable = (Distance >= CurrentCamera->MinVisibleDistance);

#ifdef _DEBUG
			//CMainGamePhase::Instance->DebugStr2 = (String)"";
			//char temp2[1000];

			//sprintf(temp2, "   [P: %.3f, %.3f, %.3f]", Player->Location.x, Player->Location.y, Player->Location.z);
			//CMainGamePhase::Instance->DebugStr2 += (String)temp2;

			//sprintf(temp2, "   [L: %.3f, %.3f, %.3f]", Location.x, Location.y, Location.z);
			//CMainGamePhase::Instance->DebugStr2 += (String)temp2;

			//sprintf(temp2, "   [D: %.3f, %.3f, %.3f]", Direction.x, Direction.y, Direction.z);
			//CMainGamePhase::Instance->DebugStr2 += (String)temp2;

			//sprintf(temp2, "   [U: %.3f, %.3f, %.3f]", Up.x, Up.y, Up.z);
			//CMainGamePhase::Instance->DebugStr2 += (String)temp2;

			//sprintf(temp2, "   [d: %.3f]", Distance);
			//CMainGamePhase::Instance->DebugStr2 += (String)temp2;
#endif

			if (CMainGamePhase::Instance->bInitCamera)
			{
				CMainGamePhase::Instance->bInitCamera = false;
			}
		}
	}

	WeCamera::Update();
}



void CGameCamera::UpdateCurrentCamera()
{
	CTuber* Player = CMainGamePhase::Instance->InputPlayer;
	if (Player && !Player->Dead && Player->RaceFinished)
		CurrentCamera = &FinishingCamera;
	else if (CMainGamePhase::Instance->UseReverseCamera)
		CurrentCamera = &ReverseCamera;
	else
		CurrentCamera = &CameraCfg[CurrentCameraIndex];
}



void CGameCamera::SetNextCamera()
{
	++CurrentCameraIndex;
	if (CurrentCameraIndex == CameraCount)
		CurrentCameraIndex = 0;
}



void CGameCamera::SetPrevCamera()
{
	--CurrentCameraIndex;
	if (CurrentCameraIndex < 0)
		CurrentCameraIndex = CameraCount - 1;
}


void CGameCamera::SetCamera(int index)
{
	if ((index >= 0) && (index < CameraCount))
		CurrentCameraIndex = index;
}
