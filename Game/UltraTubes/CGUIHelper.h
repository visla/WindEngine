/*
     CGUIHelper.h

	 Autor: Nebojsa Vislavski
	 Date: 15.3.2006.
	 Version: 1.0

*/


#ifndef _CGUIHELPER_H
#define _CGUIHELPER_H

#include <WeCEGUI.h>
#include <CString.h>
#include <Wecolor.h>



void ReloadWindowConfig(CEGUI::Window *Wnd, StringParam UIName, bool UseExist,
						float x, float xx, float y, float yy, float w, float ww, float h, float hh);


class CGUIHelper
{
protected:
	//static bool OnButtonEnters(const CEGUI::EventArgs& e);
	//static bool OnButtonLeaves(const CEGUI::EventArgs& e);
	static bool OnButtonTitleEnters(const CEGUI::EventArgs& e);
	static bool OnButtonTitleLeaves(const CEGUI::EventArgs& e);
	static bool OnButtonMouseDown(const CEGUI::EventArgs& e);
	static bool OnButtonMouseUp(const CEGUI::EventArgs& e);

public:
	static void Initialize();
	static void ReloadAllWindowsConfig();

	static CEGUI::Window* CreateWnd(StringParam UIName, StringParam Caption, float titlebarX, float x, float y, float w, float h, StringParam ParentName, StringParam WndVisible = "true");
	static CEGUI::Combobox *CreateCombobox(StringParam UIName, float x, float y, float w, float h, Strings *rows, StringParam Parent, bool (*function)(const CEGUI::EventArgs &e));
	static CEGUI::Checkbox *CreateCheckbox(StringParam UIName, float x, float y, float w, float h, StringParam Parent, bool (*function)(const CEGUI::EventArgs &e));
	static CEGUI::Editbox* CreateEditbox(StringParam UIName, float x, float y, float w, float h, StringParam DefaultText, CEGUI::colour SelTextColor, StringParam Parent);
	static CEGUI::ProgressBar* CreateProgressBar(StringParam UIName, float x, float y, float w, float h, StringParam Parent); 
	static CEGUI::Window* CreateButton(StringParam UIName, StringParam Caption, float x, float y, float w, float h, StringParam ParentName, bool (*function)(const CEGUI::EventArgs &e));
	static CEGUI::MultiColumnList* CreateMultiList(StringParam UIName, float x, float y, float w, float h, CSmartList<String> &ColumnNames, bool EnableSorting, bool RowSelecting, StringParam ParentName);
	static CEGUI::Window* CreateText(StringParam UIName, StringParam Caption, float x, float y, float w, float h, StringParam FontName, StringParam HorzAlignment, StringParam VertAlignment, StringParam Parent);
	static CEGUI::Window* CreateStaticImage(StringParam UIName, StringParam Image, float x, float y, float w, float h, StringParam Parent);
	static CEGUI::Window* CreateStaticColor(StringParam UIName, WeColor &Color, float x, float y, float w, float h, StringParam Parent);
	static CEGUI::Scrollbar* CreateHorzScrollbar(StringParam UIName, float x, float y, float w, float h, float aPageSize, float aTotalSize, StringParam Parent, bool (*function)(const CEGUI::EventArgs &e));

	static inline CEGUI::UVector2 UVector2(float x, float y)
	{
		return CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0));
	}

	static bool WindowFade(StringParam WNDName, float &WNDAlpha);

	static void Render();
};



class MyListItem: public CEGUI::ListboxTextItem
{
protected:
	String ParentName;

public:
	MyListItem(StringParam text, StringParam aParentName);
	void SetSelectedColorConfigName(StringParam aSelectedColorConfigName);
};



#endif