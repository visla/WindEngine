/*
    CMassSelection.cpp

	Autor: Nebojsa Vislavski
	Date: 14.12.2005.
	Version: 1.0

	This is mass selection class

*/

#include <CMassSelection.h>
#include <CMapEditorWindow.h>
#include <CSelection.h>
#include <CMapEditorCamera.h>
#include <WePhysics.h>
#include <WeRenderDevice.h>
#include <ActorTags.h>
#include <CActionManager.h>
#include <CEditorMap.h>
#include <WePathNode.h>
#include <CActorTypesSupplier.h>
#include <CDialogStartLoacationProperties.h>
#include <CUndo.h>
#include <CPickupFactory.h>
#include <WeCloth.h>
#include <WeZone.h>
#include <WePortal.h>
#include <WeLight.h>
#include <CDialogPickupProperties.h>
#include <CDialogActor.h>
#include <CDialogLight.h>
#include <CDialogCloth.h>
#include <CDialogParticleSystem.h>
#include <WeParticleSystem.h>
#include <CDialogZone.h>
#include <CDialogPortal.h>
#include <CDialogPathNode.h>
#include <WeLoger.h>



CMassSelection::CMassSelection()
{
	DeltaMoving = WeVector(0.0f,0.0f,0.0f);
	DeltaRotation = WeVector(0.0f,0.0f,0.0f);
	StartRotation = WeVector(0.0f,0.0f,0.0f);
	StartLocation = WeVector(0.0f,0.0f,0.0f);
	//RefX = 0.0f;
	//RefY = 0.0f;
	ShownPropertiesType = 0;
	SelectPrefabs = true;
	SelectLights = true;
	SelectPathNodes = true;
	SelectStartLocations = true;
	SelectParticleSystems = true;
}


void CMassSelection::Align(int AlignType)
{
	Selections.First();
	float refx, refy, refz;
	float refyaw, refpitch, refroll;
	float yaw, pitch, roll;
	WeVector RefX, RefY, RefZ;

	WeVector loc;

	refx = Selections.GetItem()->SelectedActor->Location.x;
	refy = Selections.GetItem()->SelectedActor->Location.y;
	refz = Selections.GetItem()->SelectedActor->Location.z;
	Selections.GetItem()->SelectedActor->GetOrientation(refyaw, refpitch, refroll);
	Selections.GetItem()->SelectedActor->GetLocalAxis(RefX, RefY, RefZ);
	
	
	for (Selections.Next(); !Selections.End(); Selections.Next())
	{
		loc = Selections.GetItem()->SelectedActor->Location;
		Selections.GetItem()->SelectedActor->GetOrientation(yaw, pitch, roll);
		

		// align x axis
		if (AlignType & ALIGN_PIVOT_X) 		
			loc.x = refx;
		if (AlignType & ALIGN_PIVOT_Y) 		
			loc.y = refy;
		if (AlignType & ALIGN_PIVOT_Z) 		
			loc.z = refz;
		if (AlignType & ALIGN_ROTATION_YAW)
			yaw = refyaw;
		if (AlignType & ALIGN_ROTATION_PITCH)
			pitch = refpitch;
		if (AlignType & ALIGN_ROTATION_ROLL)
			roll = refroll;

		Selections.GetItem()->SelectedActor->MoveTo(loc);
		Selections.GetItem()->SelectedActor->SetOrientation(yaw, pitch, roll);		

		WeMatrix TM;
		if (AlignType & ALIGN_AXIS)
		{
			TM._11 = RefX.x; TM._12 = RefX.y; TM._13 = RefX.z;
			TM._21 = RefY.x; TM._22 = RefY.y; TM._23 = RefY.z;
			TM._31 = RefZ.x; TM._32 = RefZ.y; TM._33 = RefZ.z;
			TM._41 = Selections.GetItem()->SelectedActor->Location.x; 
			TM._42 = Selections.GetItem()->SelectedActor->Location.y; 
			TM._43 = Selections.GetItem()->SelectedActor->Location.z; 
			TM._44 = 1; 


			Selections.GetItem()->SelectedActor->SetTransformation(TM);				
		}

	}

}


void CMassSelection::Deselect(CSelection *selected)
{
	for (Selections.First(); !Selections.End(); Selections.Next())
	{
		if (Selections.GetItem() == selected)
		{
			SAFE_DELETE(Selections.GetItem());
			Selections.Delete();
			return;
		}
	}
}

WeActor *CMassSelection::CreateNewActor(int TypeOfNew)
{
	WeActor *newclone;
	switch (TypeOfNew)
	{
	case WETYPE_ACTOR:
		newclone = new WeActor;
		break;
	case WETYPE_LIGHT:
		newclone = new WeLight;
		break;
	case WETYPE_CLOTH:
		newclone = new WeCloth;
		break;
	case WETYPE_PARTICLESYSTEM:
		newclone = new WeParticleSystem;
		break;
	case WETYPE_PATHNODE:
		newclone = new WePathNode;
		break;
	case WETYPE_ZONE:
		newclone = new WeZone;
		break;
	case WETYPE_PORTAL:
		newclone = new WePortal;
		break;
	case ACTORTYPE_STARTINGLOCATION:
		newclone = new CStartLocation;
		break;
	default:
		newclone = (WeActor*)CPickUpFactory::Instance->MakePickUp(TypeOfNew);		
	}
	return newclone;
}


void CMassSelection::CloneSelection()
{
	CSmartList<WeActor*> NewSelection;
	WeActor *newclone;

	for (Selections.First(); !Selections.End(); Selections.Next())
	{	
		newclone = CreateNewActor(Selections.GetItem()->SelectedActor->Type);
		Selections.GetItem()->SelectedActor->Clone(newclone);
		EditorWindow->MapEditorWorld->AddActor(newclone);		
		CActionManager::Instance()->RecordAction(newclone);
		if (Selections.GetItem()->SelectedActor->Type == ACTORTYPE_STARTINGLOCATION)
			EditorWindow->MapEditorWorld->MapHeader.StartLocations.PushBack((CStartLocation*)newclone);
		NewSelection.PushBack(newclone);
	}

	DeselectAll();
	for (NewSelection.First(); !NewSelection.End(); NewSelection.Next())
	{
		MakeSelection(NewSelection.GetItem(), true);
	}
	NewSelection.Clear();
}




void CMassSelection::CloneArray(int NumObjects)
{
	if (Selections.GetItemsCount() != 2) return;
	if (NumObjects <= 0) return;

	// Find direction vector and distance between objects
	WeVector RefLoc, Direction;
	WeActor *Ac1, *Ac2, *New;

	Selections.First();
	Ac1 = Selections.GetItem()->SelectedActor;
	Selections.Next();
	Ac2 = Selections.GetItem()->SelectedActor;

	Direction = Ac2->Location - Ac1->Location;
	RefLoc = Ac2->Location;

	for (int i = 0; i < NumObjects; i++)
	{
		New = CreateNewActor(Ac2->Type);
		Ac2->Clone(New);
		CActionManager::Instance()->RecordAction(New);
		EditorWindow->MapEditorWorld->AddActor(New);		
		New->MoveTo(RefLoc + Direction);
		RefLoc = New->Location;                
		CUndo::Instance()->MakeUndoItem(UNDO_CREATE, New);
	}
}

void CMassSelection::CloneRadialArray(int NumObjects)
{
	if (Selections.GetItemsCount() != 2) return;
	if (NumObjects <= 0) return;

	// Find direction vector and distance between objects
	WeVector XAxis, YAxis, ZAxis, tmp, Direction;
	float Angle, DeltaAngle;
	WeActor *Ac1, *Ac2, *New;
	WeMatrix Rot, M, M2;

	DeltaAngle = 2.0f*D3DX_PI/(float)(NumObjects+1);
	Angle = DeltaAngle;

	Selections.First();
	Ac1 = Selections.GetItem()->SelectedActor;
	Selections.GetItem()->SelectedActor->GetLocalAxis(XAxis, YAxis, ZAxis);
	Selections.Next();
	Ac2 = Selections.GetItem()->SelectedActor;
	M2 = Ac2->TM;
	M2._41 = 0; M2._42 = 0; M2._43 = 0; 


	Direction = Ac2->Location - Ac1->Location;

	for (int i = 0; i < NumObjects; i++)
	{
		D3DXMatrixRotationAxis(&Rot, &ZAxis, Angle);
		D3DXVec3TransformNormal(&tmp, &Direction, &Rot);
		tmp = Ac1->Location + tmp;
		D3DXMatrixTranslation(&M, tmp.x, tmp.y, tmp.z);
		M = M2*Rot*M;
        
		Angle += DeltaAngle;
		New = CreateNewActor(Ac2->Type);
		Ac2->Clone(New);
		CActionManager::Instance()->RecordAction(New);

		EditorWindow->MapEditorWorld->AddActor(New);		
		New->SetTransformation(M);		
		//New->MoveTo(New->Location+tmp);
		CUndo::Instance()->MakeUndoItem(UNDO_CREATE, New);
	}
}


void CMassSelection::DeselectAll()
{
	for (Selections.First(); !Selections.End(); Selections.Delete())
	{
		SAFE_DELETE(Selections.GetItem());
	}
}



void CMassSelection::Move(int mx, int my, bool Left, bool Right, bool LockX, bool LockY, bool LocalMove)
{
	//if (RefX == 0) RefX = (float)mx;
	//if (RefY == 0) RefY = (float)my;


	// This will move only if local move is turned on
	if ((EditorWindow->Cam->GetViewMode() == CAMERA_FREE) || LocalMove)
	{
		if (Left && !Right)
		{
			for (Selections.First(); !Selections.End(); Selections.Next())
			{
				float dmx;
				float dmy;
				WeVector loc;

				//dmx = mx - RefX;
				//dmy = my - RefY;
			
				dmx = (float)mx;
				dmy = (float)my;

				loc = Selections.GetItem()->SelectedActor->Location;
				if (LockX) loc += dmx * GroupPivotAxisX / 10.0f;
				else if (LockY) loc += dmx * GroupPivotAxisY / 10.0f;
				else loc += dmx * GroupPivotAxisZ / 10.0f;
				Selections.GetItem()->SelectedActor->MoveTo(loc);

				DeltaMoving = loc - StartLocation;
			}
			MakeGroupPivot();
		}		
	}
	else
	{
		if (EditorWindow->Cam->GetViewMode() == CAMERA_TOP)
		{
			if (Left && !Right)
			{
				for (Selections.First(); !Selections.End(); Selections.Next())
				{
					float dmx;
					float dmy;
					WeVector loc;

					//dmx = mx - RefX;
					//dmy = my - RefY;

					dmx = (float)mx;
					dmy = (float)my;

					loc = Selections.GetItem()->SelectedActor->Location;
					if (!LockX) loc.x += dmx / 10.0f;
					if (!LockY) loc.z -= dmy / 10.0f;
					Selections.GetItem()->SelectedActor->MoveTo(loc);

					DeltaMoving = loc - StartLocation;
				}
				MakeGroupPivot();
			}
		}

		else if (EditorWindow->Cam->GetViewMode() == CAMERA_FRONT)
		{
			if (Left && !Right)
			{
				for (Selections.First(); !Selections.End(); Selections.Next())
				{
					float dmx;
					float dmy;
					WeVector loc;

					//dmx = mx - RefX;
					//dmy = my - RefY;

					dmx = (float)mx;
					dmy = (float)my;

					loc = Selections.GetItem()->SelectedActor->Location;
					if (!LockX) loc.x += dmx / 10.0f;
					if (!LockY) loc.y -= dmy / 10.0f;
					Selections.GetItem()->SelectedActor->MoveTo(loc);

					DeltaMoving = loc - StartLocation;
				}
				MakeGroupPivot();
			}
		}

		else if (EditorWindow->Cam->GetViewMode() == CAMERA_LEFT)
		{
			if (Left && !Right)
			{
				for (Selections.First(); !Selections.End(); Selections.Next())
				{
					float dmx;
					float dmy;
					WeVector loc;

					//dmx = mx - RefX;
					//dmy = my - RefY;				
				
					dmx = (float)mx;
					dmy = (float)my;

					loc = Selections.GetItem()->SelectedActor->Location;
					if (!LockX) loc.z -= dmx / 10.0f;
					if (!LockY) loc.y -= dmy / 10.0f;
					Selections.GetItem()->SelectedActor->MoveTo(loc);			

					DeltaMoving = loc - StartLocation;
				}
				MakeGroupPivot();
			}
		}
	} // LocalMode


	//RefX = (float)mx;
	//RefY = (float)my;
}




void CMassSelection::Rotate(int mx, int my, bool Left, bool Right, bool LockX, bool LockY, bool LocalRotate)
{
	// we cannot rotate in free camera yet
	//if (RefX == 0) RefX = (float)mx;
	//if (RefY == 0) RefY = (float)my;

	if ((EditorWindow->Cam->GetViewMode() == CAMERA_FREE) || LocalRotate)
	{
		if (Left && !Right) 
		{	
			// First make trans matrix for GroupPivot
		    float r1;
			WeMatrix M,M2,M3;

			//r1 = (mx - RefX) / 100.0f;
			r1 = (float)mx / 100.0f;

			if (!LockX && !LockY) { D3DXMatrixRotationAxis(&M, &GroupPivotAxisZ, r1); DeltaRotation.z = (StartRotation.z += r1) * 180.0f / D3DX_PI; }
			if (LockX && !LockY)  { D3DXMatrixRotationAxis(&M, &GroupPivotAxisX, r1); DeltaRotation.x = (StartRotation.x += r1) * 180.0f / D3DX_PI; }
			if (!LockX && LockY)  { D3DXMatrixRotationAxis(&M, &GroupPivotAxisY, r1); DeltaRotation.y = (StartRotation.y += r1) * 180.0f / D3DX_PI; }

			M._41 = 0;
			M._42 = 0;
			M._43 = 0;

			for (Selections.First(); !Selections.End(); Selections.Next())
			{	
				Selections.GetItem()->SelectedActor->GetTransformation(M3);
				M3._41 = 0;
				M3._42 = 0;
				M3._43 = 0;				
				M2 = M3 * M;
				D3DXMatrixTranslation(&M3, Selections.GetItem()->SelectedActor->Location.x, Selections.GetItem()->SelectedActor->Location.y, Selections.GetItem()->SelectedActor->Location.z);
				//M2 = M2 * M3;
				M2._41 = Selections.GetItem()->SelectedActor->Location.x;
				M2._42 = Selections.GetItem()->SelectedActor->Location.y;
				M2._43 = Selections.GetItem()->SelectedActor->Location.z;

				Selections.GetItem()->SelectedActor->SetTransformation(M2);
			}			
		}
	}
	else
	{
		if (EditorWindow->Cam->GetViewMode() == CAMERA_TOP) 
		{
			if (Left && !Right) 
			{	
				// First make trans matrix for GroupPivot
				float r1;
				float y,p,r;
				WeMatrix M, M2, TM, Fin;
				WeVector d;

				//r1 = (mx - RefX) / 1000.0f;
				r1 = (float)mx / 1000.0f;
				y = r1;
				p = 0;
				r = 0;
				//D3DXMatrixRotationYawPitchRoll(&M, y,p,r);
				D3DXMatrixRotationY(&M, y);

				DeltaRotation.x = (StartRotation.x += r1) * 180 / D3DX_PI;

				for (Selections.First(); !Selections.End(); Selections.Next())
				{			
					TM = Selections.GetItem()->SelectedActor->TM;					
					TM._41 = 0;
					TM._42 = 0;
					TM._43 = 0;


					d = Selections.GetItem()->SelectedActor->Location;
					d = d - GroupPivot;
					D3DXVec3TransformNormal(&d, &d, &M);
					Fin = TM*M;
					d = GroupPivot + d;
					Fin._41 = d.x;
					Fin._42 = d.y;
					Fin._43 = d.z;
                    
					Selections.GetItem()->SelectedActor->SetTransformation(Fin);					
				}			
			}        	
		}

		else if (EditorWindow->Cam->GetViewMode() == CAMERA_FRONT) 
		{
			if (Left && !Right) 
			{	
				// First make trans matrix for GroupPivot
				float r1;
				float y,p,r;
				WeMatrix M, TM, Fin;
				WeVector d;

				//r1 = (mx - RefX) / 1000.0f;
				r1 = (float)mx / 1000.0f;
				y = 0;
				p = 0;
				r = r1;
				D3DXMatrixRotationZ(&M, r);		

				DeltaRotation.z = (StartRotation.z += r1) * 180 / D3DX_PI;

				for (Selections.First(); !Selections.End(); Selections.Next())
				{			
					TM = Selections.GetItem()->SelectedActor->TM;
					TM._41 = 0;
					TM._42 = 0;
					TM._43 = 0;
                    
					d = Selections.GetItem()->SelectedActor->Location;
					d = d - GroupPivot;
					D3DXVec3TransformNormal(&d, &d, &M);
					Fin = TM*M;

					d = GroupPivot+d;
					Fin._41 = d.x;
					Fin._42 = d.y;
					Fin._43 = d.z;

					Selections.GetItem()->SelectedActor->SetTransformation(Fin);					
				}			
			}        	
		}

		else if (EditorWindow->Cam->GetViewMode() == CAMERA_LEFT) 
		{
			if (Left && !Right) 
			{	
				// First make trans matrix for GroupPivot
				float r1;
				float y,p,r;
				WeMatrix M, TM, Fin;
				WeVector d;

				//r1 = (mx - RefX) / 1000.0f;
				r1 = (float)mx / 1000.0f;
				y = 0;
				p = r1;
				r = 0;
				//D3DXMatrixRotationYawPitchRoll(&M, y,p,r);			
				D3DXMatrixRotationX(&M, p);			

				DeltaRotation.y = (StartRotation.y += r1) * 180 / D3DX_PI;

				for (Selections.First(); !Selections.End(); Selections.Next())
				{			
					TM = Selections.GetItem()->SelectedActor->TM;
					TM._41 = 0;
					TM._42 = 0;
					TM._43 = 0;
                    
					d = Selections.GetItem()->SelectedActor->Location;
					d = d - GroupPivot;
					D3DXVec3TransformNormal(&d, &d, &M);
					d = GroupPivot+d;
					Fin = TM*M;
					Fin._41 = d.x;
                    Fin._42 = d.y;
					Fin._43 = d.z;
					
					Selections.GetItem()->SelectedActor->SetTransformation(Fin);					
				}			
			}        	
		}
	}

	//RefX = (float)mx;
	//RefY = (float)my;
}



void CMassSelection::DoSelection(int x, int y, bool AddToSelection)
{
	WeRaycastHit hit;
	WeMatrix _ProjMatrix = EditorWindow->Cam->Projection;
	WeMatrix _ViewMatrix = EditorWindow->Cam->View;
	WeMatrix m;
	WeVector RayLocation;
	WeVector RayDirection;

	if (EditorWindow->Cam->GetViewMode() == CAMERA_FREE)
	{

		float rX = ((2.0f * (float)x / (float)WeRenderDevice::Instance()->GetResWidth()) - 1.0f) / _ProjMatrix._11;
		float rY = -(((2.0f * (float)y / (float)WeRenderDevice::Instance()->GetResHeight()) - 1.0f) / _ProjMatrix._22);
		float rZ = 1.0f;

		D3DXMatrixInverse(&m, NULL, &_ViewMatrix);

		RayDirection.x = rX * m._11 + rY * m._21 + rZ * m._31;
		RayDirection.y = rX * m._12 + rY * m._22 + rZ * m._32;
		RayDirection.z = rX * m._13 + rY * m._23 + rZ * m._33;

		D3DXVec3Normalize(&RayDirection, &RayDirection);

		RayLocation.x = m._41;
		RayLocation.y = m._42;
		RayLocation.z = m._43;

		RayLocation += RayDirection;

	}


	// for ortho views
	// -------------------------------------------------------------
	if (EditorWindow->Cam->GetViewMode() == CAMERA_TOP)
	{
		RayLocation =  EditorWindow->Cam->Location;

		float tx,ty;
		tx = EditorWindow->Cam->OrthoW*x / EditorWindow->GetClientWidth();
		ty = EditorWindow->Cam->OrthoH*y / EditorWindow->GetClientHeight();

		tx -= EditorWindow->Cam->OrthoW/2.0f;
		ty -= EditorWindow->Cam->OrthoH/2.0f;

		RayLocation.x += tx;
		RayLocation.z -= ty;

		RayDirection.x = 0;
		RayDirection.y = -1.0f;
		RayDirection.z = 0;		

	}

	if (EditorWindow->Cam->GetViewMode() == CAMERA_FRONT)
	{
		RayLocation =  EditorWindow->Cam->Location;

		float tx,ty;
		tx = EditorWindow->Cam->OrthoW*x / EditorWindow->GetClientWidth();
		ty = EditorWindow->Cam->OrthoH*y / EditorWindow->GetClientHeight();

		tx -= EditorWindow->Cam->OrthoW/2.0f;
		ty -= EditorWindow->Cam->OrthoH/2.0f;

		RayLocation.x += tx;
		RayLocation.y -= ty;

		RayDirection.x = 0;
		RayDirection.y = 0.0f;
		RayDirection.z = 1.0;		

	}

	if (EditorWindow->Cam->GetViewMode() == CAMERA_LEFT)
	{
		RayLocation =  EditorWindow->Cam->Location;

		float tx,ty;
		tx = EditorWindow->Cam->OrthoW*x / EditorWindow->GetClientWidth();
		ty = EditorWindow->Cam->OrthoH*y / EditorWindow->GetClientHeight();

		tx -= EditorWindow->Cam->OrthoW/2.0f;
		ty -= EditorWindow->Cam->OrthoH/2.0f;

		RayLocation.z -= tx;
		RayLocation.y -= ty;

		RayDirection.x = 1.0;
		RayDirection.y = 0.0f;
		RayDirection.z = 0.0;		
	}

	// --------------------------------------------




	if(WePhysics::Instance()->RaycastClosest(RayLocation, RayDirection, WE_SHAPE_TYPE_ALL, hit, 7, 50000))
	{
		MakeSelection(hit.ActorHit, AddToSelection);
		return;
	}

	DeselectAll();
}


void CMassSelection::DeleteSelected()
{
	for (Selections.First(); !Selections.End(); Selections.Next())
	{
		CActionManager::Instance()->RemoveRecordedAction(Selections.GetItem()->SelectedActor);
		// Ako slucajno brisemo startnu lokaciju onda je izbaci i iz mapheader-a
		if (Selections.GetItem()->SelectedActor->Type == ACTORTYPE_STARTINGLOCATION)
		{
			for (EditorWindow->MapEditorWorld->MapHeader.StartLocations.First(); !EditorWindow->MapEditorWorld->MapHeader.StartLocations.End(); EditorWindow->MapEditorWorld->MapHeader.StartLocations.Next())
			{
				if (EditorWindow->MapEditorWorld->MapHeader.StartLocations.GetItem() == Selections.GetItem()->SelectedActor)
				{
					EditorWindow->MapEditorWorld->MapHeader.StartLocations.Delete();					
					break;
				}
			}			
		}
			
		SAFE_DELETE(Selections.GetItem()->SelectedActor);
		SAFE_DELETE(Selections.GetItem());	
	}
	Selections.Clear();
}


void CMassSelection::MakeSelection(WeActor *actorhit, bool AddToSelection)
{
	if (!actorhit->Model) return;

	bool Removing = false;

	switch (actorhit->Type)
	{
	case WETYPE_ACTOR:
		if (!SelectPrefabs)
			return;
		break;

	case WETYPE_LIGHT:
		if (!SelectLights)
			return;
		break;

	case WETYPE_PATHNODE:
		if (!SelectPathNodes)
			return;
		break;

	case ACTORTYPE_STARTINGLOCATION:
		if (!SelectPathNodes)
			return;
		break;

	case WETYPE_PARTICLESYSTEM:
		if (!SelectParticleSystems)
			return;
		break;
	}

	// Check to see if this actorhit is maybe already selected.

	for (Selections.First(); !Selections.End(); Selections.Next())
	{
		if (Selections.GetItem()->SelectedActor == actorhit)
		{
			// if we are holding shift then remove this from selections
			if (AddToSelection)
			{
				Deselect(Selections.GetItem());
				Removing = true;
			}
			break;
		}
	}

	if (!Removing)
	{
		CSelection *newSelection;
		newSelection = new CSelection(actorhit);
		//EditorWindow->MapEditorWorld->AddActor(newSelection);

		if (!AddToSelection)
			DeselectAll();

		Selections.PushBack(newSelection);

		MakeGroupPivot();

		StartRotation = WeVector(0.0f,0.0f,0.0f);
	}

	if (Selections.GetItemsCount() == 1)
	{
		Selections.First();		
		if (Selections.GetItem()->GetSelectionType() == ShownPropertiesType)
			ShowSelectionProperties();
		else
			HideSelectionProperties();
	}
	else
		HideSelectionProperties();


}



void CMassSelection::ShowSelectionProperties()
{
	if (Selections.GetItemsCount() == 1)
	{
		CUndo::Instance()->MakeUndoItem(UNDO_ACTORSTATECHANGE);
		Selections.First();		

		IS_PICKUP(Selections.GetItem()->GetSelectionType())
		{
            CDialogPickupProperties::Instance()->Show((CPickUp*)Selections.GetItem()->SelectedActor);
			return;
		}
		
		switch (Selections.GetItem()->GetSelectionType())
		{
		case WETYPE_ACTOR: 
			CDialogActor::Instance()->Show(Selections.GetItem()->SelectedActor);
			ShownPropertiesType = Selections.GetItem()->GetSelectionType();
			break;

		case WETYPE_LIGHT:
			CDialogLight::Instance()->Show((WeLight *)Selections.GetItem()->SelectedActor);
			ShownPropertiesType = Selections.GetItem()->GetSelectionType();
			break;

		case WETYPE_CLOTH:
			CDialogCloth::Instance()->Show((WeCloth*)Selections.GetItem()->SelectedActor);
			ShownPropertiesType = Selections.GetItem()->GetSelectionType();
			break;

		case WETYPE_PARTICLESYSTEM:
			CDialogParticleSystem::Instance()->Show((WeParticleSystem *)Selections.GetItem()->SelectedActor);
			ShownPropertiesType = Selections.GetItem()->GetSelectionType();
			break;

		case WETYPE_ZONE:
			CDialogZone::Instance()->Show((WeZone *)Selections.GetItem()->SelectedActor);
			ShownPropertiesType = Selections.GetItem()->GetSelectionType();
			break;

		case WETYPE_PORTAL:
			CDialogPortal::Instance()->Show((WePortal*)Selections.GetItem()->SelectedActor);
			ShownPropertiesType = Selections.GetItem()->GetSelectionType();
			break;

		//case PICKUPTYPE_GLOBAL:
			//CDialogPickupProperties::Instance->Show((CPickUp*)Selections.GetItem()->SelectedActor);
			//break;

		case WETYPE_PATHNODE:		
			CDialogPathNode::Instance()->Show((WePathNode *)Selections.GetItem()->SelectedActor);
			ShownPropertiesType = Selections.GetItem()->GetSelectionType();
			break;
			
		case ACTORTYPE_STARTINGLOCATION:
			CDialogStartLoacationProperties::Instance()->ShowDlg((CStartLocation *)Selections.GetItem()->SelectedActor);
			ShownPropertiesType = Selections.GetItem()->GetSelectionType();
			break;	
		}
		
	} else
	{

		CSmartList <WeActor *> SelActors;
		Selections.First();
		if (Selections.GetItem()->GetSelectionType() == WETYPE_ACTOR)
		{
			for (Selections.First(); !Selections.End(); Selections.Next())
			{
				SelActors.PushBack(Selections.GetItem()->SelectedActor);
			}
						
			CDialogActor::Instance()->Show(SelActors);
			ShownPropertiesType = WETYPE_ACTOR;
		}
	}
}



void CMassSelection::HideSelectionProperties()
{
	if (CDialogActor::Instance()->GetVisible()) CDialogActor::Instance()->Hide();
	if (CDialogPickupProperties::Instance()->GetVisible()) CDialogPickupProperties::Instance()->Hide();
	if (CDialogLight::Instance()->GetVisible()) CDialogLight::Instance()->Hide();
	if (CDialogCloth::Instance()->GetVisible()) CDialogCloth::Instance()->Hide();
	if (CDialogParticleSystem::Instance()->GetVisible()) CDialogParticleSystem::Instance()->Hide();
	if (CDialogZone::Instance()->GetVisible()) CDialogZone::Instance()->Hide();
	if (CDialogPortal::Instance()->GetVisible()) CDialogPortal::Instance()->Hide();
	if (CDialogPathNode::Instance()->GetVisible()) CDialogPathNode::Instance()->Hide();
	if (CDialogStartLoacationProperties::Instance()->GetVisible()) CDialogStartLoacationProperties::Instance()->Hide();
}



void CMassSelection::MakeGroupPivot()
{
	WeVector Sum, SumX, SumY, SumZ;
	WeVector tmpX, tmpY, tmpZ;
    Sum.x = 0; Sum.y = 0; Sum.z = 0;
    SumX.x = 0; SumX.y = 0; SumX.z = 0;
    SumY.x = 0; SumY.y = 0; SumY.z = 0;
    SumZ.x = 0; SumZ.y = 0; SumZ.z = 0;

	for (Selections.First(); !Selections.End(); Selections.Next())
	{
		Selections.GetItem()->SelectedActor->GetLocalAxis(tmpX, tmpY, tmpZ);
		SumX += tmpX;
		SumY += tmpY;
		SumZ += tmpZ;
		Sum += Selections.GetItem()->SelectedActor->Location;
	}
	Sum /= (float)Selections.GetItemsCount();
	SumX /= (float)Selections.GetItemsCount();
	SumY /= (float)Selections.GetItemsCount();
	SumZ /= (float)Selections.GetItemsCount();
	GroupPivot = Sum;
	GroupPivotAxisX = SumX;
	GroupPivotAxisY = SumY;
	GroupPivotAxisZ = SumZ;

}




void CMassSelection::RotateCW()
{
	float r1;
	WeMatrix M,M2,M3;


	r1 = D3DX_PI / 2.0f;

	D3DXMatrixRotationAxis(&M, &GroupPivotAxisZ, r1);			

	M._41 = 0;
	M._42 = 0;
	M._43 = 0;

	for (Selections.First(); !Selections.End(); Selections.Next())
	{	
		Selections.GetItem()->SelectedActor->GetTransformation(M3);
		M3._41 = 0;
		M3._42 = 0;
		M3._43 = 0;				
		M2 = M3 * M;
		D3DXMatrixTranslation(&M3, Selections.GetItem()->SelectedActor->Location.x, Selections.GetItem()->SelectedActor->Location.y, Selections.GetItem()->SelectedActor->Location.z);
		//M2 = M2 * M3;
		M2._41 = Selections.GetItem()->SelectedActor->Location.x;
		M2._42 = Selections.GetItem()->SelectedActor->Location.y;
		M2._43 = Selections.GetItem()->SelectedActor->Location.z;

		Selections.GetItem()->SelectedActor->SetTransformation(M2);
	}			
}



void CMassSelection::RotateCWYAxis()
{
	float r1;
	WeMatrix M,M2,M3;


	r1 = D3DX_PI / 2.0f;

	D3DXMatrixRotationAxis(&M, &GroupPivotAxisY, r1);			

	M._41 = 0;
	M._42 = 0;
	M._43 = 0;

	for (Selections.First(); !Selections.End(); Selections.Next())
	{	
		Selections.GetItem()->SelectedActor->GetTransformation(M3);
		M3._41 = 0;
		M3._42 = 0;
		M3._43 = 0;				
		M2 = M3 * M;
		D3DXMatrixTranslation(&M3, Selections.GetItem()->SelectedActor->Location.x, Selections.GetItem()->SelectedActor->Location.y, Selections.GetItem()->SelectedActor->Location.z);
		//M2 = M2 * M3;
		M2._41 = Selections.GetItem()->SelectedActor->Location.x;
		M2._42 = Selections.GetItem()->SelectedActor->Location.y;
		M2._43 = Selections.GetItem()->SelectedActor->Location.z;

		Selections.GetItem()->SelectedActor->SetTransformation(M2);
	}			
}