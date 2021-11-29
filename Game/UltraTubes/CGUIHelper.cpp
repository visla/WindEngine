/*
     CGUIHelper.cpp

	 Autor: Nebojsa Vislavski
	 Date: 15.3.2006.
	 Version: 1.0

*/

#include <CGUIHelper.h>
#include <CString.h>
#include <WeCEGUI.h>
#include <WeTimer.h>
#include <CConfig.h>
#include <WeLoger.h>
#include <CEGUIPropertyHelper.h>
#include <Helper.h>
#include <CLocalizationSettings.h>
#include <CUTWindow.h>



struct CButtonStyle
{
	String Name;
	String TextFont;
	CEGUI::argb_t TextColorNormal;
	CEGUI::argb_t TextColorOver;
	CEGUI::argb_t TextColorClicked;
	float TextAlignX;
	float TextAlignY;
	int Width;
	int Height;
	float Fading;
};


struct CFrameStyle
{
	String Name;
};


struct CButtonData
{
	String Name;
	bool (*Function)(const CEGUI::EventArgs &e);
};


const String ConfigBaseName = (String)"cegui.Components.";


String ClickedButtonName;
String OverButtonName;
CSmartList<String> FadeOutButtonNames;
CSmartList<CButtonData> ButtonsData;


CSmartList<CButtonStyle> ButtonStyles;
CButtonStyle* DefaultButtonStyle = NULL;

int WindowTitleWidth, WindowTitleHeight;
CEGUI::colour TextColor;
CEGUI::colour CaptionColor;
String DefaultTextFont;
String CaptionFont;



CButtonStyle* GetButtonStyle(StringParam StyleName)
{
	for (ButtonStyles.First(); !ButtonStyles.End(); ButtonStyles.Next())
	{
		if (ButtonStyles.GetItem().Name == StyleName)
		{
			return &ButtonStyles.GetItem();
		}
	}

	return DefaultButtonStyle;
}



void ReloadWindowConfig(CEGUI::Window *Wnd, StringParam UIName, bool UseExist,
						float x, float xx, float y, float yy, float w, float ww, float h, float hh)
{
	String FinalUIName = UIName, ConfigStr, s;
	int DeltaY = 0;


	if (UseExist)
	{
		CEGUI::UVector2 pos = Wnd->getWindowPosition();
		x = pos.d_x.d_scale;
		xx = pos.d_x.d_offset;
		y = pos.d_y.d_scale;
		yy = pos.d_y.d_offset;

		CEGUI::UVector2 size = Wnd->getWindowSize();
		w = size.d_x.d_scale;
		ww = size.d_x.d_offset;
		h = size.d_y.d_scale;
		hh = size.d_y.d_offset;
	}


	if (Wnd->getType() == "TaharezLook/Combobox")
	{
		CEGUI::Combobox* combo = static_cast<CEGUI::Combobox*>(Wnd);
		h = 0.0f;
		hh = combo->getFont()->getLineSpacing() * 1.5f +
			combo->getItemCount() * combo->getFont()->getLineSpacing() * GetConfigFloat("cegui.List.VScale", 1.0f) +
			GetConfigFloat("cegui.List.VBase", 1.0f);
	}


	s = FinalUIName.Extract(' ', 1);
	if (s.GetLen() != 0)
	{
		char PostFix[100];
		int index = 0;

		PostFix[0] = (char)0;
		sscanf(s.GetString(), "%d%s", &index, PostFix);

		s = FinalUIName.Extract(' ', 0);

		if (PostFix[0] == (char)0)
		{
			DeltaY = index * GetConfigInt(ConfigBaseName + s + " DeltaY", 25);
		}

		FinalUIName = s + " 0" + PostFix;
	}

	ConfigStr = GetConfigString(ConfigBaseName + FinalUIName, "");

	s = ConfigStr.Extract(',', 1);
	if (s.GetLen() != 0)
	{
		xx = s.AsFloat() / 1024.0f * WeRenderDevice::Instance()->GetResWidth(); // / Wnd->getParentPixelWidth();
		x = 0.0f;
	}

	s = ConfigStr.Extract(',', 2);
	if (s.GetLen() != 0)
	{
		yy = (s.AsFloat() + DeltaY) / 768.0f * WeRenderDevice::Instance()->GetResHeight();// / Wnd->getParentPixelHeight();
		y = 0.0f;
	}

	s = ConfigStr.Extract(',', 3);
	if (s.GetLen() != 0)
	{
		ww = s.AsFloat() / 1024.0f * WeRenderDevice::Instance()->GetResWidth(); // / Wnd->getParentPixelWidth();
		w = 0.0f;
	}

	s = ConfigStr.Extract(',', 4);
	if (s.GetLen() != 0)
	{
		hh = s.AsFloat() / 768.0f * WeRenderDevice::Instance()->GetResHeight(); // / Wnd->getParentPixelHeight();
		h = 0;
	}


	ConfigStr = GetConfigString(ConfigBaseName + FinalUIName + "$", "");
	if (ConfigStr.GetLen() != 0)
	{
		int PartIndex = 0;
		String Part, PropertyName, PropertyValue;

		Part = ConfigStr.Extract(';', 0);
		while (Part.GetLen() != 0)
		{
			PropertyName = Part.Extract(':', 0);
			PropertyValue = Part.Extract(':', 1);

			if ((PropertyName.GetLen() != 0) && (PropertyValue.GetLen() != 0))
			{
				Wnd->setProperty(PropertyName.GetString(), PropertyValue.GetString());
			}

			++PartIndex;
			Part = ConfigStr.Extract(';', PartIndex);
		}
	}


	if (Wnd->getType() == "TaharezLook/MultiColumnList")
	{
		CEGUI::MultiColumnList* MultiList = static_cast<CEGUI::MultiColumnList*>(Wnd);

		ConfigStr = GetConfigString(ConfigBaseName + FinalUIName + "$ColumnWidths", "");
		if (ConfigStr.GetLen() != 0)
		{
			for (int i = 0; i < (int)MultiList->getColumnCount(); ++i)
			{
				s = ConfigStr.Extract(',', i);
				if (s.GetLen() != 0)
				{
					float width = s.AsFloat() / 1024.0f * WeRenderDevice::Instance()->GetResWidth();
					MultiList->setColumnHeaderWidth(i, CEGUI::UDim(0, width));
				}
			}
		}
	}


	Wnd->setWindowXPosition(CEGUI::UDim(x, xx));
	Wnd->setWindowYPosition(CEGUI::UDim(y, yy));
	Wnd->setWindowWidth(CEGUI::UDim(w, ww));
	Wnd->setWindowHeight(CEGUI::UDim(h, hh));
}



void CGUIHelper::Initialize()
{
	String Styles, StyleName, BaseName;
	int iStyle;


	DefaultTextFont = GetConfigString("cegui.Texts.DefaultFont", "Font_10");
	TextColor = CEGUI::colour(GetConfigHexInt("cegui.Texts.DefaultColor", 0xFFFFFFFF));

	CaptionFont = GetConfigString("cegui.Texts.CaptionFont", "Font_14");
	CaptionColor = CEGUI::colour(GetConfigHexInt("cegui.Texts.CaptionColor", 0xFFFFFFFF));

	CEGUI::System::getSingleton().setDefaultFont(DefaultTextFont.GetString());


	Styles = GetConfigString("cegui.Init.ButtonStyles", "");

	iStyle = 0;
	StyleName = Styles.Extract(',', iStyle);

	while (StyleName.GetLen() != 0)
	{
		BaseName = (String)"cegui.Button_" + StyleName;

		CButtonStyle bs;
		bs.Name = StyleName;
		bs.TextFont = GetConfigString(BaseName + ".TextFont", "Font_10");
		bs.TextColorNormal = CEGUI::PropertyHelper::stringToColour(GetConfigString(BaseName + ".TextColorNormal", "FF000000").GetString());
		bs.TextColorOver = CEGUI::PropertyHelper::stringToColour(GetConfigString(BaseName + ".TextColorOver", "FFFFFFFF").GetString());
		bs.TextColorClicked = CEGUI::PropertyHelper::stringToColour(GetConfigString(BaseName + ".TextColorClicked", "FFFFFFFF").GetString());
		bs.TextAlignX = GetConfigFloat(BaseName + ".TextAlignX", 0.5f);
		bs.TextAlignY = GetConfigFloat(BaseName + ".TextAlignY", 0.5f);
		bs.Width = AbsX(GetConfigInt(BaseName + ".Width", 128) / 1024.0f);
		bs.Height = AbsY(GetConfigInt(BaseName + ".Height", 32) / 768.0f);
		bs.Fading = GetConfigFloat(BaseName + ".Fading", 5.0f);
		ButtonStyles.PushBack(bs);

		++iStyle;
		StyleName = Styles.Extract(',', iStyle);
	}

	ButtonStyles.First();
	if (!ButtonStyles.End())
	{
		DefaultButtonStyle = &ButtonStyles.GetItem();
	}


	WindowTitleWidth = GetConfigInt("cegui.Window.TitleWidth", 0);
	WindowTitleHeight = GetConfigInt("cegui.Window.TitleHeight", 0);
}



void CGUIHelper::ReloadAllWindowsConfig()
{
	if (!CUTWindow::CEGUIInitialized) return;

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	static char KeysValues[32768];
	static char KeyNameStr[0100];
	String KeyName;

	GetPrivateProfileSection("Components", KeysValues, sizeof(KeysValues), GetConfigFileName("cegui"));

	int i = 0;
	while (KeysValues[i] != (char)0)
	{
		int j = i;
		while (KeysValues[j] != '=')
			++j;

		strncpy(KeyNameStr, &KeysValues[i], j - i);
		KeyNameStr[j - i] = (char)0;
		KeyName = KeyNameStr;

		if (winMgr.isWindowPresent(KeyNameStr))
		{
			CEGUI::Window *wnd = winMgr.getWindow(KeyNameStr);
			ReloadWindowConfig(wnd, KeyName, true, 0, 0, 0, 0, 0, 0, 0, 0);
		}

		while (KeysValues[j] != (char)0)
			++j;

		i = j + 1;
	}
}



CEGUI::Window* CGUIHelper::CreateWnd(StringParam UIName, StringParam Caption, float titlebarX, float x, float y, float w, float h, StringParam ParentName, StringParam WndVisible)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	String ConfigStr = GetConfigString(ConfigBaseName + UIName, "");
	String BackgroundImg = ConfigStr.Extract(',', 0);

	CEGUI::Window* tmpWindow = winMgr.createWindow("TaharezLook/FrameWindow", UIName.GetString());
	winMgr.getWindow((ParentName.GetLen() != 0) ? ParentName.GetString() : "RootWindow")->addChildWindow(tmpWindow);

	ReloadWindowConfig(tmpWindow, UIName, false, x, 0, y, 0, w, 0, h, 0);

	String tmpStr;
	tmpStr = ConfigStr.Extract(',', 5);
	if (tmpStr.GetLen() != 0)
	{
		titlebarX = tmpStr.AsFloat();
	}

	if (BackgroundImg.GetLen() == 0)
	{
		tmpWindow->setProperty("FrameEnabled", "true");
	}
	else
	{
		tmpWindow->setProperty("FrameEnabled", "false");
	}

	tmpWindow->setProperty("CloseButtonEnabled","false");
	tmpWindow->setProperty("DragMovingEnabled","false");
	tmpWindow->setProperty("SizingEnabled","false");
	tmpWindow->setProperty("TitlebarEnabled","false");
	tmpWindow->setMouseCursor("TaharezLook","MouseArrow");
	tmpWindow->setProperty("Visible",WndVisible.GetString());


	if ((Caption.GetLen() != 0) && (WindowTitleWidth > 0) && (WindowTitleHeight > 0))
	{
		float winW = tmpWindow->getPixelSize().d_width;
		float winH = tmpWindow->getPixelSize().d_height;
		float ww = AbsX(OffsetX(WindowTitleWidth)) / winW;
		float hh = AbsY(OffsetY(WindowTitleHeight)) / winH;
		CEGUI::Window* TitleBar = CreateStaticImage(UIName + ":TitleBar", "Titlebar", titlebarX * (1.0f - ww), -hh - 1.0f / winH, ww, hh, UIName);
		TitleBar->setProperty("ClippedByParent", "false");
		CEGUI::Window *CaptionWnd;
		CaptionWnd = CreateText(UIName + ":TitleBar_Caption", Caption, 0.0f, 0.0f, 1.0f, 1.0f, CaptionFont, "HorzCentred", "VertCentred", UIName + ":TitleBar");
		CaptionWnd->setProperty("TextColours", CEGUI::PropertyHelper::colourRectToString(CEGUI::ColourRect(CaptionColor)));
	}


	if (BackgroundImg.GetLen() != 0)
	{
		CEGUI::Window* Img = CreateStaticImage(UIName + ":Background", BackgroundImg, 0.0f, 0.0f, 1.0f, 1.0f, UIName);
		Img->setProperty("ZOrderChangeEnabled", "false");
	}


	return tmpWindow;
}


CEGUI::Combobox* CGUIHelper::CreateCombobox(StringParam UIName, float x, float y, float w, float h, Strings *rows, StringParam Parent, bool (*function)(const CEGUI::EventArgs &e))
{
	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Combobox *pCombobox = static_cast<CEGUI::Combobox*>(winMgr.createWindow("TaharezLook/Combobox", UIName.GetString()));
	winMgr.getWindow((Parent.GetLen() != 0) ? Parent.GetString() : "RootWindow")->addChildWindow(pCombobox);

	//pCombobox->setAlwaysOnTop(true);
	pCombobox->setProperty("Visible","true");
	pCombobox->setReadOnly(true);
	pCombobox->setMouseCursor("TaharezLook", "MouseArrow");
	pCombobox->setShowHorzScrollbar(false);
	
	MyListItem *pDefault = NULL;
	if(!rows->Empty())
	{
		rows->First();
		pDefault = new MyListItem(rows->GetItem(), UIName);
		pCombobox->addItem(pDefault);
		rows->Next();

		while(!rows->End())
		{
		    pCombobox->addItem(new MyListItem(rows->GetItem(), UIName));
			rows->Next();
		}

		pCombobox->setText(pDefault->getText());
	}

	ReloadWindowConfig(pCombobox, UIName, false, x, 0, y, 0, w, 0, h, 0);

	pCombobox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(function));

	ReloadWindowConfig(pCombobox->getEditbox(), pCombobox->getEditbox()->getName().c_str(), true, 0, 0, 0, 0, 0, 0, 0, 0);
	ReloadWindowConfig(pCombobox->getPushButton(), pCombobox->getPushButton()->getName().c_str(), true, 0, 0, 0, 0, 0, 0, 0, 0);
	ReloadWindowConfig(pCombobox->getDropList(), pCombobox->getDropList()->getName().c_str(), true, 0, 0, 0, 0, 0, 0, 0, 0);

	return pCombobox;
}



CEGUI::Checkbox* CGUIHelper::CreateCheckbox(StringParam UIName, float x, float y, float w, float h, StringParam Parent, bool (*function)(const CEGUI::EventArgs &e))
{
	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Checkbox* Check = static_cast<CEGUI::Checkbox*>(winMgr.createWindow("TaharezLook/Checkbox", UIName.GetString()));
	winMgr.getWindow((Parent.GetLen() != 0) ? Parent.GetString() : "RootWindow")->addChildWindow(Check);

	ReloadWindowConfig(Check, UIName, false, x, 0, y, 0, w, 0, h, 0);

	Check->setMouseCursor("TaharezLook","MouseTarget");
	Check->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(function));

	return Check;
}



CEGUI::Editbox* CGUIHelper::CreateEditbox(StringParam UIName, float x, float y, float w, float h, StringParam DefaultText, CEGUI::colour SelTextColor, StringParam Parent)
{
	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Editbox* Edit = static_cast<CEGUI::Editbox*>(winMgr.createWindow("TaharezLook/Editbox", UIName.GetString()));
	winMgr.getWindow((Parent.GetLen() != 0) ? Parent.GetString() : "RootWindow")->addChildWindow(Edit);

	Edit->setText(DefaultText.GetString());
	Edit->setProperty("SelectedTextColour", CEGUI::PropertyHelper::colourRectToString(CEGUI::ColourRect(SelTextColor)).c_str());
	Edit->setMouseCursor("TaharezLook","MouseTextBar");

	ReloadWindowConfig(Edit, UIName, false, x, 0, y, 0, w, 0, h, 0);

	return Edit;
}



CEGUI::ProgressBar*	CGUIHelper::CreateProgressBar(StringParam UIName, float x, float y, float w, float h, StringParam Parent)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::ProgressBar *PB = static_cast<CEGUI::ProgressBar *> (CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/ProgressBar", UIName.GetString()));
	winMgr.getWindow((Parent.GetLen() != 0) ? Parent.GetString() : "RootWindow")->addChildWindow(PB);

	ReloadWindowConfig(PB, UIName, false, x, 0, y, 0, w, 0, h, 0);

	//PB->setAlwaysOnTop(true);
	PB->setProgress(0.0f);
	PB->setMouseCursor("TaharezLook","MouseArrow");
	return PB;
}


CEGUI::Window* CGUIHelper::CreateButton(StringParam UIName, StringParam Caption, float x, float y, float w, float h, StringParam ParentName, bool (*function)(const CEGUI::EventArgs &e))
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *Button, *Title;

	String ButtonConfig = GetConfigString(ConfigBaseName + UIName, "");
	CButtonStyle* style = GetButtonStyle(ButtonConfig.Extract(',', 0));

	CEGUI::Window* Base = winMgr.createWindow("TaharezLook/FrameWindow", UIName.GetString());
	winMgr.getWindow((ParentName.GetLen() != 0) ? ParentName.GetString() : "RootWindow")->addChildWindow(Base);

	ReloadWindowConfig(Base, UIName, false, x, 0, y, 0, 0, (float)style->Width, 0, (float)style->Height);
	Base->setProperty("FrameEnabled", "false");
	Base->setProperty("CloseButtonEnabled","false");
	Base->setProperty("DragMovingEnabled","false");
	Base->setProperty("SizingEnabled","false");
	Base->setProperty("TitlebarEnabled","false");
	Base->setMouseCursor("TaharezLook","MouseArrow");

	Base->setUserData(style);


	Button = CreateStaticImage(UIName + ":Normal", (String)"Button_" + style->Name + (String)"_Normal", 0.0f, 0.0f, 1.0f, 1.0f, UIName);
	Button->setProperty("ZOrderChangeEnabled", "false");

	Button->subscribeEvent(CEGUI::PushButton::EventMouseMove,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonTitleEnters));
	Button->subscribeEvent(CEGUI::PushButton::EventMouseLeaves,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonTitleLeaves));
	Button->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonMouseDown));
	Button->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonMouseUp));


	Button = CreateStaticImage(UIName + ":Over", (String)"Button_" + style->Name + (String)"_Over", 0.0f, 0.0f, 1.0f, 1.0f, UIName);
	Button->setProperty("ZOrderChangeEnabled", "false");
	Button->setAlpha(0.0f);

	Button->subscribeEvent(CEGUI::PushButton::EventMouseMove,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonTitleEnters));
	Button->subscribeEvent(CEGUI::PushButton::EventMouseLeaves,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonTitleLeaves));
	Button->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonMouseDown));
	Button->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonMouseUp));


	Button = CreateStaticImage(UIName + ":Clicked", (String)"Button_" + style->Name + (String)"_Clicked", 0.0f, 0.0f, 1.0f, 1.0f, UIName);
	Button->setProperty("ZOrderChangeEnabled", "false");
	Button->setAlpha(0.0f);

	Button->subscribeEvent(CEGUI::PushButton::EventMouseMove,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonTitleEnters));
	Button->subscribeEvent(CEGUI::PushButton::EventMouseLeaves,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonTitleLeaves));
	Button->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonMouseDown));
	Button->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonMouseUp));


	String NewCaption;
	NewCaption = GetConfigString(ConfigBaseName + UIName + ":Title", "");
	if (NewCaption.GetLen() == 0)
	{
		NewCaption = CLocalizationSettings::Instance()->GetLocalText(Caption);
	}

	Title = CreateText(UIName + ":Title", NewCaption, style->TextAlignX - 0.5f, style->TextAlignY - 0.5f, 1.0f, 1.0f, style->TextFont, "HorzCentred", "VertCentred", UIName);
	Title->setProperty("TextColours", CEGUI::PropertyHelper::colourRectToString(CEGUI::ColourRect(style->TextColorNormal)));

	Title->subscribeEvent(CEGUI::PushButton::EventMouseMove,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonTitleEnters));
	Title->subscribeEvent(CEGUI::PushButton::EventMouseLeaves,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonTitleLeaves));
	Title->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonMouseDown));
	Title->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp,CEGUI::Event::Subscriber(&CGUIHelper::OnButtonMouseUp));

	if (function)
	{
		bool found = false;

		for (ButtonsData.First(); !ButtonsData.End(); ButtonsData.Next())
		{
			if (ButtonsData.GetItem().Name == UIName)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			CButtonData data;
			data.Name = UIName;
			data.Function = function;
			ButtonsData.PushBack(data);
		}		
	}


	return Base;
}



CEGUI::MultiColumnList* CGUIHelper::CreateMultiList(StringParam UIName, float x, float y, float w, float h, CSmartList<String> &ColumnNames, bool EnableSorting, bool RowSelecting, StringParam ParentName)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::MultiColumnList* MultiList = static_cast<CEGUI::MultiColumnList*>(winMgr.createWindow("TaharezLook/MultiColumnList", UIName.GetString()));
	winMgr.getWindow((ParentName.GetLen() != 0) ? ParentName.GetString() : "RootWindow")->addChildWindow(MultiList);

	String ColumnNamesStr;
	ColumnNamesStr = GetConfigString(ConfigBaseName + UIName + "$ColumnNames", "");

	int i;
	for (ColumnNames.First(), i = 0; !ColumnNames.End(); ColumnNames.Next(), ++i)
	{
		String ColumnName = ColumnNamesStr.Extract(',', i);
		if (ColumnName.GetLen() == 0)
			ColumnName = ColumnNames.GetItem();
		MultiList->addColumn(CLocalizationSettings::Instance()->GetLocalText(ColumnName).GetString(), i, CEGUI::UDim(1.0f / ColumnNames.GetItemsCount(), 0.0f));
	}

	MultiList->setProperty("ColumnsSizable", "False");
	MultiList->setProperty("ColumnsMovable", "False");
	MultiList->setProperty("SortSettingEnabled", "False");

	ReloadWindowConfig(MultiList, UIName, false, x, 0, y, 0, w, 0, h, 0);

	if (RowSelecting)
		MultiList->setProperty("SelectionMode", "RowSingle");
	else
		MultiList->setProperty("SelectionMode", "CellSingle");

	return MultiList;
}



CEGUI::Window* CGUIHelper::CreateText(StringParam UIName, StringParam Caption, float x, float y, float w, float h, StringParam FontName, StringParam HorzAlignment, StringParam VertAlignment, StringParam ParentWnd)
{
	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Window *pStaticText = (winMgr.createWindow("TaharezLook/StaticText", UIName.GetString()));
	winMgr.getWindow((ParentWnd.GetLen() != 0) ? ParentWnd.GetString() : "RootWindow")->addChildWindow(pStaticText);

	if (FontName.GetLen() != 0)
	{
		pStaticText->setFont(FontName.GetString());
	}
	else
	{
		pStaticText->setFont(DefaultTextFont.GetString());
	}

	ReloadWindowConfig(pStaticText, UIName, false, x, 0, y, 0, w, 0, h, 0);
	pStaticText->setText(Caption.GetString());
	pStaticText->setProperty("Visible", "true");
	pStaticText->setProperty("TextColours", CEGUI::PropertyHelper::colourRectToString(CEGUI::ColourRect(TextColor)));
	pStaticText->setProperty("FrameEnabled", "false");
	pStaticText->setProperty("BackgroundEnabled", "false");
	//pStaticText->setAlwaysOnTop(true);
	pStaticText->setMouseCursor("TaharezLook","MouseArrow");
	pStaticText->setProperty("HorzFormatting", ((HorzAlignment.GetLen() != 0) ? HorzAlignment.GetString() : "WordWrapLeftAligned"));
	pStaticText->setProperty("VertFormatting", ((VertAlignment.GetLen() != 0) ? VertAlignment.GetString() : "TopAligned"));

	return pStaticText;
}


CEGUI::Window* CGUIHelper::CreateStaticImage(StringParam UIName, StringParam ImageName, float x, float y, float w, float h, StringParam Parent)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* SImage;

	SImage = (winMgr.createWindow("TaharezLook/StaticImage", UIName.GetString()));
	winMgr.getWindow((Parent.GetLen() != 0) ? Parent.GetString() : "RootWindow")->addChildWindow(SImage);

	ReloadWindowConfig(SImage, UIName, false, x, 0, y, 0, w, 0, h, 0);
    SImage->setProperty("FrameEnabled", "false");
    SImage->setProperty("BackgroundEnabled", "false");
	if (ImageName.GetLen() != 0)
	{
		SImage->setProperty("Image", ((String)"set:TaharezLook image:" + ImageName).GetString());
	}
	SImage->setMouseCursor("TaharezLook","MouseArrow");
	//SImage->setAlwaysOnTop(true);

	return SImage;
}


CEGUI::Window* CGUIHelper::CreateStaticColor(StringParam UIName, WeColor &Color, float x, float y, float w, float h, StringParam Parent)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Window* Img = CreateStaticImage(UIName, "StaticColor", x, y, w, h, Parent);

	CEGUI::colour col;
	col.set(Color.r, Color.g, Color.b, Color.a);
	Img->setProperty("Colour", CEGUI::PropertyHelper::colourRectToString(CEGUI::ColourRect(col)).c_str());

	return Img;
}



CEGUI::Scrollbar* CGUIHelper::CreateHorzScrollbar(StringParam UIName, float x, float y, float w, float h, float aPageSize, float aTotalSize,
												  StringParam Parent, bool (*function)(const CEGUI::EventArgs &e))
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Scrollbar* Base = static_cast<CEGUI::Scrollbar*>(winMgr.createWindow("TaharezLook/HorizontalScrollbar", UIName.GetString()));
	winMgr.getWindow((Parent.GetLen() != 0) ? Parent.GetString() : "RootWindow")->addChildWindow(Base);

	ReloadWindowConfig(Base, UIName, false, x, 0, y, 0, w, 0, h, 0);
	Base->setMouseCursor("TaharezLook","MouseArrow");

	Base->setPageSize(aPageSize);
	Base->setDocumentSize(aTotalSize);
	Base->setOverlapSize(0.0f);
	Base->setStepSize(aPageSize);

	Base->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(function));

	return Base;
}



bool FadeFloat(float &Value, float DestValue, float delta)
{
	if (Value < DestValue)
	{
		Value += delta;
		if (Value >= DestValue)
		{
			Value = DestValue;
			return true;
		}
	}
	else
	{
		Value -= delta;
		if (Value <= DestValue)
		{
			Value = DestValue;
			return true;
		}
	}

	return false;
}


bool FadeColorToColor(CEGUI::colour &Color, CEGUI::colour &DestColor, float delta)
{
	int OkCount = 0;

	float r = Color.getRed();
	float g = Color.getGreen();
	float b = Color.getBlue();
	float a = Color.getAlpha();

	if (FadeFloat(r, DestColor.getRed(), delta))
		++OkCount;
	if (FadeFloat(g, DestColor.getGreen(), delta))
		++OkCount;
	if (FadeFloat(b, DestColor.getBlue(), delta))
		++OkCount;
	if (FadeFloat(a, DestColor.getAlpha(), delta))
		++OkCount;

	Color.set(r, g, b, a);

	return (OkCount == 4);
}



bool FadeButton(StringParam ButtonName, CButtonStyle* style, float NormalAlpha, float OverAlpha, float ClickedAlpha, CEGUI::argb_t &TextColor)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *Base, *Button, *Title;
	CEGUI::colour color;
	float alpha;
	bool fadedOver, fadedClicked, fadedText;

	float delta = WeTimer::Instance()->GetDeltaTimeF() * style->Fading;

	Base = winMgr.getWindow(ButtonName.GetString());

	//Button = winMgr.getWindow((ButtonName + ":Normal").GetString());
	//alpha = Button->getAlpha();
	//faded = FadeFloat(alpha, NormalAlpha, delta) && faded;
	//Button->setAlpha(alpha);

	Button = winMgr.getWindow((ButtonName + ":Over").GetString());
	alpha = Button->getAlpha();
	fadedOver = FadeFloat(alpha, OverAlpha, delta);
	Button->setAlpha(alpha);

	Button = winMgr.getWindow((ButtonName + ":Clicked").GetString());
	alpha = Button->getAlpha();
	fadedClicked = FadeFloat(alpha, ClickedAlpha, delta);
	Button->setAlpha(alpha);

	Title = winMgr.getWindow((ButtonName + ":Title").GetString());
	color = CEGUI::PropertyHelper::stringToColourRect(Title->getProperty("TextColours").c_str()).d_top_left;
	fadedText = FadeColorToColor(color, CEGUI::colour(TextColor), delta);
	Title->setProperty("TextColours", CEGUI::PropertyHelper::colourRectToString(CEGUI::ColourRect(color)).c_str());

	return fadedOver && fadedClicked && fadedText;
}



bool CGUIHelper::WindowFade(StringParam WNDName, float &WNDAlpha)
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	float FadeInTime = 0.06f;

	if(WNDAlpha >= 1.0f)
	{
		WNDAlpha = 1.0f;
		winMgr.getWindow(WNDName.GetString())->setAlpha(WNDAlpha);
		return false;
	}
	else
	{
		winMgr.getWindow(WNDName.GetString())->setAlpha(WNDAlpha);
	}
	
	WNDAlpha += FadeInTime;
	
	return true;
}



/*
bool CGUIHelper::OnButtonEnters(const CEGUI::EventArgs& e)
{
	CEGUI::MouseEventArgs *me = (CEGUI::MouseEventArgs*)&e;
	String NewOverButtonName = me->window->getParent()->getName().c_str();

	if (OverButtonName != NewOverButtonName)
	{
		if (OverButtonName.GetLen() != 0)
		{
			FadeOutButtonNames.PushBackUnique(OverButtonName);
		}
		OverButtonName = NewOverButtonName;
	}

	return true;
}



bool CGUIHelper::OnButtonLeaves(const CEGUI::EventArgs& e)
{
	CEGUI::MouseEventArgs *me = (CEGUI::MouseEventArgs*)&e;
	String NewOverButtonName = me->window->getParent()->getName().c_str();

	if (NewOverButtonName == OverButtonName)
	{
		OverButtonName = "";
	}
	FadeOutButtonNames.PushBackUnique(NewOverButtonName);

	return true;
}
*/



bool CGUIHelper::OnButtonTitleEnters(const CEGUI::EventArgs& e)
{
	CEGUI::MouseEventArgs *me = (CEGUI::MouseEventArgs*)&e;
	String NewOverButtonName = me->window->getParent()->getName().c_str();
	
	if (NewOverButtonName != OverButtonName)
	{
		if ((OverButtonName.GetLen() != 0) && (OverButtonName != ClickedButtonName))
		{
			FadeOutButtonNames.PushBackUnique(OverButtonName);
		}
		OverButtonName = NewOverButtonName;
	}

	return true;
}



bool CGUIHelper::OnButtonTitleLeaves(const CEGUI::EventArgs& e)
{
	CEGUI::MouseEventArgs *me = (CEGUI::MouseEventArgs*)&e;
	String NewOverButtonName = me->window->getParent()->getName().c_str();

	if (NewOverButtonName == OverButtonName)
	{
		OverButtonName = "";
	}

	if (NewOverButtonName != ClickedButtonName)
	{
		FadeOutButtonNames.PushBackUnique(NewOverButtonName);
	}

	return true;
}



bool CGUIHelper::OnButtonMouseDown(const CEGUI::EventArgs& e)
{
	CEGUI::MouseEventArgs *me = (CEGUI::MouseEventArgs*)&e;
	String NewClickedButtonName = me->window->getParent()->getName().c_str();

	me->window->captureInput();

	if (ClickedButtonName.GetLen() == 0)
	{
		ClickedButtonName = NewClickedButtonName;
	}
	else if (ClickedButtonName != NewClickedButtonName)
	{
		if (ClickedButtonName != OverButtonName)
		{
			FadeOutButtonNames.PushBackUnique(ClickedButtonName);
		}
		ClickedButtonName = NewClickedButtonName;
	}

	return true;
}



bool CGUIHelper::OnButtonMouseUp(const CEGUI::EventArgs& e)
{
	CEGUI::MouseEventArgs *me = (CEGUI::MouseEventArgs*)&e;
	String NewClickedButtonName = me->window->getParent()->getName().c_str();

	me->window->releaseInput();

	if (ClickedButtonName.GetLen() != 0)
	{
		if (ClickedButtonName != OverButtonName)
		{
			FadeOutButtonNames.PushBackUnique(ClickedButtonName);
		}
		else if (ClickedButtonName == NewClickedButtonName)
		{
			for (ButtonsData.First(); !ButtonsData.End(); ButtonsData.Next())
			{
				if (ButtonsData.GetItem().Name == ClickedButtonName)
				{
					ButtonsData.GetItem().Function(e);
					break;
				}
			}
		}

		ClickedButtonName = "";
	}

	return true;
}



void CGUIHelper::Render()
{
	CEGUI::WindowManager* winMgr = &CEGUI::WindowManager::getSingleton();
	if (!winMgr)
		return;

	if (ClickedButtonName.GetLen() != 0)
	{
		if (!winMgr->isWindowPresent(ClickedButtonName.GetString()))
		{
			ClickedButtonName = "";
		}
		else
		{
			CButtonStyle* style = (CButtonStyle*)winMgr->getWindow(ClickedButtonName.GetString())->getUserData();
			FadeButton(ClickedButtonName, style, 0.0f, 0.0f, 1.0f, style->TextColorClicked);
		}
	}
	else if (OverButtonName.GetLen() != 0)
	{
		if (!winMgr->isWindowPresent(OverButtonName.GetString()))
		{
			OverButtonName = "";
		}
		else
		{
			CButtonStyle* style = (CButtonStyle*)winMgr->getWindow(OverButtonName.GetString())->getUserData();
			FadeButton(OverButtonName, style, 0.0f, 1.0f, 0.0f, style->TextColorOver);
		}
	}

	for (FadeOutButtonNames.First(); !FadeOutButtonNames.End(); )
	{
		if (!winMgr->isWindowPresent(FadeOutButtonNames.GetItem().GetString()))
		{
			FadeOutButtonNames.Delete();
		}
		else if (FadeOutButtonNames.GetItem() == OverButtonName)
		{
			FadeOutButtonNames.Delete();
		}
		else if (FadeOutButtonNames.GetItem() == ClickedButtonName)
		{
			FadeOutButtonNames.Delete();
		}
		else
		{
			CButtonStyle* style = (CButtonStyle*)winMgr->getWindow(FadeOutButtonNames.GetItem().GetString())->getUserData();
			if (FadeButton(FadeOutButtonNames.GetItem(), style, 1.0f, 0.0f, 0.0f, style->TextColorNormal))
			{
				FadeOutButtonNames.Delete();
			}
			else
			{
				FadeOutButtonNames.Next();
			}
		}
	}
}




MyListItem::MyListItem(StringParam text, StringParam aParentName) : ListboxTextItem(text.GetString())
{
	ParentName = aParentName;

	setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
	SetSelectedColorConfigName("cegui.List.SelectedColor");
}



void MyListItem::SetSelectedColorConfigName(StringParam aSelectedColorConfigName)
{
	String ConfigColor;

	if (ParentName.GetLen() == 0)
		ConfigColor = GetConfigString("cegui.List.UnselectedColor", "");
	else
		ConfigColor = GetConfigString(aSelectedColorConfigName, "");

	if (ConfigColor.GetLen() != 0)
	{
		setSelectionColours(CEGUI::PropertyHelper::stringToColour(ConfigColor.GetString()));
	}
}
