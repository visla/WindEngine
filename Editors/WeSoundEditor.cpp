/*
     File: WeSoundEditor.cpp
	 Author: Tomislav Kukic
	 Date: 10.01.2006.
	 Version: 1.0
*/




#include <stdio.h>
#include <WePackage.h>
#include <WePackage.h>
#include <WeSoundEditor.h>



WeSoundEditor::WeSoundEditor()
{
}



bool WeSoundEditor::Save(StringParam HoleName, bool Replace)
{
	//if(Replace) WePackage::Instance()->Remove(HoleName);

	FILE *f = fopen(Name, "rb");
	fseek(f,0,SEEK_END);
	DWORD Size = ftell(f);
	fclose(f);

	f = fopen(Name, "rb");

	char* SoundData;
	SoundData = new char[Size];
	fread((void*)SoundData, Size, 1, f);
	fclose(f);

	if(WePackage::Instance()->BeginWriting(HoleName, 4, true))
	{
		WePackage::Instance()->DataStream.WriteDWord(Size);
		WePackage::Instance()->DataStream.WriteData((void*)SoundData, (int)Size);

	}else return false;

	WePackage::Instance()->EndWriting();

	SAFE_DELETE(SoundData);
	return true;
}

