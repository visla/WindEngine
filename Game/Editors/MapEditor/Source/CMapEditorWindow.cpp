/*
      File: CMapEditorWindow.cpp
	  Author: Tomislav Kukic
	  Date: 04.11.2005.
	  Version: 1.0
*/


#include <CMapEditorWindow.h>

#include <WeRenderDevice.h>
#include <CEditorResourceManager.h>
#include <CString.h>
#include <direct.h>
#include <Commdlg.h>
#include <CEditorMap.h>
#include <CStartLocation.h>
#include <smartlist.h>
#include <CMap.h>
#include <CSelection.h>
#include <WePackage.h>
#include <WeCamera.h>
#include <WeActor.h>
#include <WePathNode.h>
#include <WeParticleSystem.h>
#include <WeStaticModel.h>
#include <WePhysics.h>
#include <WeMaterial.h>
#include <WePhysicHandle.h>
#include <ActorTags.h>
#include <CTubeSegment.h>
#include <WeModelManager.h>
#include <WeTextureManager.h>
#include <CMapEditorCamera.h>
#include <CUndo.h>
#include <WeMaterialManager.h>
#include <WeTimer.h>
#include <CPickUp.h>
#include <CActorTypesSupplier.h>
#include <WePhysics.h>
#include <WeModelManager.h>
#include <WeCloth.h>
#include <WeZone.h>
#include <WePortal.h>
#include <CDialogAnimTextureEditor.h>
#include <WeShaderManager.h>
#include <WeFlareRenderer.h>
#include <CDialogHelp.h>
#include <CDialogAbout.h>
#include <CDialogBaking.h>
#include <CDialogInsert.h>
#include <CDialogPostProcess.h>
#include <CDialogTextureEditor.h>
#include <CDialogMaterialEditor.h>
#include <CDialogMeshEditor.h>
#include <CDialogModelEditor.h>
#include <CDialogTubersEditor.h>
#include <CDialogEffectEditor.h>
#include <CDialogParticleSystemEditor.h>
#include <CDialogSoundEditor.h>
#include <CDialogPhysicModelEditor.h>
#include <CDialogAnimatedMesh.h>
//#include <CDialogMaterialPresets.h>
#include <CDialogActionManagment.h>
#include <CDialogSkyBox.h>
#include <CDialogMapDescription.h>
#include <CDialogAIProfiles.h>
#include <CDialogTournamentEditor.h>
#include <CDialogActorTypes.h>
#include <CDialogInsertObject.h>
#include <CDialogPickupList.h>
#include <CDialogTubeConnect.h>
#include <CDialogAlign.h>
#include <CDialogInput.h>
#include <CDialogTools.h>
#include <CDialogSelection.h>
#include <CDialogRenderPhases.h>
#include <CConfig.h>
#include <WePostProcess.h>



#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A // (not always defined)
#endif
#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120 // (not always defined)
#endif


void SetMenuItemChecked(int ID, bool Checked)
{
	CheckMenuItem(GetMenu(EditorWindow->GetHWND()), ID, (Checked ? MF_CHECKED : MF_UNCHECKED));
}


bool GetMenuItemChecked(int ID)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STATE;
	GetMenuItemInfo(GetMenu(EditorWindow->GetHWND()), ID, false, &mii);
	return ((mii.fState | MFS_CHECKED) != 0);
}


CMapEditorWindow *EditorWindow = NULL;


CMapEditorWindow::CMapEditorWindow()
{
	BakingLights = false;
	MouseLeftDown = false;
	MouseRightDown = false;	
	MouseDeltaRoll = 0;
	OldMouseDeltaRoll = 0;
	MouseDeltaX = 0;
	MouseDeltaY = 0;
	LastMouseDeltaX = 0;
	LastMouseDeltaY = 0;
	SaveTimer = 0.0f;
	Moving = false;
	Shift = false;
	Ctrl  = false;
	Alt = false;
	Rotating = false;
	LockedX = false;
	LockedY = false;
	LocalMove = false;
	LocalRotate = false;
	Selecting = false;
	Orbiting = false;
	OrbitTarget = NULL;

	MoveUp = false;
	MoveDown = false;
	MoveLeft = false;
	MoveRight = false;
	SaveTemp = false;
	ShowPathNode = true;

	MapFile = (String)"";

	ReInsertHoleName = (String)"";

	for (int i = 0; i < 10; ++i)
	{
		Bookmark[i] = WeVector(0.0f, 0.0f, 0.0f);
	}

	ShowInfo = true;
}




CMapEditorWindow::~CMapEditorWindow()
{
	//ShutDown();
}



void CMapEditorWindow::Initialize()
{
	MapEditorWorld = new CEditorMap;

	Cam = new CMapEditorCamera();
	Cam->Location = WeVector(0,0,-50.0);
	Cam->Direction = WeVector(0,0,1.0);
	Cam->Up = WeVector(0,1.0,0);


	/*Grid = new CEditorGrid();
	Grid->Initialize(MapEditorWorld);			*/

	CTubeSegment::Instance()->LoadFromTxt("segments.txt");
	CUndo::Instance()->Reset();
	//CActionManager::Instance()->LoadActions();

	WeRenderDevice::Instance()->SetLight(false);

	WePathNode::ResetSequenceNumbers();

	TextureEditor->OnRefreshPackage("");

	LoadStates();
	WePostProcess::Instance()->UpdateEffects();
}


void CMapEditorWindow::Terminate()
{
	static bool Terminated = false;
	if (Terminated) return;
	Terminated = true;

	SaveStates();

	CEditorWindow::Terminate();
}


bool CMapEditorWindow::Create()
{
	if (!CEditorWindow::Create("MAP_EDITOR", "Bugditor 1.5", IDM_MAINMENU, IDI_ICON1, IDI_ICON1, false, false, true, false, true, true))
	  return false;

	
	MouseX = 0.0;
	MouseY = 0.0;

	Yaw = 0.0;
	Pitch = 0.0;

	OldMouseX = 0.0;
	OldMouseY = 0.0;

	Speed = 10;
	OldMouseY = 0.0;
	
	MouseRotation = false;
	MouseMoveFB = false;
	MouseMoveUD = false;

	MapEditorWorld = NULL;
	Cam = NULL;

	return true;
}


void CMapEditorWindow::LoadStates()
{
	WeRenderDevice::Instance()->SetSpecular(GetConfigInt("MapEditor.View.Specular", 0) != 0);
	WeRenderDevice::Instance()->SetBump(GetConfigInt("MapEditor.View.Bump", 0) != 0);
	WeRenderDevice::Instance()->SetRenderParticles(GetConfigInt("MapEditor.View.Particles", 1) != 0);
	WeRenderDevice::Instance()->SetShadows(GetConfigInt("MapEditor.View.Shadows", 0) != 0);
	WeRenderDevice::Instance()->SetRefractive(GetConfigInt("MapEditor.View.Refraction", 0) != 0);
	WeRenderDevice::Instance()->SetGlow(GetConfigInt("MapEditor.View.Glow", 0) != 0);
	WeRenderDevice::Instance()->SetMotionBlur(GetConfigInt("MapEditor.View.MotionBlur", 0) != 0);
	WeRenderDevice::Instance()->SetPixelMotionBlur(GetConfigInt("MapEditor.View.PixelMotionBlur", 0) != 0);
	WeRenderDevice::Instance()->SetPostProcessEffects(GetConfigInt("MapEditor.View.PostProcessEffects", 0) != 0);

	SetMenuItemChecked(ID_VIEW_SPECULAR, WeRenderDevice::Instance()->GetSpecular());
	SetMenuItemChecked(ID_VIEW_BUMPING, WeRenderDevice::Instance()->GetBump());
	SetMenuItemChecked(ID_VIEW_SHOW, WeRenderDevice::Instance()->GetRenderParticles());
	SetMenuItemChecked(ID_VIEW_SHADOWS, WeRenderDevice::Instance()->GetShadows());
	SetMenuItemChecked(ID_VIEW_REFRACTION, WeRenderDevice::Instance()->GetRefractive());
	SetMenuItemChecked(ID_VIEW_GLOW, WeRenderDevice::Instance()->GetGlow());
	SetMenuItemChecked(ID_VIEW_MOTIONBLUR, WeRenderDevice::Instance()->GetMotionBlur());
	SetMenuItemChecked(ID_VIEW_PIXELMOTIONBLUR, WeRenderDevice::Instance()->GetPixelMotionBlur());
	SetMenuItemChecked(ID_VIEW_ADDEDPOSTPROCESSES, WeRenderDevice::Instance()->GetPostProcessEffects());

	ShowInfo = GetConfigInt("MapEditor.View.ShowInfo", 1) != 0;
}


void CMapEditorWindow::SaveStates()
{
	char tmp[100];

	WeTextStream TS;
	TS.OpenNew("MapEditor.ini");

	TS.WriteString("[Camera]\n");
	sprintf_s(tmp, "FarClip=%.3f\n", Cam->GetFarClip());
	TS.WriteString(tmp);
	TS.WriteString("\n");

	TS.WriteString("[View]\n");
	sprintf_s(tmp, "Specular=%d\n", (int)WeRenderDevice::Instance()->GetSpecular());
	TS.WriteString(tmp);
	sprintf_s(tmp, "Bump=%d\n", (int)WeRenderDevice::Instance()->GetBump());
	TS.WriteString(tmp);
	sprintf_s(tmp, "Particles=%d\n", (int)WeRenderDevice::Instance()->GetRenderParticles());
	TS.WriteString(tmp);
	sprintf_s(tmp, "Shadows=%d\n", (int)WeRenderDevice::Instance()->GetShadows());
	TS.WriteString(tmp);
	sprintf_s(tmp, "Refractions=%d\n", (int)WeRenderDevice::Instance()->GetRefractive());
	TS.WriteString(tmp);
	sprintf_s(tmp, "Glow=%d\n", (int)WeRenderDevice::Instance()->GetGlow());
	TS.WriteString(tmp);
	sprintf_s(tmp, "MotionBlur=%d\n", (int)WeRenderDevice::Instance()->GetMotionBlur());
	TS.WriteString(tmp);
	sprintf_s(tmp, "PixelMotionBlur=%d\n", (int)WeRenderDevice::Instance()->GetPixelMotionBlur());
	TS.WriteString(tmp);
	sprintf_s(tmp, "PostProcessEffects=%d\n", (int)WeRenderDevice::Instance()->GetPostProcessEffects());
	TS.WriteString(tmp);
	sprintf_s(tmp, "ShowInfo=%d\n", (int)ShowInfo);
	TS.WriteString(tmp);

	TS.Close();
}


bool FirstTime = true;
void CMapEditorWindow::OnResize(int neww, int newh, DWORD ResizeType)
{
	if(FirstTime) { FirstTime = false; return; }
	
	if(Cam) Cam->SetAspect((float)GetWidth() / (float)GetHeight());
}


void CMapEditorWindow::FixStartLocations()
{
	MapEditorWorld->MapHeader.StartLocations.Clear();

	for (WeActor* Iterator = MapEditorWorld->Actors->NextActor; Iterator != MapEditorWorld->Actors; Iterator = Iterator->NextActor)
	{
		if (Iterator->Type == ACTORTYPE_STARTINGLOCATION)
			MapEditorWorld->MapHeader.StartLocations.PushBack((CStartLocation*)Iterator);
	}
}


void CMapEditorWindow::OnClick(DWORD ID)
{
	static bool ElementsVisible = false;

	switch(ID)
	{
	case IDM_FILE_EXIT:
		Terminate();
		break;

	case ID_MAPEDIT_FILE_NEW:
		if(IDNO == MessageBox(GetHWND(),"Create new map?","New",MB_YESNO | MB_ICONQUESTION)) return;
		NewMap();
		break;

	case ID_FILE_SAVEMAPAS:
		SaveMap();
		break;

	case ID_FILE_LOADMAP:
		LoadMap();
		break;

	case ID_FILE_EXPORTTOOBJ:
		ExportToOBJ();
		break;

	case ID_HELP_HELP:
		DialogHelp->Show();
		break;

	case ID_HELP_ABOUT:
		DialogAbout->ShowModal(GetHWND());
		break;

	case ID_VIEW_POSTPROCESES:
		CDialogPostProceses::Instance()->Show();
		break;

	case ID_EDITORS_ANIMTEXTUREEDITOR:
		CDialogAnimTextureEditor::Instance()->Show();
		break;

	case ID_EDITORS_TEXTUREEDITOR:
		TextureEditor->Show();
		break;

	case ID_EDITORS_MATERIALEDITOR:
		MaterialEditor->Show();
		break;

	case ID_EDITORS_MESHEDITOR:
		MeshEditor->Show();
		break;

	case ID_EDITORS_MODELEDITOR:
		ModelEditor->Show();
		break;

	case ID_EDITORS_TUBERSEDITOR:
		CDialogTubersEditor::Instance()->Show();		
		break;

	case ID_OPTIONS_CLEARALLLIGHTSONMAP:
		if (IDYES == MessageBox(GetHWND(), "Are you sure?", "Clear All Lights", MB_YESNO | MB_ICONQUESTION))
			MapEditorWorld->ClearAllLights();
		break;

	case ID_VIEW_SHADOWS:
		WeRenderDevice::Instance()->SetShadows(!WeRenderDevice::Instance()->GetShadows());
		SetMenuItemChecked(ID_VIEW_SHADOWS, WeRenderDevice::Instance()->GetShadows());
		break;

	case ID_VIEW_BUMPING:
		WeRenderDevice::Instance()->SetBump(!WeRenderDevice::Instance()->GetBump());
		SetMenuItemChecked(ID_VIEW_BUMPING, WeRenderDevice::Instance()->GetBump());
		break;

	case ID_VIEW_SPECULAR:
		WeRenderDevice::Instance()->SetSpecular(!WeRenderDevice::Instance()->GetSpecular());
		SetMenuItemChecked(ID_VIEW_SPECULAR, WeRenderDevice::Instance()->GetSpecular());
		break;

	case ID_VIEW_SHOWHIDE:
		if (ElementsVisible)
			MapEditorWorld->Editor_HideDoodads(); 
		else
			MapEditorWorld->Editor_ShowDoodads(); 
		ElementsVisible = !ElementsVisible;
		break;

	case ID_VIEW_SHOWHIDEPATHNODES:
		OnShowHidePathNodes();
		break;

	case ID_VIEW_GLOW:
		WeRenderDevice::Instance()->SetGlow(!WeRenderDevice::Instance()->GetGlow());
		SetMenuItemChecked(ID_VIEW_GLOW, WeRenderDevice::Instance()->GetGlow());
		WePostProcess::Instance()->UpdateEffects();
		break;

	case ID_VIEW_REFRACTION:
		WeRenderDevice::Instance()->SetRefractive(!WeRenderDevice::Instance()->GetRefractive());
		SetMenuItemChecked(ID_VIEW_REFRACTION, WeRenderDevice::Instance()->GetRefractive());
		WePostProcess::Instance()->UpdateEffects();
		break;

	case ID_VIEW_MOTIONBLUR:
		WeRenderDevice::Instance()->SetMotionBlur(!WeRenderDevice::Instance()->GetMotionBlur());
		SetMenuItemChecked(ID_VIEW_MOTIONBLUR, WeRenderDevice::Instance()->GetMotionBlur());
		WePostProcess::Instance()->UpdateEffects();
		break;

	case ID_VIEW_PIXELMOTIONBLUR:
		WeRenderDevice::Instance()->SetPixelMotionBlur(!WeRenderDevice::Instance()->GetPixelMotionBlur());
		SetMenuItemChecked(ID_VIEW_PIXELMOTIONBLUR, WeRenderDevice::Instance()->GetPixelMotionBlur());
		WePostProcess::Instance()->UpdateEffects();
		break;

	case ID_VIEW_ADDEDPOSTPROCESSES:
		WeRenderDevice::Instance()->SetPostProcessEffects(!WeRenderDevice::Instance()->GetPostProcessEffects());
		SetMenuItemChecked(ID_VIEW_ADDEDPOSTPROCESSES, WeRenderDevice::Instance()->GetPostProcessEffects());
		WePostProcess::Instance()->UpdateEffects();
		break;

	case ID_ANISOTROPIC_OFF:
		WeRenderDevice::Instance()->SetAnisotropic(false);		
		break;
	case ID_ANISOTROPIC_2X:
		WeRenderDevice::Instance()->SetAnisotropic(true);
		WeRenderDevice::Instance()->SetAnisotropicFactor(2);		
		break;
	case ID_ANISOTROPIC_4X:
		WeRenderDevice::Instance()->SetAnisotropic(true);
		WeRenderDevice::Instance()->SetAnisotropicFactor(4);		
		break;
	case ID_ANISOTROPIC_8:
		WeRenderDevice::Instance()->SetAnisotropic(true);
		WeRenderDevice::Instance()->SetAnisotropicFactor(8);		
		break;
	case ID_ANISOTROPIC_16X:
		WeRenderDevice::Instance()->SetAnisotropic(true);
		WeRenderDevice::Instance()->SetAnisotropicFactor(16);		
		break;

	case ID_ANTIALIASING_OFF:
		WeRenderDevice::Instance()->SetAntialiasFactor(0);
		break;
	case ID_ANTIALIASING_2X:
		WeRenderDevice::Instance()->SetAntialiasFactor(2);
		break;
	case ID_ANTIALIASING_4X:
		WeRenderDevice::Instance()->SetAntialiasFactor(4);
		break;
	case ID_ANTIALIASING_6X:
		WeRenderDevice::Instance()->SetAntialiasFactor(6);
		break;
	case ID_ANTIALIASING_8X:
		WeRenderDevice::Instance()->SetAntialiasFactor(8);
		break;
	case ID_ANTIALIASING_16X:
		WeRenderDevice::Instance()->SetAntialiasFactor(16);
		break;

	case ID_RENDERPHASES:
		DialogRenderPhases->Show();
		break;

	case ID_EDITORS_EFFECTEDITOR:
		EffectEditor->Show();
		break;

	case ID_EDITORS_PARTICLESYSTEMEDITOR:
		ParticleSystemEditor->Show();
		break;

	case ID_EDITORS_SOUNDEDITOR:
		DialogSoundEditor->Show();
		break;

	case ID_EDITORS_PHYSICMODELEDITOR:
		CDialogPhysicModelEditor::Instance()->Show();
		break;

	case ID_EDITORS_ANIMMESHEDITOR:
		CDialogAnimatedMesh::Instance()->Show();
		break;

	case ID_INSERTMENU_OBJECT:
		InsertObject();
		break;

		// Views
	case ID_CAMERA_CAMERALOOK:
		Cam->SetViewMode(CAMERA_FREE);
		break;

	case ID_CAMERA_FRONTVIEW:
		Cam->SetViewMode(CAMERA_FRONT);
		break;

	case ID_CAMERA_TOPVIEW:
		Cam->SetViewMode(CAMERA_TOP);
		break;

	case ID_CAMERA_LEFT:
		Cam->SetViewMode(CAMERA_LEFT);
		break;

	case ID_OPTIONS_ALIGN:
		DoAlign();
		break;

	case ID_EDIT_UNDO2:
		CUndo::Instance()->Undo();
		break;

	case ID_EDIT_ARRAY:
		DoArrayClone();
		break;

	case ID_EDIT_RADIALARRAY:
		DoRadialArrayClone();
		break;

	case ID_EDIT_FIXSTARTLOCATIONS:
		FixStartLocations();
		break;

	case ID_EDIT_FIXMODELS:
		for (WeActor *Iterator = MapEditorWorld->Actors->NextActor; Iterator != MapEditorWorld->Actors; Iterator = Iterator->NextActor)
		{
			switch (Iterator->Type)
			{
			case WETYPE_PATHNODE:
				Iterator->SpecialModelName = "MapEditor.Geometry.PathNode";
				Iterator->Initialize();
				break;

			case WETYPE_ZONE:
				Iterator->SpecialModelName = "MapEditor.Geometry.Box";
				Iterator->Initialize();
				break;

			case WETYPE_PORTAL:
				Iterator->SpecialModelName = "MapEditor.Geometry.Portal";
				Iterator->Initialize();
				break;

			case WETYPE_PARTICLESYSTEM:
				Iterator->SpecialModelName = "MapEditor.Geometry.ParticleSys";
				Iterator->Initialize();
				break;

			case WETYPE_LIGHT:
				Iterator->SpecialModelName = "MapEditor.Geometry.Light";
				Iterator->Initialize();
				break;

			case ACTORTYPE_STARTINGLOCATION:
				Iterator->SpecialModelName = "MapEditor.Geometry.StartLocati";
				Iterator->Initialize();
				break;
			}
		}
		break;

	case ID_EDIT_FIXPATHNODES:
		MapEditorWorld->DoSortPathNodes();

		for (WePathNode* Iterator = MapEditorWorld->PathNodes->NextPathNode; Iterator != MapEditorWorld->PathNodes; Iterator = Iterator->NextPathNode)
		{
			WeVector Lx, Ly, Lz;

			if (Iterator->IsValid && (Iterator->NodeType != NODETYPE_SPLIT) && (Iterator->NodeType != NODETYPE_TELEPORTING))
			{
				Lz = Iterator->NextNode->Location - Iterator->Location;
				D3DXVec3Normalize(&Lz, &Lz);

				Ly = WeVector(0.0f, 1.0f, 0.0f);

				D3DXVec3Cross(&Lx, &Ly, &Lz);
				D3DXVec3Normalize(&Lx, &Lx);

				D3DXVec3Cross(&Ly, &Lz, &Lx);

				static WeVector V0 = WeVector(0.0f, 0.0f, 0.0f);
				WeMatrix M, TM;

				D3DXMatrixLookAtLH(&M, &V0, &Lz, &Ly);
				D3DXMatrixInverse(&TM, NULL, &M);
 
				//TM = M;
				TM._41 = Iterator->Location.x;
				TM._42 = Iterator->Location.y;
				TM._43 = Iterator->Location.z;

				Iterator->SetTransformation(TM);
			}
		}

		break;

/*
	case IDC_EDIT_REMOVE_PATHNODES_BELOW_380:
		WePathNode* TempPathNode, *Iterator;
		for (Iterator = MapEditorWorld->PathNodes->NextPathNode; Iterator != MapEditorWorld->PathNodes; )
		{
			if (Iterator->SequenceNumber < 380)
			{
				TempPathNode = Iterator;
				Iterator = Iterator->NextPathNode;
				delete TempPathNode;
			}
			else
			{
				Iterator = Iterator->NextPathNode;
			}
		}
		break;
*/

	case ID_VIEW_GRID:
		Grid->Renderable = !Grid->Renderable;
		break;

	case ID_VIEW_ZONESANDPORTALS:
		OnShowHideZonesAndPortals();
		break;

	case ID_EDIT_FLIP:
		Selections.RotateCW();
		break;

	case ID_OPTIONS_MATERIALPRESETDEFAULT:
		//CDialogMaterialPresets::Instance()->ShowModal(GetHWND());
		break;

	case ID_OPTIONS_CALCULATESTATICLIGHTS:
		break;

	case ID_OPTIONS_CALCULATESIMPLELIGHTS:
		CalculateStaticLights(true);
		break;

	case ID_OPTIONS_CALCULATESIMPLESELECTEDLIGHT:
		CalculateSelectedLights(true);
		break;

	case ID_VIEW_SHOW:
		WeRenderDevice::Instance()->SetRenderParticles(!WeRenderDevice::Instance()->GetRenderParticles());
		SetMenuItemChecked(ID_VIEW_SHOW, WeRenderDevice::Instance()->GetRenderParticles());
		break;

	case ID_VIEW_ACTIONMANAGMENT:
		CDialogActionManagment::Instance()->SetVisible(!CDialogActionManagment::Instance()->GetVisible());
		break;

	case ID_DISPLAYQUALITY_LOW:
		WeShaderManager::Instance()->SetQuality(SHADER_LOW);
		break;

	case ID_DISPLAYQUALITY_MIDIUM:
		WeShaderManager::Instance()->SetQuality(SHADER_MEDIUM);
		break;

	case ID_DISPLAYQUALITY_HIGH:
		WeShaderManager::Instance()->SetQuality(SHADER_HIGH);
		break;

	case ID_INSERT_SETSKYBOX:
		CDialogSkyBox::Instance()->SetVisible(true);
		break;

	case ID_OPTIONS_MAPDESCRIPTION:
		CDialogMapDescription::Instance()->Show(MapEditorWorld);        
		break;

	case ID_OPTIONS_AILIST:
		CDialogAIProfiles::Instance()->Show();
		break;

	case ID_OPTIONS_TOURNAMENTEDITOR:
		CDialogTournamentEditor::Instance()->Show();
		//CDialogTournament::Instance()->Show();
		break;

	case ID_OPTIONS_COMPACTPACKAGE:
		if (GetFile("WindEngine Packages (*.wep)\0*.wep\0\0","*.wep"))
		{
			if (WePackage::Instance()->CompactPackage(FileName))
			{
				MessageBox(0, ((String)"Package " + FileName + (String)" successfully compacted!").GetString(), "Compact Package", 0);
			}
		}

		break;

	case ID_EDIT_SHOWALLHIDDEN:
		ShowAllHidden();
		break;

	case ID_EDIT_HIDEOBJECT:
		HideSelected();
		break;
	}
}



void CMapEditorWindow::OnShowHidePathNodes()
{
	if (ShowPathNode)
		MapEditorWorld->Editor_HidePathNodes();
	else
		MapEditorWorld->Editor_ShowPathNodes();

	ShowPathNode = !ShowPathNode;
}


void CMapEditorWindow::CalculateStaticLights(bool Simple)
{
	MapEditorWorld->OnBakingProgress = CDialogBaking::OnLoadProgress;
	MapEditorWorld->CalculateStaticLights(Simple);

	if (!ShowPathNode)
		MapEditorWorld->Editor_HidePathNodes();

	//MessageBox(0, "Finished Calculating", 0, MB_OK);
}


void CMapEditorWindow::CalculateSelectedLights(bool Simple)
{
	CSmartList <WeLight *> Lights;
	for (Selections.Selections.First(); !Selections.Selections.End(); Selections.Selections.Next())
	{
		if (Selections.Selections.GetItem()->SelectedActor->IsLight)
		{
			Lights.PushBack((WeLight *)Selections.Selections.GetItem()->SelectedActor);
		}
	}

	MapEditorWorld->OnBakingProgress = CDialogBaking::OnLoadProgress;
	MapEditorWorld->CalculateStaticLights(Lights, Simple);

	if (!ShowPathNode)
		MapEditorWorld->Editor_HidePathNodes();
}


void CMapEditorWindow::CalculateSelectedLightsActors(bool Simple)
{
	CSmartList <WeActor*> Actors;
	Actors.Clear();

	for (Selections.Selections.First(); !Selections.Selections.End(); Selections.Selections.Next())
	{
		if (Selections.Selections.GetItem()->SelectedActor->IsPrefab)
		{
			Actors.PushBack(Selections.Selections.GetItem()->SelectedActor);
		}
	}

	MapEditorWorld->OnBakingProgress = CDialogBaking::OnLoadProgress;			
	MapEditorWorld->CalculateStaticLights(Actors, Simple);

	if (!ShowPathNode)
		MapEditorWorld->Editor_HidePathNodes();
}


void CMapEditorWindow::OnBeforeClose(bool &AcceptClose)
{
	Terminate();
	AcceptClose = true;
}


void CMapEditorWindow::NewMap()
{
	ShutDown();
	Initialize();

	MapFile = (String)"";
}

void CMapEditorWindow::LoadMap()
{
	char OldDir[MAX_PATH];
	getcwd(OldDir, MAX_PATH);


	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	//HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = EditorWindow->GetHWND();
	ofn.lpstrFile = szFile;
	ofn.lpstrInitialDir = OldDir;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "UT map files *.utm\0*.utm\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrDefExt = "utm";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn))
	{
		NewMap();	
		String MapName = ofn.lpstrFile;	

		MapEditorWorld->OnLoadProgress = CDialogBaking::OnMapLoadProgress;
		while (!MapEditorWorld->IsLoaded())
			MapEditorWorld->LoadInTick(MapName, true);

		MapEditorWorld->MapHeader.SetupRenderDevice();

		MapFile = MapName;
		Cam->SetFarClip(MapEditorWorld->MapHeader.ClippingDistance);
	}

	chdir(OldDir);
}



void CalculatePathNodeHitDistance(WePathNode* PathNode, int HitIndex, WeVector &Dir)
{
	float distance = -1.0f;
	WeRaycastHit Hit;
	bool occluded;
	String HitName = "";

	WeVector Location = PathNode->Location;
	D3DXVec3Normalize(&Dir, &Dir);

	Location = Location + Dir * 0.0f;

	do {
		occluded = WePhysics::Instance()->RaycastClosest(Location, Dir, WE_SHAPE_TYPE_ALL, Hit, 7, 1000000.0f);

		if (occluded)
		{
			//if (Hit.ActorHit->PassFlares)
			if (Hit.ActorHit->Type == WETYPE_PATHNODE)
			{
				WeVector DeltaDir;
				D3DXVec3Scale(&DeltaDir, &Dir, 0.1f);
				D3DXVec3Add(&Location, &Location, &DeltaDir);
			}
			else
			{
				WeVector DeltaPos;
				D3DXVec3Subtract(&DeltaPos, &Hit.WorldImpact, &PathNode->Location);
				distance = D3DXVec3Length(&DeltaPos);
				HitName = Hit.ActorHit->ModelName;
				occluded = false;
			}
		}
	} while (occluded);

	PathNode->HitDistances[HitIndex] = distance;
	//WeLoger::DebugLog("Hit[%d:%d] = %.3f (%s)\n", PathNode->SequenceNumber, HitIndex, distance, HitName.GetString());
}



#define MAX_RADIUS 100.0f

void CMapEditorWindow::CheckPathNodes()
{
	WePathNode *Iterator;
	WeVector X, Y, Z;

	MapEditorWorld->DoSortPathNodes();

	//if (!MapEditorWorld->DoSortPathNodes())
		//MessageBox(0, "Some PathNodes were invalid and removed", "Save Map", 0);

	for(Iterator = MapEditorWorld->PathNodes->NextPathNode;
		Iterator != MapEditorWorld->PathNodes;
		Iterator = Iterator->NextPathNode)
	{
		Iterator->GetLocalAxis(X, Y, Z);

		CalculatePathNodeHitDistance(Iterator, 0, Y);
		CalculatePathNodeHitDistance(Iterator, 1, -Y);
		CalculatePathNodeHitDistance(Iterator, 2, -X);
		CalculatePathNodeHitDistance(Iterator, 3, X);

		Iterator->Radius = MAX_RADIUS;
		for (int i = 0; i < 4; ++i)
		{
			float d = Iterator->HitDistances[i];
			if ((d > 0.0f) && (d < Iterator->Radius))
				Iterator->Radius = d;
		}

		if (Iterator->Radius > MAX_RADIUS)
			Iterator->Radius = MAX_RADIUS;
		Iterator->Radius *= 0.99f;
	}
}



void CMapEditorWindow::SaveMap(bool FromShortCut)
{
	if (!FromShortCut || (MapFile == (String)""))
	{
		char OldDir[MAX_PATH];
		getcwd(OldDir, MAX_PATH);


		OPENFILENAME ofn;       // common dialog box structure
		char szFile[260];       // buffer for file name
		//HANDLE hf;              // file handle

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = EditorWindow->GetHWND();
		ofn.lpstrFile = szFile;
		ofn.lpstrInitialDir = OldDir;
		//
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		//
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "UT map files *.utm\0*.utm\0\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrDefExt = "utm";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

		// Display the Open dialog box. 

		if (GetSaveFileName(&ofn))
		{
			String MapName = ofn.lpstrFile;
			MapFile = MapName;

			DoSaveMap();
		}

		chdir(OldDir);
	}
	else
		DoSaveMap();
}


void CMapEditorWindow::DoSaveMap()
{
	CheckPathNodes();
	FixStartLocations();
	MapEditorWorld->Save(MapFile);
	MapEditorWorld->RenderTree->SaveToXMLFile(MapFile.Extract('.', 0) + ".xml");
	MessageBox(0, ((String)"Map " + MapFile + " saved.").GetString(), "MapEditor", 0);
}


void CMapEditorWindow::ExportToOBJ()
{
	char OldDir[MAX_PATH];
	getcwd(OldDir, MAX_PATH);


	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	//HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = EditorWindow->GetHWND();
	ofn.lpstrFile = szFile;
	ofn.lpstrInitialDir = OldDir;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Wavefront OBJ files (*.obj)\0*.obj\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrDefExt = "obj";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	// Display the Open dialog box. 

	if(GetSaveFileName(&ofn))
	{
		String ObjName = ofn.lpstrFile;
		MapEditorWorld->ExportToOBJ(ObjName);
	}

	chdir(OldDir);
}


void CMapEditorWindow::OnDblClk(int x, int y)
{
	// Convert screen client coords to back buff coords
	if (Cam->GetViewMode() == CAMERA_FREE)
	{
	  x = WeRenderDevice::Instance()->GetResWidth() * x / GetClientWidth();
	  y = WeRenderDevice::Instance()->GetResHeight() * y / GetClientHeight();
	}

	DoSelect(x, y, Shift);
}




void CMapEditorWindow::OnMouseWheel(WORD Position)
{
	if ((short)Position > 0)
		Cam->MoveCamera(MouseDeltaX, MouseDeltaY, 0, 0, false, false, 1);			
	if ((short)Position < 0)
		Cam->MoveCamera(MouseDeltaX, MouseDeltaY, 0, 0, false, false, -1);			
}



void CMapEditorWindow::OnLButtonDown(int x, int y)
{
	MouseLeftDown = true;
	MouseDeltaRoll = 0;

	if (!MouseRightDown)
	{
		MouseOriginX = x;
		MouseOriginY = y;
		MouseDeltaX = 0;
		MouseDeltaY = 0;
		LastMouseDeltaX = 0;
		LastMouseDeltaY = 0;
	}

	if (Moving)
	{
		Selections.OldTransformations.Clear();
		for (Selections.Selections.First(); !Selections.Selections.End(); Selections.Selections.Next())
		{
			WeMatrix M;
			Selections.Selections.GetItem()->SelectedActor->GetTransformation(M);
			Selections.OldTransformations.PushBack(M);
		}
	}
	else if (Alt)
		Selecting = true;
	
	ShowCursor(false);
	SetCapture(GetHWND());	
}



void CMapEditorWindow::GetBBoxViewProjRect(WeSphere &sphere, WeCamera *Cam, float &left, float &right, float &bottom, float &top)
{
	WeVector inV;
	D3DXVECTOR4 outV;

	inV.x = sphere.Position.x - sphere.Radius;
	inV.y = sphere.Position.y - sphere.Radius;
	inV.z = sphere.Position.z - sphere.Radius;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	left = outV.x;
	right = outV.x;
	bottom = outV.y;
	top = outV.y;

	inV.x = sphere.Position.x + sphere.Radius;
	inV.y = sphere.Position.y - sphere.Radius;
	inV.z = sphere.Position.z - sphere.Radius;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;

	inV.x = sphere.Position.x - sphere.Radius;
	inV.y = sphere.Position.y + sphere.Radius;
	inV.z = sphere.Position.z - sphere.Radius;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;

	inV.x = sphere.Position.x + sphere.Radius;
	inV.y = sphere.Position.y + sphere.Radius;
	inV.z = sphere.Position.z - sphere.Radius;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;

	inV.x = sphere.Position.x - sphere.Radius;
	inV.y = sphere.Position.y - sphere.Radius;
	inV.z = sphere.Position.z + sphere.Radius;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;

	inV.x = sphere.Position.x + sphere.Radius;
	inV.y = sphere.Position.y - sphere.Radius;
	inV.z = sphere.Position.z + sphere.Radius;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;

	inV.x = sphere.Position.x - sphere.Radius;
	inV.y = sphere.Position.y + sphere.Radius;
	inV.z = sphere.Position.z + sphere.Radius;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;

	inV.x = sphere.Position.x + sphere.Radius;
	inV.y = sphere.Position.y + sphere.Radius;
	inV.z = sphere.Position.z + sphere.Radius;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;
}



void CMapEditorWindow::GetBBoxViewProjRect(WeBox &box, WeCamera *Cam, float &left, float &right, float &bottom, float &top)
{
	WeVector inV;
	D3DXVECTOR4 outV;

	inV.x = box.Min.x;
	inV.y = box.Min.y;
	inV.z = box.Min.z;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	left = outV.x;
	right = outV.x;
	bottom = outV.y;
	top = outV.y;

	inV.x = box.Max.x;
	inV.y = box.Min.y;
	inV.z = box.Min.z;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;

	inV.x = box.Min.x;
	inV.y = box.Max.y;
	inV.z = box.Min.z;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;

	inV.x = box.Max.x;
	inV.y = box.Max.y;
	inV.z = box.Min.z;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;

	inV.x = box.Min.x;
	inV.y = box.Min.y;
	inV.z = box.Max.z;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;

	inV.x = box.Max.x;
	inV.y = box.Min.y;
	inV.z = box.Max.z;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;

	inV.x = box.Min.x;
	inV.y = box.Max.y;
	inV.z = box.Max.z;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;

	inV.x = box.Max.x;
	inV.y = box.Max.y;
	inV.z = box.Max.z;
	D3DXVec3Transform(&outV, &inV, &Cam->ViewProj);
	outV.x /= outV.w;
	outV.y /= outV.w;
	if (outV.x < left) left = outV.x;
	if (outV.x > right) right = outV.x;
	if (outV.y < bottom) bottom = outV.y;
	if (outV.y > top) top = outV.y;
}



void CMapEditorWindow::OnLButtonUp(int x, int y)
{
	if (Selecting)
	{
		bool Crossing = !Ctrl;
		int x, y, w, h;

		if (MouseDeltaX < 0)
			x = MouseOriginX + MouseDeltaX;
		else
			x = MouseOriginX;
		x = x * WeRenderDevice::Instance()->GetResWidth() / GetWidth();

		if (MouseDeltaY < 0)
			y = MouseOriginY + MouseDeltaY;
		else
			y = MouseOriginY;
		y = y * WeRenderDevice::Instance()->GetResHeight() / GetHeight();

		w = abs(MouseDeltaX);
		w = w * WeRenderDevice::Instance()->GetResWidth() / GetWidth();

		h = abs(MouseDeltaY);
		h = h * WeRenderDevice::Instance()->GetResHeight() / GetHeight();

		CSmartList<WeActor*> NewSelection;

		WeActor *Iterator;
		for (Iterator = MapEditorWorld->Actors->NextActor; Iterator != MapEditorWorld->Actors; Iterator = Iterator->NextActor)
		{
			D3DXVECTOR4 v;
			D3DXVec3Transform(&v, &Iterator->Location, &Cam->ViewProj);

			if ((v.z >= 0) && (v.z < Cam->GetFarClip()))
			{
				float left, right, bottom, top;
				//GetBBoxViewProjRect(Iterator->BSphere, Cam, left, right, bottom, top);
				GetBBoxViewProjRect(Iterator->BBox, Cam, left, right, bottom, top);

				int x1 = (int)((left + 1.0f) / 2.0f * WeRenderDevice::Instance()->GetResWidth());
				int x2 = (int)((right + 1.0f) / 2.0f * WeRenderDevice::Instance()->GetResWidth());
				int y1 = (int)((1.0f - top) / 2.0f * WeRenderDevice::Instance()->GetResHeight());
				int y2 = (int)((1.0f - bottom) / 2.0f * WeRenderDevice::Instance()->GetResHeight());
				int cx = (x1 + x2) / 2;
				int cy = (y1 + y2) / 2;

				if ((Iterator->Type == WETYPE_LIGHT) ||
					(Iterator->Type == WETYPE_PARTICLESYSTEM) ||
					(Iterator->Type == WETYPE_PATHNODE))
				{
					if ((cx >= x) && (cx <= x + w) && (cy >= y) && (cy <= y + h))
					{
						NewSelection.PushBack(Iterator);
					}
				}
				else if (Crossing)
				{
					if ((x1 <= x + w) && (x2 >= x) && (y1 <= y + h) && (y2 >= y))
					{
						NewSelection.PushBack(Iterator);
					}
				}
				else
				{
					if ((x1 >= x) && (x2 <= x + w) && (y1 >= y) && (y2 <= y + h))
					{
						NewSelection.PushBack(Iterator);
					}
				}
			}
		}

		if (!Shift)
			Selections.DeselectAll();

		for (NewSelection.First(); !NewSelection.End(); NewSelection.Next())
			Selections.MakeSelection(NewSelection.GetItem(), true);

		NewSelection.Clear();
	}

	MouseLeftDown = false;
	MouseDeltaRoll = 0;

	if (!MouseRightDown)
	{
		//MouseRightDown = false;
		MouseDeltaX = 0;
		MouseDeltaY = 0;
		LastMouseDeltaX = 0;
		LastMouseDeltaY = 0;
	}

	Selecting = false;

	ReleaseCapture();
	ShowCursor(true);

	// Convert screen client coords to back buff coords
	/*if (Cam->GetViewMode() == CAMERA_FREE)
	{
	  x = WeRenderDevice::Instance()->GetResWidth()*x / GetClientWidth();
	  y = WeRenderDevice::Instance()->GetResHeight()*y / GetClientHeight();
	}

	if ((GetKeyState(VK_LSHIFT) & 0x80))
	{
		DoSelect(x,y,true);
	} else
	{
		DoSelect(x,y,false);
	}*/

}



void CMapEditorWindow::OnRButtonDown(int x, int y)
{
	if ((Moving || Rotating) && MouseLeftDown)
	{
		for (Selections.Selections.First(), Selections.OldTransformations.First();
			!Selections.Selections.End() && !Selections.OldTransformations.End();
			Selections.Selections.Next(), Selections.OldTransformations.Next())
		{
			Selections.Selections.GetItem()->SelectedActor->SetTransformation(Selections.OldTransformations.GetItem());
		}
	}

	MouseRightDown = true;
	MouseDeltaRoll = 0;

	if (!MouseLeftDown)
	{
		MouseOriginX = x;
		MouseOriginY = y;
		MouseDeltaX = 0;
		MouseDeltaY = 0;
		LastMouseDeltaX = 0;
		LastMouseDeltaY = 0;
	}


	ShowCursor(false);
	SetCapture(GetHWND());
}



void CMapEditorWindow::OnRButtonUp(int x, int y)
{
	MouseRightDown = false;
	//MouseLeftDown = false;
	MouseDeltaRoll = 0;
	MouseDeltaX = 0;
	MouseDeltaY = 0;
	LastMouseDeltaX = 0;
	LastMouseDeltaY = 0;


	ReleaseCapture();
	ShowCursor(true);
}


void CMapEditorWindow::OnEveryFrame()
{
	SaveTimer += WeTimer::Instance()->GetDeltaTimeF();
	if(SaveTimer >= 60.0f) SaveTemp = true;

	WeVector newLoc;

	Shift = false;
	Ctrl = false;
	Alt = false;

	if ((GetKeyState(VK_LSHIFT) & 0x80))
	{
		Shift = true;
	}
	if ((GetKeyState(VK_RSHIFT) & 0x80))
	{
		Shift = true;
	}
	if ((GetKeyState(VK_LCONTROL) & 0x80)) 
	{
  	   Ctrl = true;
	}
	if ((GetKeyState(VK_RCONTROL) & 0x80)) 
	{
		Ctrl = true;
	}
	if ((GetKeyState(VK_LMENU) & 0x80))
	{
		Alt = true;
	}
	if ((GetKeyState(VK_RMENU) & 0x80))
	{
		Alt = true;
	}

	if (Selections.Selections.GetItemsCount() == 0)
	{
		Moving = false;
		Rotating = false;
		WeRenderDevice::Instance()->SetClearColor(BACK_COLOR);
		// if we have sleection properties
		Selections.HideSelectionProperties();
	}


	MeshEditor->OnEveryFrame();

	if (MoveUp)
	{
		//if(Cam->GetViewMode() == CAMERA_FREE)
			Cam->MoveCamera(CAMERA_MOVEUP);
	}
	if (MoveDown)
	{
		//if(Cam->GetViewMode() == CAMERA_FREE)
			Cam->MoveCamera(CAMERA_MOVEDOWN);
	}
	if (MoveLeft)
	{
		//if(Cam->GetViewMode() == CAMERA_FREE)
			Cam->MoveCamera(CAMERA_MOVELEFT);
	}
	if (MoveRight)
	{
		//if(Cam->GetViewMode() == CAMERA_FREE)
			Cam->MoveCamera(CAMERA_MOVERIGHT);
	}

	if(SaveTemp)
	{
		MapEditorWorld->Save("Temp_SavedMap.tmp");
		SaveTimer = 0.0f;
		SaveTemp = false;
	}

	if (BakingLights)
	{
		CalculateStaticLights();
		BakingLights = false;
		SetCursor(LoadCursor(NULL,IDC_ARROW));
	}
}




void CMapEditorWindow::OnKeyUp(DWORD KeyCode)
{
	if (KeyCode == VK_UP)
	{
		MoveUp = false;
	}
	if (KeyCode == VK_DOWN)
	{
		MoveDown = false;
	}
	if (KeyCode == VK_LEFT)
	{
		MoveLeft = false;
	}
	if (KeyCode == VK_RIGHT)
	{
		MoveRight = false;
	}
	if (KeyCode == 0xBE) // .>
	{
		Orbiting = false;
		OrbitTarget = NULL;
	}
}





void CMapEditorWindow::OnKeyDown(DWORD KeyCode)
{
	if (KeyCode == VK_OEM_PLUS) Cam->SetScrollSpeed(Cam->ScrollSpeed + 10);
	if (KeyCode == VK_OEM_MINUS) Cam->SetScrollSpeed(Cam->ScrollSpeed - 10);


	if (KeyCode == VK_UP)
	{
		MoveUp = true;
	}
	if (KeyCode == VK_DOWN)
	{
		MoveDown = true;
	}
	if (KeyCode == VK_LEFT)
	{
		MoveLeft = true;
	}
	if (KeyCode == VK_RIGHT)
	{
		MoveRight = true;
	}

	else if(KeyCode == VK_ESCAPE)
	{
		Selections.DeselectAll();
	}


	else if (KeyCode == '1' && Shift)
	{
		OnClick(ID_EDITORS_TEXTUREEDITOR);
	}
	else if (KeyCode == '2' && Shift)
	{
		OnClick(ID_EDITORS_ANIMTEXTUREEDITOR);
	}
	else if (KeyCode == '3' && Shift)
	{
		OnClick(ID_EDITORS_MATERIALEDITOR);
	}
	else if (KeyCode == '4' && Shift)
	{
		OnClick(ID_EDITORS_MESHEDITOR);
	}
	else if (KeyCode == '5' && Shift)
	{
		OnClick(ID_EDITORS_ANIMMESHEDITOR);
	}
	else if (KeyCode == '6' && Shift)
	{
		OnClick(ID_EDITORS_MODELEDITOR);
	}
	else if (KeyCode == '7' && Shift)
	{
		OnClick(ID_EDITORS_EFFECTEDITOR);
	}
	else if (KeyCode == '8' && Shift)
	{
		OnClick(ID_EDITORS_TUBERSEDITOR);
	}
	else if (KeyCode == '9' && Shift)
	{
		OnClick(ID_EDITORS_PARTICLESYSTEMEDITOR);
	}
	else if (KeyCode == '0' && Shift)
	{
		OnClick(ID_EDITORS_SOUNDEDITOR);
	}


	else if ((KeyCode >= '0') && (KeyCode <= '9') && Ctrl)
	{
		Bookmark[(int)KeyCode - (int)'0'] = Cam->Location;
	}

	else if ((KeyCode >= '0') && (KeyCode <= '9') && Alt)
	{
		Cam->Location = Bookmark[(int)KeyCode - (int)'0'];
	}


	else if (KeyCode == '`')
	{
		if (MapEditorWorld)
		{
			WeActor *FirstActor = MapEditorWorld->Actors->NextActor;
			if (FirstActor != MapEditorWorld->Actors)
			{
				Cam->Location = FirstActor->Location;
			}
		}
	}

	else if (KeyCode == '1') Cam->SetViewMode(CAMERA_FREE);
	else if (KeyCode == '2') Cam->SetViewMode(CAMERA_TOP);
	else if (KeyCode == '3') Cam->SetViewMode(CAMERA_FRONT);
	else if (KeyCode == '4') Cam->SetViewMode(CAMERA_LEFT);

	else if (KeyCode == '8') WeShaderManager::Instance()->SetQuality(SHADER_LOW);
	else if (KeyCode == '9') WeShaderManager::Instance()->SetQuality(SHADER_MEDIUM);
	else if (KeyCode == '0') WeShaderManager::Instance()->SetQuality(SHADER_HIGH);


	else if (KeyCode == 'J')
	{
		if (Shift)
		{
			ShowPathNode = true;
			MapEditorWorld->Editor_ShowPathNodes();
		}
		else
			ShowAllHidden();
	}

	else if (KeyCode == 'H') 
	{
		if (Shift)
		{
			ShowPathNode = false;
			MapEditorWorld->Editor_HidePathNodes();
		}
		else
			HideSelected();
	}

	else if (KeyCode == 'K') HideUnselected();

	else if (KeyCode == '7') 
	{
		//static bool b = false;
		//MapEditorWorld->SetZonedRendering(!b);
		//WeRenderDevice::Instance()->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, b);
		//b = !b;
		WeActor *Actor;
		WeMatrix M2;
		WeVector Location;


		for (int i = 0; i < 2000; i++)
		{
			D3DXMatrixIdentity(&M2);
			Location.x = (float)(rand() % 2000 - 1000);
			Location.y = (float)(rand() % 2000 - 1000);
			Location.z = (float)(rand() % 2000 - 1000);
			//Location.x = 4000;
			//Location.y = 4000;
			//Location.z = 4000;

			M2._41 = Location.x;
			M2._42 = Location.y;
			M2._43 = Location.z;

			Actor = new WeActor();
			Actor->ModelName = "MapEditor.Geometry.BoxNMap";		
			//Actor->InEditor = false;
			Actor->Location = Location;			
			Actor->TM = M2;
			//Actor->Model = WeModelManager::Instance()->Load("MapEditor.Geometry.BoxNMap");
			Actor->Initialize();
			Actor->Model->UpdateTransformations(M2, M2);
			//SAFE_DELETE(Actor->PhysicHandle);
			//Actor->IsPrefab = false;
			MapEditorWorld->AddActor(Actor);
			//if (Actor->IsPrefab) Actor->LightMap.GenerateDefault(Actor);
			//Actor->SetTransformation(M2);
			if (i % 1000 == 0) MessageBox(0, "1000 ti", 0, 0);
		}
	}


	else if (KeyCode == VK_F5)
	{
		BakingLights = true;
		SetCursor(LoadCursor(NULL,IDC_WAIT));
		//CalculateStaticLights();
	}
	else if (KeyCode == VK_F6) CalculateSelectedLights();
	else if (KeyCode == VK_F7) CalculateStaticLights(true);
	else if (KeyCode == VK_F8) CalculateSelectedLights(true);
	else if (KeyCode == VK_F9) CalculateSelectedLightsActors(false);
	else if (KeyCode == VK_F10) CalculateSelectedLightsActors(true);

	else if (KeyCode == VK_TAB) Selections.ShowSelectionProperties();
	else if (KeyCode == 'C' && !Ctrl) 
	{
		CDialogActionManagment::Instance()->SetVisible(!CDialogActionManagment::Instance()->GetVisible());
	}
	else if (KeyCode == VK_RETURN)
	{
		CDialogActionManagment::Instance()->OnPlayButtonClick();
	}

	
	else if (KeyCode == 'S' && Ctrl)
	{
		SaveMap(true);
	}


	else if (KeyCode == 'G')
	{
		Selections.RotateCWYAxis();
	}

	else if (KeyCode == 'P')
	{
		WeRenderDevice::Instance()->SetRenderParticles(!WeRenderDevice::Instance()->GetRenderParticles());
	}


	else if (KeyCode == VK_SPACE)
	{
		if (ReInsertHoleName != (String)"")
		{
			InsertObject(ReInsertType, ReInsertHoleName);
		}
	}

	else if (KeyCode == 'N')
	{
		Selections.Selections.First();
		ReInsertType = Selections.Selections.GetItem()->SelectedActor->Type;
		ReInsertHoleName = Selections.Selections.GetItem()->SelectedActor->ModelName;
		ReInsertLocation = Selections.Selections.GetItem()->SelectedActor->Location;
	}

	else if (KeyCode == 'C' && Ctrl) 
	{
		Selections.CloneSelection();
	}


	//if (KeyCode == 'V' && Ctrl) MessageBox(0, "Paste", 0, 0);

	
	/*if (KeyCode == 'k' || KeyCode == 'K' ) 
	{
		MessageBox(0, "play", 0, 0);
		CActionManager::Instance()->DoAction("Demo Action");
	}

	if (KeyCode == 'o' || KeyCode == 'O' ) 
	{
		MessageBox(0, "NewAction", 0, 0);
		CActionManager::Instance()->NewAction("Demo Action");
	}
	if (KeyCode == 'p' || KeyCode == 'P' ) 
	{
		MessageBox(0, "StopRec", 0, 0);
		CActionManager::Instance()->StopRecording();
	}*/


	else if(KeyCode == 'A' && Ctrl)
	{
		DoArrayClone();
	}
	else if(KeyCode == 'Q' && Ctrl)
	{
		DoRadialArrayClone();
	}
	else if((KeyCode == 'Z') && Ctrl)
	{
		//CUndo::Instance()->Undo();
	}

	else if (KeyCode == 'Y' && Ctrl) 
	{
		//CUndo::Instance()->Redo();
	}

	else if(KeyCode == 'F')
	{
		Selections.RotateCW();
	}

	else if(KeyCode == 'E')
	{
		Selections.RotateCWYAxis();
	}

	else if ((KeyCode == 'A') && (Moving || Rotating) && !Ctrl) 
	{		
		LockedX = !LockedX;		
		if (LockedX) LockedY = false;
	}

	else if ((KeyCode == 'S') && (Moving || Rotating)) 
	{		
		LockedY = !LockedY;		
		if (LockedY) LockedX = false;
	}
	
	else if ((KeyCode == 'L') && Moving)
	{
		LocalMove = !LocalMove;		
	}

	else if ((KeyCode == 'L') && Rotating)
	{
		LocalRotate = !LocalRotate;		
	}


	else if (KeyCode == 'W')
	{
		WeRenderDevice::Instance()->SetWireFrame(!WeRenderDevice::Instance()->GetWireFrame());
	}

	else if (KeyCode == 'M') 
	{
		// if we have nothing selected
		if (Rotating) Rotating = false;

		Selections.Selections.First();
		if (Selections.Selections.GetItemsCount() > 0)
		{
			Moving = !Moving;			

			if (!Moving)
			{
				WeRenderDevice::Instance()->SetClearColor(BACK_COLOR);
			}
			else
			{
				WeRenderDevice::Instance()->SetClearColor(D3DCOLOR_XRGB(100,186,219));
				Selections.StartLocation = Selections.Selections.GetItem()->SelectedActor->Location;
				Selections.DeltaMoving = WeVector(0.0f,0.0f,0.0f);
			}
		}
		else
		{
			WeRenderDevice::Instance()->SetClearColor(BACK_COLOR);
			Moving = false;	
		}

		if (!Moving) 
		{
			LockedX = false;
			LockedY = false;
		}

		if (Moving)	CUndo::Instance()->MakeUndoItem(UNDO_ACTORSTATECHANGE);
	}

	// Rotatiion
	else if (KeyCode == 'R') 
	{
		if (Moving) 
		{
			Moving = false;	
			LocalMove = false;
			LockedX = false;
			LockedY = false;
		}


		// if we have nothing selected
		if (Selections.Selections.GetItemsCount() > 0)
		{
			if (Rotating)
				WeRenderDevice::Instance()->SetClearColor(BACK_COLOR);
			else
			{
				WeRenderDevice::Instance()->SetClearColor(D3DCOLOR_XRGB(107,218,101));
				Selections.StartRotation = WeVector(0.0f,0.0f,0.0f);
				Selections.DeltaRotation = WeVector(0.0f,0.0f,0.0f);
			}

			Rotating = !Rotating;
		}
		else
		{
			WeRenderDevice::Instance()->SetClearColor(BACK_COLOR);
			Rotating = false;			
		}
		if (Rotating)  CUndo::Instance()->MakeUndoItem(UNDO_ACTORSTATECHANGE);

	}

	else if (KeyCode == VK_DELETE) 
	{
		CUndo::Instance()->MakeUndoItem(UNDO_DELETE);		
		Selections.DeleteSelected();
	}

	else if (KeyCode == 'I')
	{
		ShowInfo = !ShowInfo;
	}

	else if (KeyCode == 'O')
	{
		CDialogInsertObject::Instance()->SelectedType = WETYPE_ACTOR;
		CDialogInsertObject::Instance()->Show();
	}

	else if (KeyCode == 0xBE) // .>
	{
		if (Selections.Selections.GetItemsCount())
		{
			Orbiting = true;
			Selections.Selections.First();
			OrbitTarget = Selections.Selections.GetItem()->SelectedActor;
		}
		else
		{
			Orbiting = false;
			OrbitTarget = NULL;
		}
	}

	else if (KeyCode == 0xBF) // /?
	{
		CDialogSelection::Instance()->Show();
	}
}


void CMapEditorWindow::OnMouseMove(int x, int y)
{
	//if (!Moving && !Rotating && !MouseLeftDown) return;

	if (x < 0)
		x = 0;
	else if (x >= GetWidth())
		x = GetWidth() - 1;

	if (y < 0)
		y = 0;
	else if (y >= GetHeight())
		y = GetHeight() - 1;

	MouseDeltaX = x - MouseOriginX;
	MouseDeltaY = y - MouseOriginY;

	x = MouseDeltaX - LastMouseDeltaX;
	y = MouseDeltaY - LastMouseDeltaY;

	LastMouseDeltaX = MouseDeltaX;
	LastMouseDeltaY = MouseDeltaY;

	if (!Selecting)
	{
		if (Shift && !Ctrl)
		{
			x = (int)(x * 50 * Cam->fScrollSpeed);
			y = (int)(y * 50 * Cam->fScrollSpeed);
		}	
		else if (Shift && Ctrl)
		{
			x = (int)(x * Cam->fScrollSpeed);
			y = (int)(y * Cam->fScrollSpeed);
		}	
		else
		{
			x = (int)(x * 10 * Cam->fScrollSpeed);
			y = (int)(y * 10 * Cam->fScrollSpeed);
		}
	}

	if (Moving)
		Selections.Move(x, y, MouseLeftDown, MouseRightDown, LockedX, LockedY, LocalMove);
	else if (Rotating)
		Selections.Rotate(x, y, MouseLeftDown, MouseRightDown, LockedX, LockedY, LocalRotate);
	else if (!Selecting)
		Cam->MoveCamera(MouseDeltaX, MouseDeltaY, x, y, MouseLeftDown, MouseRightDown, MouseDeltaRoll);		
}



void CMapEditorWindow::InsertObject()
{
	if (CDialogActorTypes::Instance()->ShowModal(GetHWND()) != MR_OK) return;    

	// Some types require model to select other don`t
	if(CDialogActorTypes::Instance()->TypeSelected == WETYPE_ACTOR ||		
		CDialogActorTypes::Instance()->TypeSelected == WETYPE_CLOTH)
	{
		CDialogInsertObject::Instance()->SelectedType = CDialogActorTypes::Instance()->TypeSelected;
		CDialogInsertObject::Instance()->Show();
		return;
	}


	else if(CDialogActorTypes::Instance()->TypeSelected == ACTORTYPE_PHYSICMODEL)
	{
		InsertDialog->Type = PACKAGEITEM_PHYSICMODEL;
		if(InsertDialog->ShowModal(GetHWND()) != MR_OK) return;		
	}

	else if(CDialogActorTypes::Instance()->TypeSelected == PICKUPTYPE_GLOBAL)
	{
		if(CDialogPickupList::Instance->ShowModal(this->GetHWND()) == MR_OK)
		{
			InsertObject(CDialogPickupList::Instance->Type, "");
		}
		return;
	}

 	

	InsertObject(CDialogActorTypes::Instance()->TypeSelected, InsertDialog->HoleName);
	
	// Now Create Object of that type...


	//CTubePart* TubePart = new CTubePart();
	//TubePart->Initialize(Location, MapEditorWorld, InsertDialog->HoleName);	
	//if (OrientationSet) 
	//{
	//	//TubePart->SetTransformation(Orientation.x, Orientation.y, Orientation.z);
	//	TubePart->SetTransformation(M2);
	//}
	//CUndo::Instance()->MakeUndoItem(UNDO_CREATE, TubePart);
	//CActionManager::Instance()->RecordAction(ACTIONTYPE_NEWOBJECT, TubePart);

}


void CMapEditorWindow::InsertObject(int Type, StringParam HoleName)
{
	WeVector Location;
	WeVector Orientation;
	WeMatrix tmpM, M2, M3;
	bool OrientationSet = false;

	Location = Cam->Location + (Cam->Direction * 50);
	D3DXMatrixIdentity(&M2);

	WeActor *SelectedForCloning = NULL; 
	int MaterialPreset = 0; 
	
	
	// if we selectected group with some name...
	if (Selections.Selections.GetItemsCount() == 1)
	{			
		// take info from his group name
		Selections.Selections.First();
		SelectedForCloning = Selections.Selections.GetItem()->SelectedActor;
		MaterialPreset = Selections.Selections.GetItem()->SelectedActor->MaterialPreset;

		if (Selections.Selections.GetItem()->SelectedActor->Model)
		{			
			String Group;
			Group = Selections.Selections.GetItem()->SelectedActor->Model->Name.Extract('.', 1);

			CTubeSegmentInfo *sinfo;
			sinfo = CTubeSegment::Instance()->GetSegment(Group);
			if (sinfo)
			{
				int PivotIndex = 0;
				// We need to show dialog where you can choose where to attach
				if (sinfo->NumPivots > 1)
				{   
					// choose pivot index from dialog
					CDialogTubeConnect::Instance()->Sinfo = sinfo;
					if (CDialogTubeConnect::Instance()->ShowModal(GetHWND()) == MR_OK)
					{
						PivotIndex = CDialogTubeConnect::Instance()->PivotIndex;
					} 
				} 

				WeVector lx,ly,lz;

				Selections.Selections.First();
				Selections.Selections.GetItem()->SelectedActor->GetLocalAxis(lx,ly,lz);
				Location = Selections.Selections.GetItem()->SelectedActor->Location;			
				lx = lx*sinfo->Pivots[PivotIndex].x;
				ly = ly*sinfo->Pivots[PivotIndex].y;
				lz = lz*sinfo->Pivots[PivotIndex].z;

				Location += lx; 
				Location += ly; 
				Location += lz; 

				WeVector orient;

				Selections.Selections.GetItem()->SelectedActor->GetTransformation(tmpM);
				tmpM._41 = 0;
				tmpM._42 = 0;
				tmpM._43 = 0;

                D3DXMatrixRotationYawPitchRoll(&M2, sinfo->Orientations[PivotIndex].x, sinfo->Orientations[PivotIndex].y, sinfo->Orientations[PivotIndex].z);
				M2 = M2*tmpM;
			    D3DXMatrixTranslation(&M3, Location.x, Location.y, Location.z);
				M2 = M2*M3;


				//Selections.Selections.GetItem()->SelectedActor->GetOrientation(orient.x, orient.y, orient.z);
				//Orientation = orient + sinfo->Orientations[PivotIndex];
				OrientationSet = true;
			} 
		}  // has model
	} // is something selected

	if (!OrientationSet)
	{
		M2._41 = Location.x;
		M2._42 = Location.y;
		M2._43 = Location.z;
	}


    WeActor *Actor;  
	switch (Type)
	{
		case WETYPE_ACTOR:						
			Actor = new WeActor();
			Actor->PhysicModel.Type = PHYSICMODEL_STATIC_SHAPE;
			Actor->ModelName = HoleName;	
			break;

		case WETYPE_CLOTH:
			Actor = new WeCloth();		
			Actor->ModelName = HoleName;	
			break;

		case WETYPE_LIGHT:
			Actor = new WeLight();
			((WeLight *)Actor)->IsDynamic = false;
			break;

		case WETYPE_PARTICLESYSTEM:
			Actor = new WeParticleSystem();
			break;

		case WETYPE_PATHNODE:
			Actor = new WePathNode();
			((WePathNode *)Actor)->SequenceNumber = WePathNode::NewSequenceNumber();
			break;

		case ACTORTYPE_STARTINGLOCATION:
			Actor = new CStartLocation();
			MapEditorWorld->MapHeader.StartLocations.PushBack((CStartLocation*)Actor);
			break;

		case ACTORTYPE_PHYSICMODEL:						
			WePhysics::Instance()->MakeFromPhysicModel(HoleName, MapEditorWorld, M2, true);

			ReInsertType  = Type;
			ReInsertHoleName = HoleName;
			ReInsertLocation = Location;
			ReTM = M2;
			ReOrientationSet = OrientationSet;
			return;
			break;

		case WETYPE_ZONE:
			Actor = new WeZone();
			MaterialPreset = 1;			
			break;

		case WETYPE_PORTAL:
			Actor = new WePortal();
			break;

		default:
			Actor = CPickUpFactory::Instance->MakePickUp(Type);
			break;
	}

	/*if (SelectedForCloning)
	{
		SelectedForCloning->Clone(Actor);
	}*/
	Actor->MaterialPreset = MaterialPreset;

	Actor->InEditor = true;
	Actor->Location = Location;
	Actor->TM  = M2;
	Actor->Initialize();	
	MapEditorWorld->AddActor(Actor);
	if (Actor->IsPrefab) Actor->LightMap.GenerateDefault(Actor);

	if (OrientationSet)  Actor->SetTransformation(M2);
	CUndo::Instance()->MakeUndoItem(UNDO_CREATE, Actor);
	CActionManager::Instance()->RecordAction(Actor);

	ReInsertType  = Type;
	ReInsertHoleName = HoleName;
	ReInsertLocation = Location;
	ReTM = M2;
	ReOrientationSet = OrientationSet;
}



void CMapEditorWindow::DoSelect(int x, int y, bool AddToSelection)
{
	WeActor* tmpActor = NULL;

	Selections.DoSelection(x, y, AddToSelection);
	//DoSelectionProperties();
}



void CMapEditorWindow::DoSelectionProperties()
{
	//if (!Selection->IsSelected()) return;


	//switch (Selection->GetSelectionType())
	//{
	//case ACTORTYPE_TUBEPART: 
	//	CDialogTubePart::Instance()->Show((CTubePart *)Selection->SelectedActor);
	//	break;
	//case ACTORTYPE_DYNAMICTUBEPART:		
	//	CDialogTubeDynamic::Instance()->Show((CTubeDynamic *)Selection->SelectedActor);
	//	break;
	//case WE_PATHNODE:		
	//	CDialogPathNode::Instance()->Show((CPathNode *)Selection->SelectedActor);
	//	break;
	//}


}


void CMapEditorWindow::HideSelected()
{
	for (Selections.Selections.First(); !Selections.Selections.End(); Selections.Selections.Next())
	{
		Selections.Selections.GetItem()->SelectedActor->Editor_Hide();
		HiddenObjects.PushBack(Selections.Selections.GetItem()->SelectedActor->ModelName);
	}
	Selections.DeselectAll();	
}


void CMapEditorWindow::HideUnselected()
{
	MapEditorWorld->Editor_HideAll();
	
	HiddenObjects.PushBack("ALL UNSELECTED OBJECTS");

	for (Selections.Selections.First(); !Selections.Selections.End(); Selections.Selections.Next())
	{
		Selections.Selections.GetItem()->SelectedActor->Editor_Show();
	}

	Selections.DeselectAll();
}





void CMapEditorWindow::ShowAllHidden()
{
	MapEditorWorld->Editor_ShowAll();
	ShowPathNode = true;
	HiddenObjects.Clear();
}


void CMapEditorWindow::MapProperties()
{
}



void CMapEditorWindow::ShutDown()
{
	Selections.DeselectAll();
	SAFE_DELETE(MapEditorWorld);
	SAFE_DELETE(Cam);
}


void CMapEditorWindow::DoAlign()
{
	int Align = 0;

	if (Selections.Selections.GetItemsCount() < 2) return;
	if (CDialogAlign::Instance()->ShowModal(EditorWindow->GetHWND()) == MR_OK)
	{
		if (CDialogAlign::Instance()->alX) Align |= ALIGN_PIVOT_X;
		if (CDialogAlign::Instance()->alY) Align |= ALIGN_PIVOT_Y;
		if (CDialogAlign::Instance()->alZ) Align |= ALIGN_PIVOT_Z;
		if (CDialogAlign::Instance()->alYaw) Align |= ALIGN_ROTATION_YAW;
		if (CDialogAlign::Instance()->alPitch) Align |= ALIGN_ROTATION_PITCH;
		if (CDialogAlign::Instance()->alRoll) Align |= ALIGN_ROTATION_ROLL;

		if (CDialogAlign::Instance()->alAxis) Align |= ALIGN_AXIS;

		Selections.Align(Align);
	}
}


void CMapEditorWindow::DoArrayClone()
{
	if (CDialogInput::Instance()->ShowModal(GetHWND()) == MR_OK)
	{
		Selections.CloneArray(CDialogInput::Instance()->InputText.AsInt());
	}
}

void CMapEditorWindow::DoRadialArrayClone()
{
	if (CDialogInput::Instance()->ShowModal(GetHWND()) == MR_OK)
	{
		Selections.CloneRadialArray(CDialogInput::Instance()->InputText.AsInt());
	}	
}






void CMapEditorWindow::OnShowHideZonesAndPortals()
{
	if(ShowZonesAndPortals)
	{
		MapEditorWorld->Editor_HideZones();
		MapEditorWorld->Editor_HidePortals();
	}else{
		MapEditorWorld->Editor_ShowZones();
		MapEditorWorld->Editor_ShowPortals();
	}

	ShowZonesAndPortals = !ShowZonesAndPortals;
}


