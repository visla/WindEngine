/*
		File: CLocalizationSettings.h
		Author: Tomislav Kukic
		Date: 28.07.2006
		Version: 1.0
*/





#ifndef _CLOCALIZATIONSETTINGS_H_
#define _CLOCALIZATIONSETTINGS_H_


#include <SmartList.h>
#include <CString.h>



struct LocalText
{
	String LocalName;
	String LocalizedText;
};


class CLocalizationSettings
{
protected:
	static CLocalizationSettings* _Instance;

    CSmartList<LocalText*> LocalTexts;
	void Initialize(StringParam LegName);
	void LoadParamsFromFile(StringParam LegName);
	void SetText(StringParam strName, StringParam strText);
	void GetAllPacks();
	void FillItemsWithText(StringParam strName);
	bool CheckLanguageName(StringParam strName);

	String LanguageName;
	Strings LanguageNames;

public:
	static CLocalizationSettings* Instance();

	String GetLocalText(StringParam strName);
	void ClearAll();
};





#endif

