/*
      File: CMapEditorWindow.h
	  Author: Tomislav Kukic
	  Date: 04.11.2005.
	  Version: 1.0
*/




#ifndef _CMAPEDITORWINDOW
#define _CMAPEDITORWINDOW


#include <CString.h>
#include <CEditorWindow.h>
#include <WeVector.h>
#include <CMassSelection.h>
#include <CEditorGrid.h>
#include <CActionManager.h>
#include <WeMatrix.h>
#include <WePathNode.h>


#define PACKAGEITEM_TEXTURE		1
#define PACKAGEITEM_MATERIAL	2
#define PACKAGEITEM_EFFECT		3
#define PACKAGEITEM_SOUND		4
#define PACKAGEITEM_MESH		5
#define PACKAGEITEM_MODEL		6
#define PACKAGEITEM_TUBER		7
#define PACKAGEITEM_PARTICLESYS	8
#define PACKAGEITEM_ANIMTEXTURE	9
#define PACKAGEITEM_ANIMMODEL	10
#define PACKAGEITEM_PHYSICMODEL 10200

class WeMesh;
class WeCamera;
class CEditorMap;
class CMapEditorCamera;

#define BACK_COLOR D3DCOLOR_XRGB(100,100,100)

extern class CMapEditorWindow *EditorWindow;


class CMapEditorWindow : public CEditorWindow
{
protected:
	void OnClick(DWORD ID);	
	void OnDblClk(int x, int y);
	void OnLButtonDown(int x, int y);
	void OnLButtonUp(int x, int y);
	void OnRButtonDown(int x, int y);
	void OnRButtonUp(int x, int y);
	void OnMouseMove(int x, int y);
	void OnBeforeClose(bool &AcceptClose);
	void OnKeyDown(DWORD KeyCode);
	void OnKeyUp(DWORD KeyCode);
	void OnEveryFrame();	
	void OnMouseWheel(WORD Position);
    
	void OnResize(int neww, int newh, DWORD ResizeType);

	String MapFile;

	float OldMouseX;
	float OldMouseY;
	float SaveTimer;
	int	  Speed;
	

	CEditorGrid	*Grid;

	bool MoveUp;
	bool MoveDown;
	bool MoveLeft;
	bool MoveRight;
	bool SaveTemp;

	bool ShowPathNode;
	bool ShowZonesAndPortals;

	void CheckPathNodes();
	void FixStartLocations();

	void LoadStates();
	void SaveStates();

	void DoSaveMap();

public:
	CMassSelection Selections;

	bool  Moving; // this is toggled if selection is moving 
	bool  Rotating;
	bool  Selecting;
	bool  Orbiting;
	WeActor *OrbitTarget;
	bool  LockedX;
	bool  LockedY;
	bool  LocalMove;
	bool  LocalRotate;
	bool  Shift;
	bool  Ctrl;
	bool  Alt;

	bool BakingLights;

	void GetBBoxViewProjRect(WeSphere &sphere, WeCamera *Cam, float &left, float &right, float &bottom, float &top);
	void GetBBoxViewProjRect(WeBox &box, WeCamera *Cam, float &left, float &right, float &bottom, float &top);


	CMapEditorWindow();
	~CMapEditorWindow();

	void Initialize();
	void Terminate();

	CEditorMap*	MapEditorWorld;
    CMapEditorCamera*	Cam;


	float MouseX;
	float MouseY;

	float Yaw;
	float Pitch;

	bool MouseLeftDown;
	bool MouseRightDown;
	short  MouseDeltaRoll;	
	short  OldMouseDeltaRoll;
	int  MouseDeltaX;
	int  MouseDeltaY;
	int  MouseOriginX;
	int  MouseOriginY;
	int  LastMouseDeltaX;
	int  LastMouseDeltaY;

	
	bool MouseRotation;
	bool MouseMoveFB;
	bool MouseMoveUD;


	WeVector	Bookmark[10];
	bool		ShowInfo;


	int			ReInsertType;
	String      ReInsertHoleName;
	WeVector	ReInsertLocation;
	WeMatrix	ReTM;
	bool		ReOrientationSet;
	Strings		HiddenObjects;

	void LoadMap();
	void SaveMap(bool FromShortCut = false);
	void ExportToOBJ();
	void NewMap();

	bool Create();

	void DoArrayClone();
	void DoRadialArrayClone();

	void CalculateStaticLights(bool Simple = false);
	void CalculateSelectedLights(bool Simple = false);
	void CalculateSelectedLightsActors(bool Simple = false);

	void InsertObject(int Type, StringParam HoleName);
	void InsertObject();
	void DoSelect(int x, int y, bool AddToSelection);
	void DoSelectionProperties();
	void DoAlign();
	void MapProperties();
	void ShutDown();
	void HideSelected();
	void HideUnselected();
	void ShowAllHidden();
	void OnShowHidePathNodes();
	void OnShowHideZonesAndPortals();

};



#endif
