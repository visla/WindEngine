/*
      File:  WeEffectEditor.cpp
	  Author: Tomislav Kukic
	  Date: 14.12.2005.
	  Version: 1.0
*/


#include <WeEffectEditor.h>
#include <WePackage.h>




WeEffectEditor::WeEffectEditor()
{
	EffectCompiler = NULL;
}



WeEffectEditor::~WeEffectEditor()
{
	Destroy();
}




bool WeEffectEditor::Save(StringParam SaveName, bool Replace)
{
	char* Data;
	int Size;

	if (EffectCompiler) 
	{
		LPD3DXBUFFER Buffer = NULL;
		LPD3DXBUFFER Error = NULL;
		HRESULT hr = EffectCompiler->CompileEffect(0,&Buffer, &Error);
		if(S_OK != hr)
		{
			//if(hr == E_FAIL)
			//{
				MessageBox(0,(char*)Error->GetBufferPointer(),SaveName.GetString(),0);
				return false;
			//}
		}

		Size = (int)Buffer->GetBufferSize();
		Data = new char[Size];
		memcpy(Data, Buffer->GetBufferPointer(), Size);

		SAFE_RELEASE(Buffer);
		SAFE_RELEASE(Error);
	}
	else
	{
		if (WePackage::Instance()->BeginReading(Name, 3))
		{
			Size = WePackage::Instance()->DataStream.ReadInt();
			Data = new char[Size];
			WePackage::Instance()->DataStream.ReadData(Data, Size);
			WePackage::Instance()->EndReading();
		}
		else
			return false;
	}

	if (WePackage::Instance()->BeginWriting(SaveName, 3, true))
	{
		WePackage::Instance()->DataStream.WriteInt(Size);
		WePackage::Instance()->DataStream.WriteData(Data, Size);
		WePackage::Instance()->EndWriting();
	}
	else
		return false;

	SAFE_DELETE(Data);
	return true;
}



void WeEffectEditor::Destroy()
{
	SAFE_RELEASE(EffectCompiler);
}


