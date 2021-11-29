/*
		File: CLocalizationSettings.cpp
		Author: Tomislav Kukic
		Date: 28.07.2006
		Version: 1.0
*/





#include <CLocalizationSettings.h>
#include <WeLoger.h>



CLocalizationSettings* CLocalizationSettings::_Instance;

CLocalizationSettings* CLocalizationSettings::Instance()
{
	static bool Inited = false;
	if (!Inited)
	{
		_Instance = new CLocalizationSettings;
		_Instance->Initialize("Language");
		Inited = true;
	}

	return _Instance;
}



void CLocalizationSettings::LoadParamsFromFile(StringParam LegName)
{
	String FileName = LegName + ".utlp";

	FILE* file = fopen(FileName, "rb");

	if (file)
	{
		String tmpText = "";
		String tmpName = "";
		char tmpChar[2];
		int ch;
		while(1)
		{
			ch = fgetc(file);
			if(feof(file)) break;
			tmpChar[0] = (char)ch;
			tmpChar[1] = '\0';
			if(tmpChar[0] == '=')
			{
				tmpName = tmpText;
				tmpText = (String)"";
				continue;
			}else{
				if((int)ch == 13)
				{
					SetText(tmpName, tmpText);
					tmpName = (String)"";
					tmpText = (String)"";
				}
				else
				{
					if ((int)ch != 10)
					{
						tmpText = tmpText + tmpChar;
					}
				}
			}
		}

		fclose(file);
	}
}



void CLocalizationSettings::GetAllPacks()
{
	WIN32_FIND_DATA FindData;
	HANDLE fHandle;
	String tmp;

	LanguageNames.Clear();
	
	fHandle = FindFirstFile("*.utlp", &FindData);
	if (fHandle != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			tmp = FindData.cFileName;
			tmp = tmp.Extract('.', 0);

			if(tmp != (String)"Template")
				LanguageNames.PushBack(tmp);
		} while (FindNextFile(fHandle, &FindData));
			FindClose(fHandle);
	}
}



void CLocalizationSettings::FillItemsWithText(StringParam strName)
{
	if(!CheckLanguageName(strName)) LanguageName = (String)"Language";
	LoadParamsFromFile(LanguageName);
}



bool CLocalizationSettings::CheckLanguageName(StringParam strName)
{
	for(LanguageNames.First(); !LanguageNames.End(); LanguageNames.Next())
	{
		if(LanguageNames.GetItem() == strName) 
		{
			LanguageName = strName;
			return true;
		}
	}

	return false;
}



String CLocalizationSettings::GetLocalText(StringParam strName)
{
	for (LocalTexts.First(); !LocalTexts.End(); LocalTexts.Next())
	{
		//WeLoger::DebugLog("%s <-> %s\n", LocalTexts.GetItem()->LocalName.GetString(), strName.GetString());
		if (LocalTexts.GetItem()->LocalName == strName)
			return LocalTexts.GetItem()->LocalizedText;
	}

	return strName;
}



void CLocalizationSettings::SetText(StringParam strName, StringParam strText)
{
	for(LocalTexts.First(); !LocalTexts.End(); LocalTexts.Next())
	{
		if(LocalTexts.GetItem()->LocalName == strName)
		{
			LocalTexts.GetItem()->LocalizedText = strText;
			return;
		}
	}

	
	LocalText* tempLT = NULL;
	tempLT = new LocalText;
	tempLT->LocalName = strName;
	tempLT->LocalizedText = strText;
	LocalTexts.PushBack(tempLT);
}




void CLocalizationSettings::ClearAll()
{
	for(LocalTexts.First(); LocalTexts.End(); LocalTexts.Next())
	{
		SAFE_DELETE(LocalTexts.GetItem());
		LocalTexts.Delete();
	}
	LocalTexts.Clear();
}



void CLocalizationSettings::Initialize(StringParam LegName)
{
	GetAllPacks();
	FillItemsWithText(LegName);
}


