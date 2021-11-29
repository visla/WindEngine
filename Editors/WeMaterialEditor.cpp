/*
        File: WeMaterialEditor.cpp
		Author: Tomislav Kukic
		Date: 15.12.2005.
		Version: 1.0
*/


#include <WePackage.h>
#include <WeMaterialEditor.h>



WeMaterialEditor::~WeMaterialEditor()
{
	Destroy();
}



bool WeMaterialEditor::Save(StringParam HoleName, bool Replace)
{
	//if(Replace) WePackage::Instance()->Remove(HoleName);

	if(WePackage::Instance()->BeginWriting(HoleName, 2, Replace))
	{
		WePackage::Instance()->DataStream.WriteInt(Version);
		WePackage::Instance()->DataStream.WriteInt(Type);
		WePackage::Instance()->DataStream.WriteInt(BlendType);
		WePackage::Instance()->DataStream.WriteInt(ReflectionType);
		WePackage::Instance()->DataStream.WriteBool(AcceptLights);
		WePackage::Instance()->DataStream.WriteBool(Glowing);
		WePackage::Instance()->DataStream.WriteBool(Refractive);
		WePackage::Instance()->DataStream.WriteBool(DoubleSided);

		WePackage::Instance()->DataStream.WriteString(Shader->Name);

		for(int i = 0; i < 16; i++)
		{
			if(Textures[i] != NULL)
			{
				WePackage::Instance()->DataStream.WriteString(Textures[i]->Name);
			}
			else
			{
				WePackage::Instance()->DataStream.WriteString(" ");
			}
		}

		WePackage::Instance()->DataStream.WriteInt(TextureAnimation);
		WePackage::Instance()->DataStream.WriteFloat(TextureAnimParam1);
		WePackage::Instance()->DataStream.WriteFloat(TextureAnimParam2);
		WePackage::Instance()->DataStream.WriteFloat(TextureAnimParam3);
		WePackage::Instance()->DataStream.WriteFloat(TextureAnimParam4);
		WePackage::Instance()->DataStream.WriteFloat(TextureAnimParam5);

		WePackage::Instance()->DataStream.WriteFloat(Param1);
		WePackage::Instance()->DataStream.WriteFloat(Param2);
		WePackage::Instance()->DataStream.WriteFloat(Param3);
		WePackage::Instance()->DataStream.WriteFloat(Param4);

		WePackage::Instance()->DataStream.WriteColor(MaterialColor);		
		WePackage::Instance()->DataStream.WriteFloat(BumpIntensity);
		WePackage::Instance()->DataStream.WriteFloat(SpecularPow);
		WePackage::Instance()->DataStream.WriteDWord(AlphaRef);

		WePackage::Instance()->EndWriting();
	}else{
		return false;
	}

	return true;
}


void WeMaterialEditor::Destroy()
{
}

