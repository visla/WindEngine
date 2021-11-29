/*

    CEditorGrid.cpp

	
	Autor: Nebojsa Vislavski
	Date: 20.12.2005.
	Version: 1.0


	This is grid used in editors

*/

#include <CEditorGrid.h>
#include <WeWorld.h>
#include <WeModelManager.h>
#include <WeShaderManager.h>
#include <WeMaterial.h>
#include <WeMesh.h>
#include <WeShaderManager.h>
#include <WeMaterial.h>



void CEditorGrid::Initialize(WeWorld* WorldToBeIn)
{
	Location = WeVector(0,0,0);
	Model = WeModelManager::Instance()->Load("Models.Editor.Grid");

	Serializable = false;

	//for (Model->Mesh->Materials.First(); !Model->Mesh->Materials.End(); Model->Mesh->Materials.Next())
	//{			
	//	Model->Mesh->Materials.GetItem()->Type = WEMATERIALTYPE_ALPHABLENDED;
	//	//Model->Mesh->Materials.GetItem()->Shader = WeShaderManager::Instance()->Load("alldata.misko.grid", false, true);
	//}

	D3DXMatrixIdentity(&TM);
	WorldToBeIn->AddActor(this);
}


