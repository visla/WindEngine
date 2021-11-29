/*
    CCreateDialogsPhase.cpp
	Autor: Tomislav Kukic
	Date: 04.11.2005.
	Version: 1.0
*/


#include <IPhase.h>
#include <CDialogPickupProperties.h>
#include <CMapEditorWindow.h>
#include <CCreateDialogsPhase.h>
#include <CDialogStartLoacationProperties.h>
#include <CDialogAnimTextureEditor.h>
#include <CDialogMaterialEditor.h>
#include <CDialogMeshEditor.h>
#include <CDialogInfo.h>
#include <CDialogInsert.h>
#include <CDialogSoundEditor.h>
#include <CDialogModelEditor.h>
#include <CDialogEffectEditor.h>
#include <CDialogAbout.h>
#include <CDialogHelp.h>
#include <CDialogPostProcess.h>
#include <CDialogSave.h>
#include <CDialogTextureEditor.h>
#include <CDialogInsertObject.h>
#include <CDialogActor.h>
#include <CDialogPathNode.h>
#include <CDialogLight.h>
#include <CDialogAlign.h>
#include <CDialogTubeConnect.h>
#include <CDialogActionManagment.h>
#include <CDialogInput.h>
#include <CDialogParticleSystem.h>
#include <CDialogParticleSystemEditor.h>
#include <CDialogActorTypes.h>
//#include <CDialogMaterialPresets.h>
#include <CDialogSkyBox.h>
#include <CDialogTubersEditor.h>
#include <CDialogMapDescription.h>
#include <CDialogAIProfile.h>
#include <CDialogAIProfiles.h>
#include <CDialogPickupList.h>
#include <CDialogTournament.h>
#include <CDialogTournamentEditor.h>
#include <CDialogTournamentMapSettings.h>
#include <CDialogTournamentOpenClose.h>
#include <CDialogPhysicModelEditor.h>
#include <CDialogBaking.h>
#include <CDialogCloth.h>
#include <CDialogZone.h>
#include <CDialogPortal.h>
#include <CDialogAnimatedMesh.h>
#include <CDialogSelection.h>
#include <CDialogRenderPhases.h>
#include <CDialogPhysicProperties.h>



PHASES CCreateDialogsPhase::Start()
{
	InsertDialog = new CDialogInsert();
	InsertDialog->Create(EditorWindow->GetHWND(), false);
	SaveDialog = new CDialogSave();
	SaveDialog->Create(EditorWindow->GetHWND(), false);
	TextureEditor = new CDialogTextureEditor();
	TextureEditor->Create(EditorWindow->GetHWND(),false);	
	MaterialEditor = new CDialogMaterialEditor();
	MaterialEditor->Create(EditorWindow->GetHWND(),false);
	MeshEditor = new CDialogMeshEditor();
	MeshEditor->Create(EditorWindow->GetHWND(),false);
	ModelEditor = new CDialogModelEditor();
	ModelEditor->Create(EditorWindow->GetHWND(), false);
	EffectEditor = new CDialogEffectEditor();
	EffectEditor->Create(EditorWindow->GetHWND(), false);
	CDialogActor::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogLight::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogPathNode::Instance()->Create(EditorWindow->GetHWND(), false);	
	CDialogAlign::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogTubeConnect::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogActionManagment::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogInput::Instance()->Create(EditorWindow->GetHWND(), false);
	DialogInfo = new CDialogInfo();
	DialogInfo->Create(EditorWindow->GetHWND(), false);
	DialogHelp = new CDialogHelp();
	DialogHelp->Create(EditorWindow->GetHWND(), false);
	DialogAbout = new CDialogAbout();
	DialogAbout->Create(EditorWindow->GetHWND(), false);
	CDialogPostProceses::Instance()->Create(EditorWindow->GetHWND(),false);
	ParticleSystemEditor = new CDialogParticleSystemEditor();
	ParticleSystemEditor->Create(EditorWindow->GetHWND(), false);
	DialogSoundEditor = new CDialogSoundEditor();
	DialogSoundEditor->Create(EditorWindow->GetHWND(), false);
	CDialogParticleSystem::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogActorTypes::Instance()->Create(EditorWindow->GetHWND());
	//CDialogMaterialPresets::Instance()->Create(EditorWindow->GetHWND());
	CDialogSkyBox::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogTubersEditor::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogMapDescription::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogAIProfile::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogAIProfiles::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogPickupList::Instance->Create(EditorWindow->GetHWND(),false);
	CDialogPickupProperties::Instance()->Create(EditorWindow->GetHWND(),false);
	CDialogTournament::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogTournamentEditor::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogTournamentMapSettings::Instance()->Create(EditorWindow->GetHWND(), false);		
	CDialogTournamentOpenClose::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogStartLoacationProperties::Instance()->Create(EditorWindow->GetHWND(),false);
	CDialogInsertObject::Instance()->Create(EditorWindow->GetHWND(),false);
	CDialogPhysicModelEditor::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogCloth::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogAnimTextureEditor::Instance()->Create(EditorWindow->GetHWND(),false);
	CDialogZone::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogPortal::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogAnimatedMesh::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogBaking::Instance()->Create(EditorWindow->GetHWND(), false);
	CDialogSelection::Instance()->Create(EditorWindow->GetHWND(), false);
	DialogRenderPhases = new CDialogRenderPhases();
	DialogRenderPhases->Create(EditorWindow->GetHWND(), false);
	CDialogPhysicProperties::Instance()->Create(EditorWindow->GetHWND(), false);

	return PHASE_MAINLOOP;
}


