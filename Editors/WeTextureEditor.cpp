/*
       File: WeTextureEditor.cpp
	   Author: Tomislav Kukic
	   Date: 13.09.2005.
	   Version: 1.0
*/



#include <WePackage.h>
#include <WeTextureEditor.h>
#include <WeLoger.h>
#include <WeFileStream.h>



WeTextureEditor::WeTextureEditor(void)
{
}



WeTextureEditor::~WeTextureEditor(void)
{
	Destroy();
}


bool WeTextureEditor::Save(StringParam SaveName, bool Replace)
{
	int Size = 1;
	char* TexData = NULL;


	FILE *f = fopen(Name, "rb");
	if (f)
	{
		fseek(f, 0 , SEEK_END);
		Size = ftell(f);
		TexData = new char[Size];

		fseek(f, 0, SEEK_SET);
		fread((void*)TexData, Size, 1, f);
		fclose(f);
	}
	else if (WePackage::Instance()->BeginReading(Name, 1))
	{
		if (!(NoMipmaps && (Type == WETEXTYPE_2D)))
		{
			int ver = 0;
			Size = WePackage::Instance()->DataStream.ReadInt();
			if (Size < 0)
			{
				Size = -Size;
				ver = WePackage::Instance()->DataStream.ReadInt();
			}
			if (Size > 65000000)
			{
				WePackage::Instance()->EndReading();
				return false;
			}

			if (ver == 0)
			{
				TexData = new char[Size];
				WePackage::Instance()->DataStream.ReadData((void*)TexData, Size);
			}
			else if (ver == 1)
			{
				TexData = new char[Size];
				WePackage::Instance()->DataStream.ReadData((void*)TexData, Size);
			}
			else if (ver == 2)
			{
				TexData = new char[Size];
				WePackage::Instance()->DataStream.ReadData((void*)TexData, Size);
			}
			else if (ver == 3)
			{
				WePackage::Instance()->DataStream.ReadDWord(); // Type

				WePackage::Instance()->DataStream.ReadDWord(); // Format
				WePackage::Instance()->DataStream.ReadDWord(); // Usage
				WePackage::Instance()->DataStream.ReadDWord(); // Pool

				WePackage::Instance()->DataStream.ReadInt(); // Width
				WePackage::Instance()->DataStream.ReadInt(); // Height
				WePackage::Instance()->DataStream.ReadInt(); // Depth

				WePackage::Instance()->DataStream.ReadInt(); // LowestQuality

				WePackage::Instance()->DataStream.ReadDWord(); // ModifyTime.dwLowDateTime
				WePackage::Instance()->DataStream.ReadDWord(); // ModifyTime.dwHighDateTime

				WePackage::Instance()->DataStream.ReadBool(); // NoMipmaps

				TexData = new char[Size];
				WePackage::Instance()->DataStream.ReadData((void*)TexData, Size);
			}
		}

		WePackage::Instance()->EndReading();
	}
	else
	{
		return false;
	}

	if (WePackage::Instance()->BeginWriting(SaveName, 1, true))
	{
		WePackage::Instance()->DataStream.WriteInt(-Size); // use versioning
		WePackage::Instance()->DataStream.WriteInt(3); // version

		WePackage::Instance()->DataStream.WriteDWord(Type);

		WePackage::Instance()->DataStream.WriteDWord((DWORD)Format);
		WePackage::Instance()->DataStream.WriteDWord(Usage);
		WePackage::Instance()->DataStream.WriteDWord((DWORD)Pool);

		WePackage::Instance()->DataStream.WriteInt(Width);
		WePackage::Instance()->DataStream.WriteInt(Height);
		WePackage::Instance()->DataStream.WriteInt(Depth);	
		
		WePackage::Instance()->DataStream.WriteInt((int)LowestQuality);

		WePackage::Instance()->DataStream.WriteDWord(ModifyTime.dwLowDateTime);
		WePackage::Instance()->DataStream.WriteDWord(ModifyTime.dwHighDateTime);

		WePackage::Instance()->DataStream.WriteBool(NoMipmaps);

		if (NoMipmaps && (Type == WETEXTYPE_2D))
		{
			RECT r;
			r.left = 0;
			r.top = 0;
			r.right = Width;
			r.bottom = Height;

			D3DLOCKED_RECT lr;

			LPDIRECT3DTEXTURE9 Tex = (LPDIRECT3DTEXTURE9)Texture;
			Tex->LockRect(0, &lr, &r, 0);

			int Pitch;
			Pitch = lr.Pitch;
			if ((D3DFMT_DXT1 <= Format) && (Format <= D3DFMT_DXT5))
				Pitch /= 4;

			WePackage::Instance()->DataStream.WriteInt(Pitch);
			WePackage::Instance()->DataStream.WriteData(lr.pBits, Height * Pitch);

			Tex->UnlockRect(0);
		}
		else
			WePackage::Instance()->DataStream.WriteData((void*)TexData, Size);

		WePackage::Instance()->EndWriting();
	}
	else
	{
		return false;
	}

	SAFE_DELETE(TexData);

	return true;
}


void WeTextureEditor::Destroy()
{
	SAFE_RELEASE(Texture);
}

