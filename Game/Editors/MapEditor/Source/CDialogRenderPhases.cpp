#include <CDialogRenderPhases.h>
#include <CEditorResourceManager.h>
#include <CMapEditorWindow.h>
#include <CDialogTools.h>
#include <CMapEditorCamera.h>
#include <CEditorMap.h>



UINT RenderPhaseIDC[RENDERPHASE_COUNT] = {
	IDC_RENDERPHASE_AMBIENT,
	IDC_RENDERPHASE_AMBIENTALPHA ,
	IDC_RENDERPHASE_LIGHTING,
	IDC_RENDERPHASE_ALPHA,
	IDC_RENDERPHASE_PROJECTORS,
	IDC_RENDERPHASE_GLOW,
	IDC_RENDERPHASE_REFRACTIONS,
	IDC_RENDERPHASE_FOREGROUND,
	IDC_RENDERPHASE_FLARES,
	IDC_RENDERPHASE_SKYBOX
};



CDialogRenderPhases* DialogRenderPhases = NULL;

bool CDialogRenderPhases::Create(HWND hParent, bool Show)
{
	if (!CDialog::Create(IDD_RENDERPHASES, hParent, false))
		return false;

	for (int RenderPhase = 0; RenderPhase < RENDERPHASE_COUNT; ++RenderPhase)
	{
		CheckRenderPhase[RenderPhase] = new CCheckBox(this, RenderPhaseIDC[RenderPhase]);
		CheckRenderPhase[RenderPhase]->SetChecked(true);
	}

	return true;
}



void CDialogRenderPhases::OnShow()
{
	for (int RenderPhase = 0; RenderPhase < RENDERPHASE_COUNT; ++RenderPhase)
		CheckRenderPhase[RenderPhase]->SetChecked(EditorWindow->MapEditorWorld->RenderPhaseEnabled(RenderPhase));
}



void CDialogRenderPhases::OnHide()
{
}



void CDialogRenderPhases::OnClick(DWORD ID)
{
	if (ID == IDOK)
	{
		Hide();
		return;
	}

	if (ID == IDC_RENDERPHASE_ALL)
	{
		for (int RenderPhase = 0; RenderPhase < RENDERPHASE_COUNT; ++RenderPhase)
		{
			CheckRenderPhase[RenderPhase]->SetChecked(true);
			EditorWindow->MapEditorWorld->EnableRenderPhase(RenderPhase, true);
		}
		return;
	}

	for (int RenderPhase = 0; RenderPhase < RENDERPHASE_COUNT; ++RenderPhase)
	{
		if (ID == RenderPhaseIDC[RenderPhase])
		{
			EditorWindow->MapEditorWorld->EnableRenderPhase(RenderPhase, CheckRenderPhase[RenderPhase]->GetChecked());
			return;
		}
	}
}


