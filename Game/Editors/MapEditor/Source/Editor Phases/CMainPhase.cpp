/*
          File: CMainPhase.cpp
		  Author: Tomislav Kukic
		  Date: 07.11.2005.
		  Version: 1.0
*/



#include <IPhase.h>
#include <WeRenderDevice.h>
#include <WeCanvas.h>
#include <WeFont.h>
#include <WeEngine.h>
#include <CMapEditorWindow.h>
#include <WeWorld.h>
#include <WeTimer.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <CMainPhase.h>
#include <WeWorld.h>
#include <WeActor.h>
#include <WeCamera.h>
#include <WeMesh.h>
#include <CEditorMap.h>
#include <CSelection.h>
#include <CMap.h>
#include <WeInput.h>
#include <WeMeshEditor.h>
#include <WeMaterialManager.h>
#include <WeStaticModel.h>
#include <CMapEditorCamera.h>
#include <WeProfiler.h>
#include <WeTextureManager.h>
#include <WePostProcess.h>
#include <CStaticString.h>
#include <WeRenderTree.h>
#include <CDialogAnimTextureEditor.h>
#include <CDialogTextureEditor.h>
#include <CDialogMaterialEditor.h>
#include <CDialogModelEditor.h>
#include <CDialogParticleSystemEditor.h>
#include <CDialogInsertObject.h>
#include <CDialogAnimatedMesh.h>
#include <CDialogMeshEditor.h>
#include <CDialogTools.h>
#include <WeAudioDevice.h>
#include <WeListener.h>

WeFont* Font = NULL;
WeFont* FontSmall = NULL;



/*
void UpdateRotations()
{
	if(MeshEditor->LocationEditor->Updated)
	{
		//Cam->Location.x = MeshEditor->LocationEditor->CamX;
		//Cam->Location.y = MeshEditor->LocationEditor->CamY;
		//Cam->Location.z = MeshEditor->LocationEditor->CamZ;

		//Actor->Yaw = MeshEditor->LocationEditor->Yaw;
		//Actor->Pitch = MeshEditor->LocationEditor->Pitch;
		//Actor->Roll = MeshEditor->LocationEditor->Roll;

		//MeshEditor->LocationEditor->Updated = false;
	}else{
		//Cam->Location.z += MeshEditor->MouseZ;
	
		//MeshEditor->Actor->Yaw += MeshEditor->MouseX;
		//MeshEditor->Actor->Pitch += MeshEditor->MouseY;


		//MeshEditor->LocationEditor->Yaw = MeshEditor->Actor->Yaw;
		//MeshEditor->LocationEditor->Pitch = MeshEditor->Actor->Pitch;
		//MeshEditor->LocationEditor->Roll = MeshEditor->Actor->Roll;

		//MeshEditor->LocationEditor->CamZ = Cam->Location.z;
		//MeshEditor->LocationEditor->CamX = Cam->Location.x;
		//MeshEditor->LocationEditor->CamY = Cam->Location.y;
	}
	//MeshEditor->MouseX = 0;
	//MeshEditor->MouseY = 0;
	//MeshEditor->MouseZ = 0;	
}
*/



void MapEditorRendering()
{
	//WePostProcess::Instance()->Initialize();
	//WePostProcess::Instance()->TurnOn();

	WePostProcess::Instance()->BeginScreenCapture();
	//EditorWindow->MapEditorWorld->Unload();
	//if (EditorWindow->MapEditorWorld->IsLoaded())
	EditorWindow->MapEditorWorld->Render(EditorWindow->Cam);
	WePostProcess::Instance()->EndScreenCapture();
	WePostProcess::Instance()->PostRender();

	if (WeAudioDevice::Instance()->Listener)
		WeAudioDevice::Instance()->Listener->SetPosition(EditorWindow->Cam->Location);	

	WeActor *FirstActor, *Iterator;

	FirstActor = EditorWindow->MapEditorWorld->Actors;

	for (Iterator = FirstActor->NextActor; Iterator != FirstActor; Iterator = Iterator->NextActor)
	{
		if (Iterator->Editor_Renderable)
		{
			String info = Iterator->GetInfo();
			if (info.GetLen() != 0)
			{
				static WeFont *pFont = NULL;

				if (!pFont)
					pFont = WeCanvas::Instance()->MakeFont("Arial", 12);

				D3DXVECTOR4 tmpOutV;
				D3DXVec3Transform(&tmpOutV, &Iterator->Location, &EditorWindow->Cam->ViewProj);

				if ((tmpOutV.z >= 0.0f) && (tmpOutV.z <= EditorWindow->Cam->GetFarClip()))
				{
					float x, y;
					x = ((tmpOutV.x / tmpOutV.w) + 1.0f) / 2.0f;
					y = ((-tmpOutV.y / tmpOutV.w) + 1.0f) / 2.0f;

					int xx, yy;
					xx = (int)((x + 0.01) * WeRenderDevice::Instance()->GetResWidth());
					yy = (int)((y - 0.05) * WeRenderDevice::Instance()->GetResHeight());

					WeCanvas::Instance()->Draw2DTextOutlined(pFont, info, xx, yy, 200, 50, WeColor(0xFFFFFF00), WeColor(0xFF000000));
				}
			}
		}
	}


	if (EditorWindow->Selecting)
	{
		int x, y, w, h;

		if (EditorWindow->MouseDeltaX < 0)
			x = EditorWindow->MouseOriginX + EditorWindow->MouseDeltaX;
		else
			x = EditorWindow->MouseOriginX;
		x = x * WeRenderDevice::Instance()->GetResWidth() / EditorWindow->GetWidth();

		if (EditorWindow->MouseDeltaY < 0)
			y = EditorWindow->MouseOriginY + EditorWindow->MouseDeltaY;
		else
			y = EditorWindow->MouseOriginY;
		y = y * WeRenderDevice::Instance()->GetResHeight() / EditorWindow->GetHeight();

		w = abs(EditorWindow->MouseDeltaX);
		w = w * WeRenderDevice::Instance()->GetResWidth() / EditorWindow->GetWidth();

		h = abs(EditorWindow->MouseDeltaY);
		h = h * WeRenderDevice::Instance()->GetResHeight() / EditorWindow->GetHeight();

		WeTexture *TexFiller = WeTextureManager::Instance()->Load("MapEditor.Textures.DefaultWhite1x1", true);

		WeCanvas::Instance()->DrawRectangle(x, y, w, 1, 0xFFFFFFFF, TexFiller);
		WeCanvas::Instance()->DrawRectangle(x, y + h - 1, w, 1, 0xFFFFFFFF, TexFiller);
		WeCanvas::Instance()->DrawRectangle(x, y, 1, h, 0xFFFFFFFF, TexFiller);
		WeCanvas::Instance()->DrawRectangle(x + w - 1, y, 1, h, 0xFFFFFFFF, TexFiller);
	}

	//CSmartList<CSelection*> *Selections;
	//Selections = &EditorWindow->Selections.Selections;

	//for (Selections->First(); !Selections->End(); Selections->Next())
	//{
	//	float left, right, bottom, top;
	//	EditorWindow->GetBBoxViewProjRect(Selections->GetItem()->SelectedActor->BSphere, EditorWindow->Cam, left, right, bottom, top);

	//	int x1 = (int)((left + 1.0f) / 2.0f * WeRenderDevice::Instance()->GetResWidth());
	//	int x2 = (int)((right + 1.0f) / 2.0f * WeRenderDevice::Instance()->GetResWidth());
	//	int y1 = (int)((1.0f - top) / 2.0f * WeRenderDevice::Instance()->GetResHeight());
	//	int y2 = (int)((1.0f - bottom) / 2.0f * WeRenderDevice::Instance()->GetResHeight());

	//	WeTexture *TexFiller = WeTextureManager::Instance()->Load("MapEditor.Textures.DefaultWhite1x1", true);

	//	WeCanvas::Instance()->DrawRectangle(x1, y1, x2 - x1 + 1, 1, 0xFF99FFFF, TexFiller);
	//	WeCanvas::Instance()->DrawRectangle(x1, y2, x2 - x1 + 1, 1, 0xFF99FFFF, TexFiller);
	//	WeCanvas::Instance()->DrawRectangle(x1, y1, 1, y2 - y1 + 1, 0xFF99FFFF, TexFiller);
	//	WeCanvas::Instance()->DrawRectangle(x2, y1, 1, y2 - y1 + 1, 0xFF99FFFF, TexFiller);
	//}


	if(!Font)
	{
		Font = WeCanvas::Instance()->MakeFont("Arial",20);
	}

	if(!FontSmall)
	{
		FontSmall = WeCanvas::Instance()->MakeFont("Arial",15);
	}

	String Final;
	StaticString <1000> tmp;
	sprintf(tmp, "FPS: %g, Objects Visible: %d SB: %d\n\n", WeTimer::Instance()->GetFPS(), WeProfiler::Instance()->BatchItemsRendered + WeProfiler::Instance()->AlphaBatchItemsRendered, WeProfiler::Instance()->ShaderBegins);
	Final = tmp;

	if (EditorWindow->Cam->GetViewMode() == CAMERA_FREE)	 
		sprintf(tmp,"Look: CAMERA \n\n");
	if (EditorWindow->Cam->GetViewMode() == CAMERA_TOP)	 
		sprintf(tmp,"Look: TOP \n\n");
	if (EditorWindow->Cam->GetViewMode() == CAMERA_LEFT)	 
		sprintf(tmp,"Look: LEFT \n\n");
	if (EditorWindow->Cam->GetViewMode() == CAMERA_FRONT)	 
		sprintf(tmp,"Look: FRONT \n\n");
	
	Final = Final + tmp;
	tmp = "";

	if (EditorWindow->Moving)
	{
		sprintf(tmp, "[MOVING]\n");
		char tempCHR[100];
		sprintf(tempCHR,"Moved  for: X:%.1f Y:%.1f Z:%.1f",EditorWindow->Selections.DeltaMoving.x,EditorWindow->Selections.DeltaMoving.y,EditorWindow->Selections.DeltaMoving.z);
		WeCanvas::Instance()->Draw2DTextOutlined(Font,tempCHR,WeRenderDevice::Instance()->GetResWidth()/2-130,WeRenderDevice::Instance()->GetResHeight()-25,400,20,WeColor(0xFFFFFF00), WeColor(0xFF000000));
	}
	static float Rx, Ry, Rz;
	static float pRx, pRy, pRz;

	if (EditorWindow->Rotating)
	{
		sprintf(tmp, "[ROTATING]\n");
		char tempCHR[100];

		sprintf(tempCHR,"Rotated: Yaw:%.1f Pitch:%.1f Roll:%.1f", EditorWindow->Selections.DeltaRotation.x, EditorWindow->Selections.DeltaRotation.y, EditorWindow->Selections.DeltaRotation.z);
		WeCanvas::Instance()->Draw2DTextOutlined(Font,tempCHR,WeRenderDevice::Instance()->GetResWidth()/2-130,WeRenderDevice::Instance()->GetResHeight()-25,400,20,WeColor(0xFFFFFF00), WeColor(0xFF000000));
	}

	if (EditorWindow->LocalMove && EditorWindow->Moving)
		sprintf(tmp, "[MOVING LOCAL]\n");
	if (EditorWindow->LocalRotate && EditorWindow->Rotating)
		sprintf(tmp, "[ROTATING LOCAL]\n");

	Final = Final + tmp;
	tmp = "";

	if (!EditorWindow->LockedX && !EditorWindow->LockedY && (EditorWindow->LocalMove || EditorWindow->LocalRotate))
		sprintf(tmp, "[Z-axis] \n");

	if (EditorWindow->LockedX && !EditorWindow->LocalMove) 
		sprintf(tmp, "Locked: [Primary axis] \n ");
	if (EditorWindow->LockedX && (EditorWindow->LocalMove || EditorWindow->LocalRotate)) 
		sprintf(tmp, "[X Axis] \n ");

	if (EditorWindow->LockedY && !EditorWindow->LocalMove) 
		sprintf(tmp, "Locked: [Secondary axis]  \n");
	if (EditorWindow->LockedY && (EditorWindow->LocalMove || EditorWindow->LocalRotate)) 
		sprintf(tmp, "[Y Axis]  \n");

	Final = Final + tmp;
	tmp = "";

	if (EditorWindow->Cam->GetViewMode() == CAMERA_TOP)
		WeCanvas::Instance()->DrawRectangle(10, EditorWindow->GetClientHeight()-150, 150,150, D3DCOLOR_XRGB(255,255,255), WeTextureManager::Instance()->Load("topaxis.dds"));
	if (EditorWindow->Cam->GetViewMode() == CAMERA_FRONT)
		WeCanvas::Instance()->DrawRectangle(10, EditorWindow->GetClientHeight()-150, 150,150, D3DCOLOR_XRGB(255,255,255), WeTextureManager::Instance()->Load("frontaxis.dds"));
	if (EditorWindow->Cam->GetViewMode() == CAMERA_LEFT)
		WeCanvas::Instance()->DrawRectangle(10, EditorWindow->GetClientHeight()-150, 150,150, D3DCOLOR_XRGB(255,255,255), WeTextureManager::Instance()->Load("leftaxis.dds"));

	if (CActionManager::Instance()->Recording())
	{
		int x;
		x = WeRenderDevice::Instance()->GetResWidth() - 250;
		WeCanvas::Instance()->Draw2DTextOutlined(Font, (String)"ACTION RECORDING...", x,10,200,100,WeColor(0xFFFFFF00), WeColor(0xFF000000));
	};

	WeCanvas::Instance()->Draw2DTextOutlined(Font, tmp, 10,10,600,200,WeColor(0xFFFFFF00), WeColor(0xFF000000));

	WeCanvas::Instance()->Draw2DTextOutlined(Font, Final, 10,10,600,200,WeColor(0xFFFFFF00), WeColor(0xFF000000));

    char tmpScrollSPD[100];
	int tmpProcenat = (int)(EditorWindow->Cam->ScrollSpeed);
	sprintf(tmpScrollSPD,"Scrolling: %d %%",tmpProcenat);
	WeCanvas::Instance()->Draw2DTextOutlined(Font,tmpScrollSPD,WeRenderDevice::Instance()->GetResWidth()-130,10,130,20,WeColor(0xFFFFFF00), WeColor(0xFF000000));



    char ss[1000];	
	String stmp;
	for (WeProfiler::Instance()->ShadersActive.First(); !WeProfiler::Instance()->ShadersActive.End(); WeProfiler::Instance()->ShadersActive.Next())
	{
		sprintf(ss,"%s\n", WeProfiler::Instance()->ShadersActive.GetItem());
		stmp = stmp + ss;
	  
	//	WeProfiler::Instance()->RenderTreeNodesVisible, EditorWindow->MapEditorWorld->RenderTree->GetNodesCount(), EditorWindow->MapEditorWorld->RenderTree->GetProcessedDepth(), WeProfiler::Instance()->tmp,
	//	WeProfiler::Instance()->PolygonsRendered, WeProfiler::Instance()->ShaderBegins);

	}
	sprintf(ss, "\nPolygons Rendered: %d, TreeNodes: %d\n", WeProfiler::Instance()->PolygonsRendered, WeProfiler::Instance()->RenderTreeNodesVisible);
	stmp = stmp + ss;

	//sprintf(ss,"RenderTree Nodes Visible: %d\nNodes Count: %d, MaxDepth: %d, Contains: %d\
	//		   \nPolygons rendererd: %d, Shader begin: %d",
	//	WeProfiler::Instance()->RenderTreeNodesVisible, EditorWindow->MapEditorWorld->RenderTree->GetNodesCount(), EditorWindow->MapEditorWorld->RenderTree->GetProcessedDepth(), WeProfiler::Instance()->tmp,
	//	WeProfiler::Instance()->PolygonsRendered, WeProfiler::Instance()->ShaderBegins);
	//sprintf(ss,"Visible Zones: %d:",
	//	WeProfiler::Instance()->ZonesVisible);
	//for (WeProfiler::Instance()->Zones.First(); !WeProfiler::Instance()->Zones.End(); WeProfiler::Instance()->Zones.Next())
	//	sprintf(ss, "%s %s", ss, WeProfiler::Instance()->Zones.GetItem().GetString());

	if (EditorWindow->ShowInfo)
		WeCanvas::Instance()->Draw2DTextOutlined(Font,stmp,500,300,1300,400,WeColor(0xFFFFFF00), WeColor(0xFF000000));


	if(EditorWindow->BakingLights)	
	{
		WeCanvas::Instance()->Draw2DTextOutlined(Font,"BAKING LIGHTS !!!",400,300,700,200,WeColor(0xFFFFFF00), WeColor(0xFF000000));
	}


	int i = 1;
	for (EditorWindow->Selections.Selections.First(); !EditorWindow->Selections.Selections.End(); EditorWindow->Selections.Selections.Next())
	{
		WeActor* SelActor = EditorWindow->Selections.Selections.GetItem()->SelectedActor;
		char tmp[1000];
		sprintf(tmp, "%s [%.3f %.3f %.3f]",
			SelActor->Model->Name.GetString(),
			//EditorWindow->Selections.Selections.GetItem()->Model->GetCurrentMaterialName(EditorWindow->Selections.Selections.GetItem()->SelectedActor).GetString(),
			SelActor->Location.x, SelActor->Location.y, SelActor->Location.z);
		WeCanvas::Instance()->Draw2DTextOutlined(FontSmall, tmp, WeRenderDevice::Instance()->GetResWidth() - 300, 20 + 17 * i, 300, 17, WeColor(0xFFFFFF00), WeColor(0xFF000000));
		i++;
	}

	i = 1;
	if (!EditorWindow->HiddenObjects.Empty())
	{
		if (EditorWindow->ShowInfo)
			WeCanvas::Instance()->Draw2DTextOutlined(Font,"Hidden Objects",WeRenderDevice::Instance()->GetResWidth()/2+250,i+100,400,20,WeColor(0xFFFFFF00), WeColor(0xFF000000));
		for(EditorWindow->HiddenObjects.First(); !EditorWindow->HiddenObjects.End(); EditorWindow->HiddenObjects.Next())
		{
			if (EditorWindow->ShowInfo)
				WeCanvas::Instance()->Draw2DTextOutlined(Font,EditorWindow->HiddenObjects.GetItem(),WeRenderDevice::Instance()->GetResWidth()/2+250,22*i+100,400,20,WeColor(0xFFFFFF00), WeColor(0xFF000000));
			i++;
		}
	}		
}




void TextureEditorRendering()
{	
	WeTexture *BackTexture = WeTextureManager::Instance()->Load("MapEditor.Textures.Back", true);
	if (BackTexture)
		WeCanvas::Instance()->DrawRectangle(0,0,WeRenderDevice::Instance()->GetResWidth(),WeRenderDevice::Instance()->GetResHeight(),D3DCOLOR_RGBA(255,255,255,255),BackTexture);

	if(TextureEditor->Texture)
		WeCanvas::Instance()->DrawRectangle(0,0,WeRenderDevice::Instance()->GetResWidth(),WeRenderDevice::Instance()->GetResHeight(),D3DCOLOR_RGBA(255,255,255,255),TextureEditor->Texture);
}




void MaterialEditorRendering()
{
	static WeTexture *BackTexture = NULL;
	if (!BackTexture)
		BackTexture = WeTextureManager::Instance()->Load("Textures.GeneralPurpose.CrackedConcrete", true);

	WePostProcess::Instance()->BeginScreenCapture();
	if (BackTexture)
		WeCanvas::Instance()->DrawRectangle(0,0,WeRenderDevice::Instance()->GetResWidth(),WeRenderDevice::Instance()->GetResHeight(),D3DCOLOR_RGBA(255,255,255,255),BackTexture);

	MaterialEditor->MEWorld->Render(MaterialEditor->MECamera);

	WePostProcess::Instance()->EndScreenCapture();
	WePostProcess::Instance()->PostRender();
}



void MaterialEditorTextureRendering()
{
	static WeTexture *BackTexture = NULL;
	if (!BackTexture)
		BackTexture = WeTextureManager::Instance()->Load("MapEditor.Textures.Back", true);
	if (BackTexture)
		WeCanvas::Instance()->DrawRectangle(0,0,WeRenderDevice::Instance()->GetResWidth(),WeRenderDevice::Instance()->GetResHeight(),D3DCOLOR_RGBA(255,255,255,255),BackTexture);

	CTreeViewItem* TexItem = MaterialEditor->TextureList->GetSelected();
	if (TexItem && TexItem->GetLevel() == 2)
	{
		WeTexture *Texture = WeTextureManager::Instance()->Load(GetFullItemName(TexItem), true);
		if (Texture)
			WeCanvas::Instance()->DrawRectangle(0,0,WeRenderDevice::Instance()->GetResWidth(),WeRenderDevice::Instance()->GetResHeight(),D3DCOLOR_RGBA(255,255,255,255),Texture);
	}
}



void MeshEditorRendering()
{
	MeshEditor->World->Render(MeshEditor->Camera);
}



void MeshEditorTextureRendering()
{
	WeTexture *BackTexture = WeTextureManager::Instance()->Load("MapEditor.Textures.Back", true);
	if (BackTexture)
		WeCanvas::Instance()->DrawRectangle(0, 0, WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DCOLOR_RGBA(255,255,255,255), BackTexture);

	if (MeshEditor->PreviewTexture)
		WeCanvas::Instance()->DrawRectangle(0, 0, WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DCOLOR_RGBA(255,255,255,255), MeshEditor->PreviewTexture);
}



void ModelEditorRendering()
{
}


void PSEditorRendering()
{
	if(ParticleSystemEditor->GetVisible())
		ParticleSystemEditor->PSEditorWorld->Render(ParticleSystemEditor->PSEditorCamera);
}

void InsertObjectRendering()
{
	bool l;
	l = WeRenderDevice::Instance()->GetLight();
	WeRenderDevice::Instance()->SetLight(false);

	if(CDialogInsertObject::Instance()->GetVisible() && CDialogInsertObject::Instance()->PreviewActor != NULL)
		CDialogInsertObject::Instance()->PreviewWorld->Render(CDialogInsertObject::Instance()->PreviewCamera);

	WeRenderDevice::Instance()->SetLight(l);
}


void AnimatedMeshEditorRendering()
{
	CDialogAnimatedMesh::Instance()->PreviewWorld->Render(CDialogAnimatedMesh::Instance()->PreviewCamera);
	if (CDialogAnimatedMesh::Instance()->Model)
	{
		char temp[100];
		sprintf(temp, "%.3f", CDialogAnimatedMesh::Instance()->Model->AnimController->GetTime());
		CDialogAnimatedMesh::Instance()->EditTime->SetText((String)temp);
	}
}



PHASES CMainPhase::Start()
{	
/*
	ModelEditorWorld = new WeWorld();
*/

	EditorWindow->Initialize();

	WeMatrix tmpTM;
	D3DXMatrixTranslation(&tmpTM, 0.0, 0.0, 20.0);

/*
	ModelActor = new WeActor();
	ModelActor->Model = new WeStaticModel();
	ModelActor->Renderable = false;
	ModelActor->TM = tmpTM;


	ModelEditorWorld->AddActor(ModelActor);
*/

	WeCamera* Cam = new WeCamera();
	Cam->Location = WeVector(0,0,-50.0);
	Cam->Direction = WeVector(0,0,1.0);
	Cam->Up = WeVector(0,1.0,0);
	

	WeRenderDevice::Instance()->SetClearColor(BACK_COLOR);
	//WePostProcess::Instance()->Initialize();
	//WePostProcess::Instance()->TurnOff();

	MaterialEditor->InitializeWorldAndObjects();


	while(EditorWindow->Running())
	{
		Sleep(0);

		EditorWindow->Update();

/*
		ModelEditorWorld->Tick();
*/
		Cam->Update();


		//WeEngine::Instance()->Update(Cam);


		if (TextureEditor->GetVisible())
		{
			WeRenderDevice::Instance()->SetHWND(TextureEditor->RenderBox->GetHWND());
			WeRenderDevice::Instance()->SetRenderCallback(TextureEditorRendering);
			WeEngine::Instance()->Update(Cam);

			continue;
		}

		if (MaterialEditor->GetVisible())
		{
			MaterialEditor->MEWorld->Tick();
			MaterialEditor->MECamera->Update();
			MaterialEditor->UpdateRotations();

			WeRenderDevice::Instance()->SetHWND(MaterialEditor->PreviewArea->GetHWND());
			WeRenderDevice::Instance()->SetRenderCallback(MaterialEditorRendering);
			WeEngine::Instance()->Update(MaterialEditor->MECamera);

			if (MaterialEditor->TexPreview->GetChecked())
			{
				WeRenderDevice::Instance()->SetHWND(MaterialEditor->TexturePreviewArea->GetHWND());
				WeRenderDevice::Instance()->SetRenderCallback(MaterialEditorTextureRendering);
				WeEngine::Instance()->Update(Cam);
			}

			continue;
		}

		if (MeshEditor->GetVisible())
		{
			MeshEditor->Tick();
			MeshEditor->World->Tick();			
			MeshEditor->Camera->Update();

			bool Wire = WeRenderDevice::Instance()->GetWireFrame();
			
			if(MeshEditor->WireFrame && !Wire)
			{
				WeRenderDevice::Instance()->SetWireFrame(true);
			}
			if(!MeshEditor->WireFrame && Wire)
			{
				WeRenderDevice::Instance()->SetWireFrame(false);
			}

			WeRenderDevice::Instance()->SetHWND(MeshEditor->MeshPreview->GetHWND());
			WeRenderDevice::Instance()->SetRenderCallback(MeshEditorRendering);
			WeEngine::Instance()->Update(MeshEditor->Camera);

			WeRenderDevice::Instance()->SetWireFrame(false);
			WeRenderDevice::Instance()->SetHWND(MeshEditor->DiffusePreview->GetHWND());
			WeRenderDevice::Instance()->SetRenderCallback(MeshEditorTextureRendering);
			WeEngine::Instance()->Update(Cam);

			continue;
		}

		
		if (CDialogAnimTextureEditor::Instance()->GetVisible())
		{
			CDialogAnimTextureEditor::Instance()->Tick();

			WeRenderDevice::Instance()->SetHWND(CDialogAnimTextureEditor::Instance()->PreviewButton->GetHWND());
			WeRenderDevice::Instance()->SetRenderCallback(CDialogAnimTextureEditor::Instance()->DoRender);
			WeEngine::Instance()->Update(Cam);

			continue;
		}


/*
		if (ModelEditor->GetVisible())
		{
			if(ModelEditor->Mesh->GetMesh() && ModelActor->Model->Mesh != ModelEditor->Mesh)
			{
				ModelActor->Model->Mesh = ModelEditor->Mesh;
				ModelActor->Renderable = true;
			}else if(!ModelEditor->Mesh->GetMesh()){
				ModelActor->Renderable = false;
			}

			bool Wire = WeRenderDevice::Instance()->GetWireFrame();
			
			if(ModelEditor->WireFrame && !Wire)
			{
				WeRenderDevice::Instance()->SetWireFrame(true);
				WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
			if(!ModelEditor->WireFrame && Wire)
			{
				WeRenderDevice::Instance()->SetWireFrame(false);
				WeRenderDevice::Instance()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}

			WeRenderDevice::Instance()->SetHWND(ModelEditor->PreviewGroup->GetHWND());
			WeRenderDevice::Instance()->SetRenderCallback(ModelEditorRendering);
			WeEngine::Instance()->Update(Cam);

			if(ModelEditor->WireFrame != Wire)
				WeRenderDevice::Instance()->SetWireFrame(Wire);

			continue;
		}
*/


		if (ParticleSystemEditor->GetVisible())
		{
			ParticleSystemEditor->PSEditorWorld->Tick();
			ParticleSystemEditor->PSEditorCamera->Update();

			WeRenderDevice::Instance()->SetHWND(ParticleSystemEditor->Preview->GetHWND());
			WeRenderDevice::Instance()->SetRenderCallback(PSEditorRendering);
			WeEngine::Instance()->Update(ParticleSystemEditor->PSEditorCamera);

			continue;			
		}

		//static bool b_Render = false;
		if (CDialogInsertObject::Instance()->GetVisible())
		{
			CDialogInsertObject::Instance()->UpdateActor();
			CDialogInsertObject::Instance()->PreviewWorld->Tick();
			CDialogInsertObject::Instance()->PreviewCamera->Update();

			WeRenderDevice::Instance()->SetHWND(CDialogInsertObject::Instance()->PreviewArea->GetHWND());
			WeRenderDevice::Instance()->SetRenderCallback(InsertObjectRendering);
			WeEngine::Instance()->Update(CDialogInsertObject::Instance()->PreviewCamera);

			continue;
		}

		if (CDialogAnimatedMesh::Instance()->GetVisible())
		{
			//CDialogAnimatedMesh::Instance()->UpdateActor();
			CDialogAnimatedMesh::Instance()->PreviewWorld->Tick();
			CDialogAnimatedMesh::Instance()->PreviewCamera->Update();

			WeRenderDevice::Instance()->SetHWND(CDialogAnimatedMesh::Instance()->PreviewArea->GetHWND());
			WeRenderDevice::Instance()->SetRenderCallback(AnimatedMeshEditorRendering);
			WeEngine::Instance()->Update(CDialogAnimatedMesh::Instance()->PreviewCamera);

			continue;
		}


/*
		UpdateRotations();
*/

		EditorWindow->MapEditorWorld->Tick();
		EditorWindow->Cam->Update();
		EditorWindow->Cam->UpdateProjection = true;

		WeRenderDevice::Instance()->SetHWND(EditorWindow->GetHWND());
		WeRenderDevice::Instance()->SetRenderCallback(MapEditorRendering);						
		WeEngine::Instance()->Update(EditorWindow->Cam);
	}

	return PHASE_DEINITIALIZE;
}
