/*
       File: CSelection.cpp
	   Author: Tomislav Kukic
	   Date: 05.12.2005
	   Version: 1.0
*/


#include <d3dx9.h>
#include <WeMesh.h>
#include <WeStaticModel.h>
#include <WeMaterialManager.h>
#include <WeActor.h>
#include <WeModelManager.h>
#include <WeMeshManager.h>
#include <WePhysics.h>
#include <WeRenderDevice.h>
#include <WeCamera.h>
#include <WeVector.h>
#include <WeMatrix.h>
#include <CMapEditorWindow.h>
#include <CEditorMap.h>
#include <CMap.h>
#include <CSelection.h>
#include <ActorTags.h>
#include <CMapEditorCamera.h>
#include <CDialogActor.h>
#include <CDialogPathNode.h>
#include <CDialogLight.h>
#include <CDialogParticleSystem.h>



CSelection::CSelection(WeActor* Actor)
{
	Type = 0;

	SelectedActor = Actor;
	if (SelectedActor)
	{
		if (SelectedActor->Model)
			SelectedActor->Model->SetMaterial("Materials.Editor.Selection", SelectionPreset);
		++SelectedActor->SelectedCount;
	}
}



CSelection::~CSelection()
{
	if (SelectedActor)
	{
		--SelectedActor->SelectedCount;
	}
	SelectedActor = NULL;
}



/*
WeActor* CSelection::GetSelectedActor()
{
	return SelectedActor;
}
*/



void CSelection::UpdateProperties()
{
	switch (GetSelectionType())
	{
	case WETYPE_ACTOR:
		CDialogActor::Instance()->Update(SelectedActor);
		break;
		
	case WETYPE_LIGHT:
		CDialogLight::Instance()->Update((WeLight*)SelectedActor);
		break;

	case WETYPE_PARTICLESYSTEM:
		CDialogParticleSystem::Instance()->Update((WeParticleSystem*)SelectedActor);
		break;

	case WETYPE_PATHNODE:
		CDialogPathNode::Instance()->Update((WePathNode*)SelectedActor);
		break;
	}
}



/*
void CSelection::SetSelectedActor(WeActor* Actor)
{
	if (!Actor) return;

    if (Model)
	{
		WeModelManager::Instance()->Destroy(Model);
		SAFE_DELETE(Model);
	}


	SelectedActor = Actor;
	BSphere = Actor->BSphere;
	BBox = Actor->BBox;

	//Model = WeModelManager::Instance()->Clone(Actor->Model, true);
	//Model->SetMaterial("Materials.Editor.Selection", MaterialPreset);
    
	MaterialPreset = SelectionPreset;
	Model = Actor->Model;

	if (Model)
		Model->SetMaterial("Materials.Editor.Selection", MaterialPreset);

	//for(Model->Mesh->Materials[0].First();
	//	!Model->Mesh->Materials[0].End();
	//	Model->Mesh->Materials[0].Next())
	//{
	//	Model->Mesh->Materials[0].GetItem() = WeMaterialManager::Instance()->Load("Materials.Editor.Selection");
	//}

	//Location = Actor->Location;
	//Direction = Actor->Direction;
	//Yaw = Actor->Yaw;
	//Pitch = Actor->Pitch;
	//Roll = Actor->Roll;
	//////Scale = WeVector(1.0,1.0,1.01);

	//WeMatrix tmpTM;
	//D3DXMatrixIdentity(&tmpTM);
	////TM = tmpTM;

	EditorWindow->MapEditorWorld->AddActor(this);
}
*/



/*
void CSelection::Tick()
{
	//Scale = SelectedActor->Scale;
	WeActor::Tick();

	//SelectedActor->GetOrientation(Yaw,Pitch, Roll);
	//Yaw = SelectedActor->Yaw;
	//Pitch = SelectedActor->Pitch;
	//Roll = SelectedActor->Roll;

	//WeActor::Tick();
	//TM = SelectedActor->TM;

	//if (Model)
		//Model->UpdateTransformations(SelectedActor->TMWithScale, SelectedActor->TMWithScale);				
}
*/



int CSelection::GetSelectionType()
{
	if (SelectedActor)
		return SelectedActor->Type;
	else
		return -1;
}
